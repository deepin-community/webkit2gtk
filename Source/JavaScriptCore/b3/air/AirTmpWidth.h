/*
 * Copyright (C) 2015-2017 Apple Inc. All rights reserved.
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

#if ENABLE(B3_JIT)

#include "AirArg.h"
#include "B3Bank.h"

namespace JSC { namespace B3 { namespace Air {

class Code;

class TmpWidth {
public:
    TmpWidth();
    TmpWidth(Code&);
    ~TmpWidth();

    template <Bank bank>
    void recompute(Code&);

    // The width of a Tmp is the number of bits that you need to be able to track without some trivial
    // recovery. A Tmp may have a "subwidth" (say, Width32 on a 64-bit system) if either of the following
    // is true:
    //
    // - The high bits are never read.
    // - The high bits are always zero.
    //
    // This doesn't tell you which of those properties holds, but you can query that using the other
    // methods.
    inline Width width(Tmp) const;

    // Return the minimum required width for all defs/uses of this Tmp.
    inline Width requiredWidth(Tmp);

    // This indirectly tells you how much of the tmp's high bits are guaranteed to be zero. The number of
    // high bits that are zero are:
    //
    //     TotalBits - defWidth(tmp)
    //
    // Where TotalBits are the total number of bits in the register, so 64 on a 64-bit system.
    inline Width defWidth(Tmp) const;

    // This tells you how much of Tmp is going to be read.
    inline Width useWidth(Tmp) const;

    void setWidths(Tmp, Width useWidth, Width defWidth);

private:
    struct Widths {
        Widths() { }

        Widths(Bank bank)
            : use(minimumWidth(bank))
            , def(minimumWidth(bank))
        {
        }

        Widths(Width useArg, Width defArg)
            : use(useArg)
            , def(defArg)
        {
        }

        void dump(PrintStream& out) const;
        
        Width use;
        Width def;
    };

    inline Widths& widths(Tmp);
    inline const Widths& widths(Tmp) const;
    inline void addWidths(Tmp, Widths);

    Vector<Widths>& widthsVector(Bank bank)
    {
        return bank == GP ? m_widthGP : m_widthFP;
    }

    template <Bank bank>
    void ensureSize(Tmp);

    // These are initialized at the beginning of recompute<bank>, which is called in the constructor for both values of bank.
    Vector<Widths> m_widthGP;
    Vector<Widths> m_widthFP;
};

} } } // namespace JSC::B3::Air

#endif // ENABLE(B3_JIT)
