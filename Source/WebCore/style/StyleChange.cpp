/*
 * Copyright (C) 2013, 2016 Apple Inc. All rights reserved.
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
#include "StyleChange.h"

#include "RenderStyleConstants.h"
#include "RenderStyleInlines.h"
#include <wtf/text/AtomString.h>

namespace WebCore {
namespace Style {

Change determineChange(const RenderStyle& s1, const RenderStyle& s2)
{
    if (s1.display() != s2.display())
        return Change::Renderer;

    if (s1.hasPseudoStyle(PseudoId::FirstLetter) != s2.hasPseudoStyle(PseudoId::FirstLetter))
        return Change::Renderer;

    // We just detach if a renderer acquires or loses a column-span, since spanning elements
    // typically won't contain much content.
    auto columnSpanNeedsNewRenderer = [&] {
        if (!s1.columnSpanEqual(s2))
            return true;
        if (s1.columnSpan() != ColumnSpan::All)
            return false;
        // Spanning in ignored for floating and out-of-flow boxes.
        return s1.isFloating() != s2.isFloating() || s1.hasOutOfFlowPosition() != s2.hasOutOfFlowPosition();
    }();

    if (columnSpanNeedsNewRenderer)
        return Change::Renderer;

    // When text-combine property has been changed, we need to prepare a separate renderer object.
    // When text-combine is on, we use RenderCombineText, otherwise RenderText.
    // https://bugs.webkit.org/show_bug.cgi?id=55069
    if (s1.hasTextCombine() != s2.hasTextCombine())
        return Change::Renderer;

    if (!s1.contentDataEquivalent(s2))
        return Change::Renderer;

    // Query container changes affect descendant style.
    if (!s1.containerTypeAndNamesEqual(s2))
        return Change::Descendants;

    if (!s1.descendantAffectingNonInheritedPropertiesEqual(s2))
        return Change::Inherited;

    if (!s1.nonFastPathInheritedEqual(s2))
        return Change::Inherited;

    bool nonInheritedEqual = s1.nonInheritedEqual(s2);
    if (!s1.fastPathInheritedEqual(s2))
        return nonInheritedEqual ? Change::FastPathInherited : Change::NonInheritedAndFastPathInherited;

    if (!nonInheritedEqual)
        return Change::NonInherited;

    return Change::None;
}

TextStream& operator<<(TextStream& ts, Change change)
{
    switch (change) {
    case Change::None: ts << "None"; break;
    case Change::NonInherited: ts << "NonInherited"; break;
    case Change::FastPathInherited: ts << "FastPathInherited"; break;
    case Change::NonInheritedAndFastPathInherited: ts << "NonInheritedAndFastPathInherited"; break;
    case Change::Inherited: ts << "Inherited"; break;
    case Change::Descendants: ts << "Descendants"; break;
    case Change::Renderer: ts << "Renderer"; break;
    }
    return ts;
}

} // namespace Style

} // namespace WebCore
