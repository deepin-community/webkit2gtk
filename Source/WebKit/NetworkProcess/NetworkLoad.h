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

#pragma once

#include "DownloadID.h"
#include "NetworkDataTask.h"
#include "NetworkLoadParameters.h"
#include "NetworkSession.h"
#include <wtf/CheckedRef.h>
#include <wtf/TZoneMalloc.h>
#include <wtf/text/WTFString.h>

namespace WebCore {
class AuthenticationChallenge;
}

namespace WebKit {

class NetworkLoadClient;
class NetworkLoadScheduler;
class NetworkProcess;

class NetworkLoad final : public RefCounted<NetworkLoad>, public NetworkDataTaskClient {
    WTF_MAKE_TZONE_ALLOCATED(NetworkLoad);
public:
    void ref() const final { RefCounted::ref(); }
    void deref() const final { RefCounted::deref(); }

    static Ref<NetworkLoad> create(NetworkLoadClient& networkLoadClient, NetworkLoadParameters&& networkLoadParameters, NetworkSession& networkSession)
    {
        return adoptRef(*new NetworkLoad(networkLoadClient, WTFMove(networkLoadParameters), networkSession));
    }

    template<typename CreateTaskCallback> static Ref<NetworkLoad> create(NetworkLoadClient& networkLoadClient, NetworkSession& networkSession, NOESCAPE const CreateTaskCallback& createTask)
    {
        return adoptRef(*new NetworkLoad(networkLoadClient, networkSession, createTask));
    }

    ~NetworkLoad();

    void start();
    void startWithScheduling();
    void cancel();

    bool isAllowedToAskUserForCredentials() const;

    const WebCore::ResourceRequest& currentRequest() const { return m_currentRequest; }
    void updateRequestAfterRedirection(WebCore::ResourceRequest&) const;
    void reprioritizeRequest(WebCore::ResourceLoadPriority);

    const NetworkLoadParameters& parameters() const { return m_parameters; }
    const URL& url() const { return parameters().request.url(); }
    String attributedBundleIdentifier(WebPageProxyIdentifier);

    void convertTaskToDownload(PendingDownload&, const WebCore::ResourceRequest&, const WebCore::ResourceResponse&, ResponseCompletionHandler&&);
    void setPendingDownloadID(DownloadID);
    void setSuggestedFilename(const String&);
    void setPendingDownload(PendingDownload&);
    std::optional<DownloadID> pendingDownloadID() { return protectedTask()->pendingDownloadID(); }

    bool shouldCaptureExtraNetworkLoadMetrics() const final;

    String description() const;
    void setH2PingCallback(const URL&, CompletionHandler<void(Expected<WTF::Seconds, WebCore::ResourceError>&&)>&&);

    void setTimingAllowFailedFlag();
    std::optional<WebCore::FrameIdentifier> webFrameID() const;
    std::optional<WebCore::PageIdentifier> webPageID() const;
    Ref<NetworkProcess> networkProcess();

    size_t bytesTransferredOverNetwork() const;

private:
    NetworkLoad(NetworkLoadClient&, NetworkLoadParameters&&, NetworkSession&);

    template<typename CreateTaskCallback> NetworkLoad(NetworkLoadClient& client, NetworkSession& networkSession, NOESCAPE const CreateTaskCallback& createTask)
        : m_client(client)
        , m_networkProcess(networkSession.networkProcess())
        , m_task(createTask(*this))
    {
    }

    // NetworkDataTaskClient
    void willPerformHTTPRedirection(WebCore::ResourceResponse&&, WebCore::ResourceRequest&&, RedirectCompletionHandler&&) final;
    void didReceiveChallenge(WebCore::AuthenticationChallenge&&, NegotiatedLegacyTLS, ChallengeCompletionHandler&&) final;
    void didReceiveInformationalResponse(WebCore::ResourceResponse&&) final;
    void didReceiveResponse(WebCore::ResourceResponse&&, NegotiatedLegacyTLS, PrivateRelayed, ResponseCompletionHandler&&) final;
    void didReceiveData(const WebCore::SharedBuffer&) final;
    void didCompleteWithError(const WebCore::ResourceError&, const WebCore::NetworkLoadMetrics&) final;
    void didSendData(uint64_t totalBytesSent, uint64_t totalBytesExpectedToSend) final;
    void wasBlocked() final;
    void cannotShowURL() final;
    void wasBlockedByRestrictions() final;
    void wasBlockedByDisabledFTP() final;
    void didNegotiateModernTLS(const URL&) final;

    RefPtr<NetworkDataTask> protectedTask();

    void notifyDidReceiveResponse(WebCore::ResourceResponse&&, NegotiatedLegacyTLS, PrivateRelayed, ResponseCompletionHandler&&);

    CheckedRef<NetworkLoadClient> m_client;
    const Ref<NetworkProcess> m_networkProcess;
    const NetworkLoadParameters m_parameters;
    RefPtr<NetworkDataTask> m_task;
    WeakPtr<NetworkLoadScheduler> m_scheduler;

    // FIXME: Deduplicate this with NetworkDataTask's m_previousRequest.
    WebCore::ResourceRequest m_currentRequest; // Updated on redirects.
};

} // namespace WebKit
