/*
 * Copyright (C) 2012-2018 Apple Inc. All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE. 
 */

#pragma once

#include <wtf/Compiler.h>

WTF_ALLOW_UNSAFE_BUFFER_USAGE_BEGIN

#if ENABLE(DFG_JIT)

#include "DFGSilentRegisterSavePlan.h"
#include "DFGSpeculativeJIT.h"
#include <wtf/FastMalloc.h>
#include <wtf/FunctionTraits.h>

namespace JSC { namespace DFG {

class SlowPathGenerator {
    WTF_MAKE_FAST_ALLOCATED;
public:
    SlowPathGenerator(SpeculativeJIT* jit)
        : m_currentNode(jit->m_currentNode)
        , m_streamIndex(jit->m_stream.size())
        , m_origin(jit->m_origin) 
    {
    }
    virtual ~SlowPathGenerator() { }
    void generate(SpeculativeJIT* jit)
    {
        m_label = jit->label();
        jit->m_currentNode = m_currentNode;
        jit->m_outOfLineStreamIndex = m_streamIndex;
        jit->m_origin = m_origin;
        generateInternal(jit);
        jit->m_outOfLineStreamIndex = std::nullopt;
        if (ASSERT_ENABLED)
            jit->abortWithReason(DFGSlowPathGeneratorFellThrough);
    }
    MacroAssembler::Label label() const { return m_label; }
    virtual MacroAssembler::Call call() const
    {
        RELEASE_ASSERT_NOT_REACHED(); // By default slow path generators don't have a call.
        return MacroAssembler::Call();
    }

    const NodeOrigin& origin() const  { return m_origin; }
    Node* currentNode() const { return m_currentNode; }

protected:
    virtual void generateInternal(SpeculativeJIT*) = 0;
    Node* m_currentNode;
    MacroAssembler::Label m_label;
    unsigned m_streamIndex;
    NodeOrigin m_origin;
};

template<typename JumpType>
class JumpingSlowPathGenerator : public SlowPathGenerator {
public:
    JumpingSlowPathGenerator(JumpType from, SpeculativeJIT* jit)
        : SlowPathGenerator(jit)
        , m_from(from)
        , m_to(jit->label())
    {
    }
    
protected:
    void linkFrom(SpeculativeJIT* jit)
    {
        m_from.link(jit);
    }
    
    void jumpTo(SpeculativeJIT* jit)
    {
        jit->jump().linkTo(m_to, jit);
    }

    JumpType m_from;
    MacroAssembler::Label m_to;
};

enum class ExceptionCheckRequirement : uint8_t {
    CheckNeeded,
    CheckNotNeeded
};

template<typename JumpType, typename DestinationType, typename SourceType, unsigned numberOfAssignments>
class AssigningSlowPathGenerator final : public JumpingSlowPathGenerator<JumpType> {
public:
    AssigningSlowPathGenerator(
        JumpType from, SpeculativeJIT* jit,
        DestinationType destination[numberOfAssignments],
        SourceType source[numberOfAssignments])
        : JumpingSlowPathGenerator<JumpType>(from, jit)
    {
        for (unsigned i = numberOfAssignments; i--;) {
            m_destination[i] = destination[i];
            m_source[i] = source[i];
        }
    }

private:
    void generateInternal(SpeculativeJIT* jit) final
    {
        this->linkFrom(jit);
        for (unsigned i = numberOfAssignments; i--;)
            jit->move(m_source[i], m_destination[i]);
        this->jumpTo(jit);
    }

    DestinationType m_destination[numberOfAssignments];
    SourceType m_source[numberOfAssignments];
};

template<typename JumpType, typename DestinationType, typename SourceType, unsigned numberOfAssignments>
inline std::unique_ptr<SlowPathGenerator> slowPathMove(
    JumpType from, SpeculativeJIT* jit, SourceType source[numberOfAssignments], DestinationType destination[numberOfAssignments])
{
    return makeUnique<AssigningSlowPathGenerator<JumpType, DestinationType, SourceType, numberOfAssignments>>(
        from, jit, destination, source);
}

template<typename JumpType, typename DestinationType, typename SourceType>
inline std::unique_ptr<SlowPathGenerator> slowPathMove(
    JumpType from, SpeculativeJIT* jit, SourceType source, DestinationType destination)
{
    SourceType sourceArray[1] = { source };
    DestinationType destinationArray[1] = { destination };
    return makeUnique<AssigningSlowPathGenerator<JumpType, DestinationType, SourceType, 1>>(
        from, jit, destinationArray, sourceArray);
}

template<typename JumpType, typename DestinationType, typename SourceType>
inline std::unique_ptr<SlowPathGenerator> slowPathMove(
    JumpType from, SpeculativeJIT* jit, SourceType source1, DestinationType destination1, SourceType source2, DestinationType destination2)
{
    SourceType sourceArray[2] = { source1, source2 };
    DestinationType destinationArray[2] = { destination1, destination2 };
    return makeUnique<AssigningSlowPathGenerator<JumpType, DestinationType, SourceType, 2>>(
        from, jit, destinationArray, sourceArray);
}

template<typename JumpType, typename FunctionType, typename ResultType>
class CallSlowPathGenerator : public JumpingSlowPathGenerator<JumpType> {
public:
    CallSlowPathGenerator(
        JumpType from, SpeculativeJIT* jit,
        SpillRegistersMode spillMode, ExceptionCheckRequirement requirement, ResultType result)
        : JumpingSlowPathGenerator<JumpType>(from, jit)
        , m_spillMode(spillMode)
        , m_exceptionCheckRequirement(requirement)
        , m_result(result)
    {
        if (m_spillMode == NeedToSpill)
            jit->silentSpillAllRegistersImpl(false, m_plans, extractResult(result));
    }
    
    MacroAssembler::Call call() const override
    {
        return m_call;
    }
    
protected:
    void setUp(SpeculativeJIT* jit)
    {
        this->linkFrom(jit);
        if (m_spillMode == NeedToSpill)
            jit->silentSpill(m_plans);
    }
    
    void recordCall(MacroAssembler::Call call)
    {
        m_call = call;
    }
    
    void tearDown(SpeculativeJIT* jit)
    {
        std::optional<GPRReg> exception;

        if (m_exceptionCheckRequirement == ExceptionCheckRequirement::CheckNeeded) {
            if (m_spillMode == NeedToSpill)
                exception = jit->tryHandleOrGetExceptionUnderSilentSpill<FunctionType>(m_plans, this->m_result);
            else
                jit->exceptionCheck(CCallHelpers::operationExceptionRegister<typename FunctionTraits<FunctionType>::ResultType>());
        }

        if constexpr (!std::is_same_v<ResultType, NoResultTag>)
            jit->setupResults(extractResult(this->m_result));

        if (m_spillMode == NeedToSpill)
            jit->silentFill(m_plans);

        if (m_exceptionCheckRequirement == ExceptionCheckRequirement::CheckNeeded && exception)
            jit->exceptionCheck(*exception);

        this->jumpTo(jit);
    }

    MacroAssembler::Call m_call;
    SpillRegistersMode m_spillMode;
    ExceptionCheckRequirement m_exceptionCheckRequirement;
    ResultType m_result;
    Vector<SilentRegisterSavePlan, 2> m_plans;
};

template<typename JumpType, typename FunctionType, typename ResultType, typename... Arguments>
class CallResultAndArgumentsSlowPathGenerator final : public CallSlowPathGenerator<JumpType, FunctionType, ResultType> {
public:
    CallResultAndArgumentsSlowPathGenerator(
        JumpType from, SpeculativeJIT* jit, FunctionType function,
        SpillRegistersMode spillMode, ExceptionCheckRequirement requirement, ResultType result, Arguments... arguments)
        : CallSlowPathGenerator<JumpType, FunctionType, ResultType>(from, jit, spillMode, requirement, result)
        , m_function(function)
        , m_arguments(std::forward<Arguments>(arguments)...)
    {
    }

private:
    template<size_t... ArgumentsIndex>
    void unpackAndGenerate(SpeculativeJIT* jit, std::index_sequence<ArgumentsIndex...>)
    {
        this->setUp(jit);
        jit->setupArguments<FunctionType>(std::get<ArgumentsIndex>(m_arguments)...);
        this->recordCall(jit->appendCall(m_function));
        this->tearDown(jit);
    }

    void generateInternal(SpeculativeJIT* jit) final
    {
        unpackAndGenerate(jit, std::make_index_sequence<std::tuple_size<std::tuple<Arguments...>>::value>());
    }

    FunctionType m_function;
    std::tuple<Arguments...> m_arguments;
};

template<typename JumpType, typename FunctionType, typename ResultType, typename... Arguments>
inline std::unique_ptr<SlowPathGenerator> slowPathCall(
    JumpType from, SpeculativeJIT* jit, FunctionType function,
    SpillRegistersMode spillMode, ExceptionCheckRequirement requirement,
    ResultType result, Arguments... arguments)
{
#if ENABLE(DFG_REGISTER_ALLOCATION_VALIDATION)
    jit->checkRegisterAllocationAgainstSlowPathCall(from);
#endif
    return makeUnique<CallResultAndArgumentsSlowPathGenerator<JumpType, FunctionType, ResultType, Arguments...>>(
        from, jit, function, spillMode, requirement, result, arguments...);
}

template<typename JumpType, typename FunctionType, typename ResultType, typename... Arguments>
inline std::unique_ptr<SlowPathGenerator> slowPathCall(
    JumpType from, SpeculativeJIT* jit, FunctionType function,
    ResultType result, Arguments... arguments)
{
    return slowPathCall(
        from, jit, function, NeedToSpill, ExceptionCheckRequirement::CheckNeeded, result, arguments...);
}

template<typename JumpType, typename FunctionType, typename ResultType, typename... Arguments>
class CallResultAndArgumentsSlowPathICGenerator final : public CallSlowPathGenerator<JumpType, FunctionType, ResultType> {
public:
    CallResultAndArgumentsSlowPathICGenerator(
        JumpType from, SpeculativeJIT* jit, StructureStubInfoIndex stubInfoConstant, GPRReg stubInfoGPR, CCallHelpers::Address slowPathOperationAddress, FunctionType function,
        SpillRegistersMode spillMode, ExceptionCheckRequirement requirement, ResultType result, Arguments... arguments)
        : CallSlowPathGenerator<JumpType, FunctionType, ResultType>(from, jit, spillMode, requirement, result)
        , m_stubInfoGPR(stubInfoGPR)
        , m_slowPathOperationAddress(slowPathOperationAddress)
        , m_function(function)
        , m_arguments(std::forward<Arguments>(arguments)...)
        , m_stubInfoConstant(stubInfoConstant)
    {
    }

private:
    template<size_t... ArgumentsIndex>
    void unpackAndGenerate(SpeculativeJIT* jit, std::index_sequence<ArgumentsIndex...>)
    {
        this->setUp(jit);
        jit->loadStructureStubInfo(m_stubInfoConstant, m_stubInfoGPR);
        jit->setupArguments<FunctionType>(std::get<ArgumentsIndex>(m_arguments)...);
        jit->appendCall(m_slowPathOperationAddress);
        this->tearDown(jit);
    }

    void generateInternal(SpeculativeJIT* jit) final
    {
        unpackAndGenerate(jit, std::make_index_sequence<std::tuple_size<std::tuple<Arguments...>>::value>());
    }

    GPRReg m_stubInfoGPR;
    CCallHelpers::Address m_slowPathOperationAddress;
    FunctionType m_function;
    std::tuple<Arguments...> m_arguments;
    StructureStubInfoIndex m_stubInfoConstant;
};

template<typename JumpType, typename FunctionType, typename ResultType, typename... Arguments>
inline std::unique_ptr<SlowPathGenerator> slowPathICCall(
    JumpType from, SpeculativeJIT* jit, StructureStubInfoIndex stubInfoConstant, GPRReg stubInfoGPR, CCallHelpers::Address slowPathOperationAddress, FunctionType function,
    SpillRegistersMode spillMode, ExceptionCheckRequirement requirement,
    ResultType result, Arguments... arguments)
{
    return makeUnique<CallResultAndArgumentsSlowPathICGenerator<JumpType, FunctionType, ResultType, Arguments...>>(from, jit, stubInfoConstant, stubInfoGPR, slowPathOperationAddress, function, spillMode, requirement, result, arguments...);
}

template<typename JumpType, typename FunctionType, typename ResultType, typename... Arguments>
inline std::unique_ptr<SlowPathGenerator> slowPathICCall(
    JumpType from, SpeculativeJIT* jit, StructureStubInfoIndex stubInfoConstant, GPRReg stubInfoGPR, CCallHelpers::Address slowPathOperationAddress, FunctionType function,
    ResultType result, Arguments... arguments)
{
    return slowPathICCall(from, jit, stubInfoConstant, stubInfoGPR, slowPathOperationAddress, function, NeedToSpill, ExceptionCheckRequirement::CheckNeeded, result, arguments...);
}

} } // namespace JSC::DFG

#endif // ENABLD(DFG_JIT)

WTF_ALLOW_UNSAFE_BUFFER_USAGE_END
