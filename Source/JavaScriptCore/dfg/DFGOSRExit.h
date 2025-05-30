/*
 * Copyright (C) 2011-2021 Apple Inc. All rights reserved.
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

#if ENABLE(DFG_JIT)

#include "DFGOSRExitBase.h"
#include "DFGVariableEventStream.h"
#include "GPRInfo.h"
#include "MacroAssembler.h"
#include "MethodOfGettingAValueProfile.h"
#include "Operands.h"
#include "ValueRecovery.h"
#include <wtf/RefPtr.h>
#include <wtf/TZoneMalloc.h>

namespace JSC {

class ArrayProfile;
class CCallHelpers;

namespace Probe {
class Context;
} // namespace Probe

namespace Profiler {
class OSRExit;
} // namespace Profiler

namespace DFG {

class BasicBlock;
class SpeculativeJIT;
struct Node;

// This enum describes the types of additional recovery that
// may need be performed should a speculation check fail.
enum SpeculationRecoveryType : uint8_t {
    SpeculativeAdd,
    SpeculativeAddSelf,
    SpeculativeAddImmediate,
    BooleanSpeculationCheck
};

// === SpeculationRecovery ===
//
// This class provides additional information that may be associated with a
// speculation check - for example 
class SpeculationRecovery {
public:
    SpeculationRecovery(SpeculationRecoveryType type, GPRReg dest, GPRReg src)
        : m_src(src)
        , m_dest(dest)
        , m_type(type)
    {
        ASSERT(m_type == SpeculativeAdd || m_type == SpeculativeAddSelf || m_type == BooleanSpeculationCheck);
    }

    SpeculationRecovery(SpeculationRecoveryType type, GPRReg dest, int32_t immediate)
        : m_immediate(immediate)
        , m_dest(dest)
        , m_type(type)
    {
        ASSERT(m_type == SpeculativeAddImmediate);
    }

    SpeculationRecoveryType type() { return m_type; }
    GPRReg dest() { return m_dest; }
    GPRReg src() { return m_src; }
    int32_t immediate() { return m_immediate; }

private:
    // different recovery types may required different additional information here.
    union {
        GPRReg m_src;
        int32_t m_immediate;
    };
    GPRReg m_dest;

    // Indicates the type of additional recovery to be performed.
    SpeculationRecoveryType m_type;
};

JSC_DECLARE_NOEXCEPT_JIT_OPERATION(operationCompileOSRExit, void, (CallFrame*, void*));
JSC_DECLARE_NOEXCEPT_JIT_OPERATION(operationDebugPrintSpeculationFailure, void, (Probe::Context&));
JSC_DECLARE_NOEXCEPT_JIT_OPERATION(operationMaterializeOSRExitSideState, void, (VM*, const OSRExitBase*, EncodedJSValue*));

// === OSRExit ===
//
// This structure describes how to exit the speculative path by
// going into baseline code.
struct OSRExit : public OSRExitBase {
    OSRExit(ExitKind, JSValueSource, MethodOfGettingAValueProfile, SpeculativeJIT*, unsigned streamIndex, unsigned recoveryIndex = UINT_MAX);

    CodeLocationLabel<JSInternalPtrTag> m_patchableJumpLocation;

    JSValueSource m_jsValueSource;
    MethodOfGettingAValueProfile m_valueProfile;
    
    unsigned m_recoveryIndex;

    CodeLocationJump<JSInternalPtrTag> codeLocationForRepatch() const { return CodeLocationJump<JSInternalPtrTag>(m_patchableJumpLocation); }

    unsigned m_streamIndex;
    void considerAddingAsFrequentExitSite(CodeBlock* profiledCodeBlock)
    {
        OSRExitBase::considerAddingAsFrequentExitSite(profiledCodeBlock, ExitFromDFG);
    }

    static void compileExit(CCallHelpers&, VM&, const OSRExit&, const Operands<ValueRecovery>&, SpeculationRecovery*, uint32_t osrExitIndex);

private:
    static void emitRestoreArguments(CCallHelpers&, VM&, const Operands<ValueRecovery>&);
};

struct SpeculationFailureDebugInfo {
    WTF_MAKE_STRUCT_TZONE_ALLOCATED(SpeculationFailureDebugInfo);
    CodeBlock* codeBlock;
    ExitKind kind;
    uint32_t exitIndex;
    BytecodeIndex bytecodeIndex;
};

} } // namespace JSC::DFG

#endif // ENABLE(DFG_JIT)
