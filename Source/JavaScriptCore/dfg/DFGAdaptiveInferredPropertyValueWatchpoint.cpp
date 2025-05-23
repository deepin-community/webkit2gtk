/*
 * Copyright (C) 2015 Apple Inc. All rights reserved.
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

#include "config.h"
#include "DFGAdaptiveInferredPropertyValueWatchpoint.h"

#if ENABLE(DFG_JIT)

#include "CodeBlock.h"
#include "DFGCommon.h"
#include <wtf/TZoneMallocInlines.h>

namespace JSC { namespace DFG {

WTF_MAKE_TZONE_ALLOCATED_IMPL(AdaptiveInferredPropertyValueWatchpoint);

AdaptiveInferredPropertyValueWatchpoint::AdaptiveInferredPropertyValueWatchpoint(const ObjectPropertyCondition& key, CodeBlock* codeBlock)
    : Base(key)
    , m_codeBlock(codeBlock)
{
}

void AdaptiveInferredPropertyValueWatchpoint::initialize(const ObjectPropertyCondition& key, CodeBlock* codeBlock)
{
    Base::initialize(key);
    m_codeBlock = codeBlock;
}

void AdaptiveInferredPropertyValueWatchpoint::handleFire(VM&, const FireDetail& detail)
{
    dataLogLnIf(DFG::shouldDumpDisassembly(), "Firing watchpoint ", RawPointer(this), " (", key(), ") on ", *m_codeBlock);


    auto lambda = scopedLambda<void(PrintStream&)>([&](PrintStream& out) {
        out.print("Adaptation of ", key(), " failed: ", detail);
    });
    LazyFireDetail lazyDetail(lambda);
    m_codeBlock->jettison(Profiler::JettisonDueToUnprofiledWatchpoint, CountReoptimization, &lazyDetail);
}

bool AdaptiveInferredPropertyValueWatchpoint::isValid() const
{
    ASSERT(!m_codeBlock->wasDestructed());
    return !m_codeBlock->isPendingDestruction();
}

} } // namespace JSC::DFG

#endif // ENABLE(DFG_JIT)
