/*
 * Copyright (C) 2022 Apple Inc. All rights reserved.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE INC. AND ITS CONTRIBUTORS ``AS IS''
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO,
 * THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL APPLE INC. OR ITS CONTRIBUTORS
 * BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 * CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 * SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 * INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 * CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 * ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF
 * THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "DocumentFullscreen.h"

#if ENABLE(FULLSCREEN_API)
#include "DocumentInlines.h"
#include "Document.h"
#include "Element.h"
#include "EventLoop.h"
#include "FullscreenManager.h"
#include "JSDOMPromiseDeferred.h"

namespace WebCore {

bool DocumentFullscreen::webkitFullscreenEnabled(Document& document)
{
    return document.fullscreenManager().isFullscreenEnabled();
}

Element* DocumentFullscreen::webkitFullscreenElement(Document& document)
{
    return document.ancestorElementInThisScope(document.fullscreenManager().protectedFullscreenElement().get());
}

bool DocumentFullscreen::webkitIsFullScreen(Document& document)
{
    return document.fullscreenManager().isFullscreen();
}

bool DocumentFullscreen::webkitFullScreenKeyboardInputAllowed(Document& document)
{
    return document.fullscreenManager().isFullscreenKeyboardInputAllowed();
}

Element* DocumentFullscreen::webkitCurrentFullScreenElement(Document& document)
{
    return document.ancestorElementInThisScope(document.fullscreenManager().protectedFullscreenElement().get());
}

void DocumentFullscreen::webkitCancelFullScreen(Document& document)
{
    document.fullscreenManager().fullyExitFullscreen();
}

// https://fullscreen.spec.whatwg.org/#exit-fullscreen
void DocumentFullscreen::exitFullscreen(Document& document, RefPtr<DeferredPromise>&& promise)
{
    if (!document.isFullyActive() || !document.fullscreenManager().fullscreenElement()) {
        promise->reject(Exception { ExceptionCode::TypeError, "Not in fullscreen"_s });
        return;
    }
    document.checkedFullscreenManager()->exitFullscreen([promise = WTFMove(promise)] (auto result) {
        if (!promise)
            return;
        if (result.hasException())
            promise->reject(result.releaseException());
        else
            promise->resolve();
    });
}

void DocumentFullscreen::webkitExitFullscreen(Document& document)
{
    if (document.fullscreenManager().fullscreenElement())
        document.checkedFullscreenManager()->exitFullscreen([] (auto) { });
}

// https://fullscreen.spec.whatwg.org/#dom-document-fullscreenenabled
bool DocumentFullscreen::fullscreenEnabled(Document& document)
{
    if (!document.isFullyActive())
        return false;
    return document.checkedFullscreenManager()->isFullscreenEnabled();
}

} // namespace WebCore

#endif // ENABLE(FULLSCREEN_API)
