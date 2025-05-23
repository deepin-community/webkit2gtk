/*
 * Copyright (C) 2017 Igalia S.L.
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
 * THIS SOFTWARE IS PROVIDED BY APPLE COMPUTER, INC. ``AS IS'' AND ANY
 * EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR
 * PURPOSE ARE DISCLAIMED.  IN NO EVENT SHALL APPLE COMPUTER, INC. OR
 * CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL,
 * EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO,
 * PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR
 * PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY
 * OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include "config.h"
#include "RemoteInspectorClient.h"

#if ENABLE(REMOTE_INSPECTOR)

#include "APIDebuggableInfo.h"
#include "APIInspectorConfiguration.h"
#include "RemoteInspectorHTTPServer.h"
#include "RemoteWebInspectorUIProxy.h"
#include <JavaScriptCore/RemoteInspectorUtils.h>
#include <WebCore/InspectorDebuggableType.h>
#include <gio/gio.h>
#include <wtf/NeverDestroyed.h>
#include <wtf/TZoneMallocInlines.h>
#include <wtf/glib/GUniquePtr.h>
#include <wtf/text/Base64.h>
#include <wtf/text/MakeString.h>

namespace WebKit {

class RemoteInspectorProxy final : public RemoteWebInspectorUIProxyClient {
    WTF_MAKE_TZONE_ALLOCATED_INLINE(RemoteInspectorProxy);
    WTF_OVERRIDE_DELETE_FOR_CHECKED_PTR(RemoteInspectorProxy);
public:
    RemoteInspectorProxy(RemoteInspectorClient& inspectorClient, uint64_t connectionID, uint64_t targetID)
        : m_inspectorClient(inspectorClient)
        , m_connectionID(connectionID)
        , m_targetID(targetID)
    {
    }

    ~RemoteInspectorProxy()
    {
        if (m_proxy) {
            m_proxy->setClient(nullptr);
            m_proxy->invalidate();
        } else
            RemoteInspectorHTTPServer::singleton().targetDidClose(m_connectionID, m_targetID);
    }

    void initialize(Inspector::DebuggableType debuggableType)
    {
        m_proxy = RemoteWebInspectorUIProxy::create();
        m_proxy->setClient(this);
        // FIXME <https://webkit.org/b/205536>: this should infer more useful data about the debug target.
        Ref<API::DebuggableInfo> debuggableInfo = API::DebuggableInfo::create(DebuggableInfoData::empty());
        debuggableInfo->setDebuggableType(debuggableType);
        m_proxy->initialize(WTFMove(debuggableInfo), m_inspectorClient.backendCommandsURL());
    }

    void show()
    {
        if (m_proxy)
            m_proxy->show();
    }

    void setTargetName(const CString& name)
    {
#if PLATFORM(GTK)
        if (m_proxy)
            m_proxy->updateWindowTitle(name);
#endif
    }

    // MARK: RemoteWebInspectorUIProxyClient methods

    void sendMessageToFrontend(const String& message)
    {
        if (m_proxy)
            m_proxy->sendMessageToFrontend(message);
        else
            RemoteInspectorHTTPServer::singleton().sendMessageToFrontend(m_connectionID, m_targetID, message);
    }

    void sendMessageToBackend(const String& message) override
    {
        m_inspectorClient.sendMessageToBackend(m_connectionID, m_targetID, message);
    }

    void closeFromFrontend() override
    {
        m_inspectorClient.closeFromFrontend(m_connectionID, m_targetID);
    }

    Ref<API::InspectorConfiguration> configurationForRemoteInspector(RemoteWebInspectorUIProxy&) override
    {
        return API::InspectorConfiguration::create();
    }

private:
    RefPtr<RemoteWebInspectorUIProxy> m_proxy;
    RemoteInspectorClient& m_inspectorClient;
    uint64_t m_connectionID;
    uint64_t m_targetID;
};

WTF_MAKE_TZONE_ALLOCATED_IMPL(RemoteInspectorClient);

const SocketConnection::MessageHandlers& RemoteInspectorClient::messageHandlers()
{
    static NeverDestroyed<const SocketConnection::MessageHandlers> messageHandlers = SocketConnection::MessageHandlers({
    { "DidClose", std::pair<CString, SocketConnection::MessageCallback> { { },
        [](SocketConnection&, GVariant*, gpointer userData) {
            auto& client = *static_cast<RemoteInspectorClient*>(userData);
            client.connectionDidClose();
        }}
    },
    { "DidSetupInspectorClient", std::pair<CString, SocketConnection::MessageCallback> { "(ay)",
        [](SocketConnection&, GVariant* parameters, gpointer userData) {
            auto& client = *static_cast<RemoteInspectorClient*>(userData);
            GRefPtr<GVariant> backendCommandsVariant;
            g_variant_get(parameters, "(@ay)", &backendCommandsVariant.outPtr());
            client.setBackendCommands(g_variant_get_bytestring(backendCommandsVariant.get()));
        }}
    },
    { "SetTargetList", std::pair<CString, SocketConnection::MessageCallback> { "(ta(tsssb))",
        [](SocketConnection&, GVariant* parameters, gpointer userData) {
            auto& client = *static_cast<RemoteInspectorClient*>(userData);
            guint64 connectionID;
            GUniqueOutPtr<GVariantIter> iter;
            g_variant_get(parameters, "(ta(tsssb))", &connectionID, &iter.outPtr());
            size_t targetCount = g_variant_iter_n_children(iter.get());
            Vector<RemoteInspectorClient::Target> targetList;
            targetList.reserveInitialCapacity(targetCount);
            guint64 targetID;
            const char* type;
            const char* name;
            const char* url;
            gboolean hasLocalDebugger;
            while (g_variant_iter_loop(iter.get(), "(t&s&s&sb)", &targetID, &type, &name, &url, &hasLocalDebugger)) {
                if (!g_strcmp0(type, "JavaScript") || !g_strcmp0(type, "ServiceWorker") || !g_strcmp0(type, "WebPage"))
                    targetList.append({ targetID, type, name, url });
            }
            client.setTargetList(connectionID, WTFMove(targetList));
        }}
    },
    { "SendMessageToFrontend", std::pair<CString, SocketConnection::MessageCallback> { "(tts)",
        [](SocketConnection&, GVariant* parameters, gpointer userData) {
            auto& client = *static_cast<RemoteInspectorClient*>(userData);
            guint64 connectionID, targetID;
            const char* message;
            g_variant_get(parameters, "(tt&s)", &connectionID, &targetID, &message);
            client.sendMessageToFrontend(connectionID, targetID, message);
        }}
    }
    });
    return messageHandlers;
}

RemoteInspectorClient::RemoteInspectorClient(String&& hostAndPort, RemoteInspectorObserver& observer)
    : m_hostAndPort(WTFMove(hostAndPort))
    , m_observer(observer)
    , m_cancellable(adoptGRef(g_cancellable_new()))
{
    GRefPtr<GSocketClient> socketClient = adoptGRef(g_socket_client_new());
    g_socket_client_connect_to_host_async(socketClient.get(), m_hostAndPort.utf8().data(), 0, m_cancellable.get(),
        [](GObject* object, GAsyncResult* result, gpointer userData) {
            GUniqueOutPtr<GError> error;
            GRefPtr<GSocketConnection> connection = adoptGRef(g_socket_client_connect_to_host_finish(G_SOCKET_CLIENT(object), result, &error.outPtr()));
            if (g_error_matches(error.get(), G_IO_ERROR, G_IO_ERROR_CANCELLED))
                return;
            auto* client = static_cast<RemoteInspectorClient*>(userData);
            if (connection)
                client->setupConnection(SocketConnection::create(WTFMove(connection), messageHandlers(), client));
            else {
                WTFLogAlways("RemoteInspectorClient failed to connect to inspector server: %s", error->message);
                client->m_observer.connectionClosed(*client);
            }
    }, this);
}

RemoteInspectorClient::~RemoteInspectorClient()
{
    if (m_socketConnection)
        m_socketConnection->close();
    g_cancellable_cancel(m_cancellable.get());
}

void RemoteInspectorClient::setupConnection(Ref<SocketConnection>&& connection)
{
    m_socketConnection = WTFMove(connection);
    m_socketConnection->sendMessage("SetupInspectorClient", g_variant_new("(@ay)", g_variant_new_bytestring(Inspector::backendCommandsHash().data())));
}

void RemoteInspectorClient::setBackendCommands(const char* backendCommands)
{
    if (!backendCommands || !backendCommands[0]) {
        m_backendCommandsURL = { };
        return;
    }

    m_backendCommandsURL = makeString("data:text/javascript;base64,"_s, base64Encoded(unsafeSpan8(backendCommands)));
}

void RemoteInspectorClient::connectionDidClose()
{
    m_targets.clear();
    m_inspectorProxyMap.clear();
    m_socketConnection = nullptr;
    m_observer.connectionClosed(*this);
}

static Inspector::DebuggableType debuggableType(const String& targetType)
{
    if (targetType == "JavaScript"_s)
        return Inspector::DebuggableType::JavaScript;
    if (targetType == "ServiceWorker"_s)
        return Inspector::DebuggableType::ServiceWorker;
    if (targetType == "WebPage"_s)
        return Inspector::DebuggableType::WebPage;
    RELEASE_ASSERT_NOT_REACHED();
}

void RemoteInspectorClient::inspect(uint64_t connectionID, uint64_t targetID, const String& targetType, InspectorType inspectorType)
{
    auto addResult = m_inspectorProxyMap.ensure(std::make_pair(connectionID, targetID), [this, connectionID, targetID] {
        return makeUnique<RemoteInspectorProxy>(*this, connectionID, targetID);
    });
    if (!addResult.isNewEntry) {
        addResult.iterator->value->show();
        return;
    }

    m_socketConnection->sendMessage("Setup", g_variant_new("(tt)", connectionID, targetID));
    if (inspectorType == InspectorType::UI)
        addResult.iterator->value->initialize(debuggableType(targetType));
}

void RemoteInspectorClient::sendMessageToBackend(uint64_t connectionID, uint64_t targetID, const String& message)
{
    m_socketConnection->sendMessage("SendMessageToBackend", g_variant_new("(tts)", connectionID, targetID, message.utf8().data()));
}

void RemoteInspectorClient::closeFromFrontend(uint64_t connectionID, uint64_t targetID)
{
    ASSERT(m_inspectorProxyMap.contains(std::make_pair(connectionID, targetID)));
    m_socketConnection->sendMessage("FrontendDidClose", g_variant_new("(tt)", connectionID, targetID));
    m_inspectorProxyMap.remove(std::make_pair(connectionID, targetID));
}

void RemoteInspectorClient::setTargetList(uint64_t connectionID, Vector<Target>&& targetList)
{
    // Find closed targets to remove them.
    Vector<uint64_t, 4> targetsToRemove;
    for (auto& connectionTargetPair : m_inspectorProxyMap.keys()) {
        if (connectionTargetPair.first != connectionID)
            continue;
        bool found = false;
        for (const auto& target : targetList) {
            if (target.id == connectionTargetPair.second) {
                m_inspectorProxyMap.get(connectionTargetPair)->setTargetName(target.name);
                found = true;
                break;
            }
        }
        if (!found)
            targetsToRemove.append(connectionTargetPair.second);
    }
    for (auto targetID : targetsToRemove)
        m_inspectorProxyMap.remove(std::make_pair(connectionID, targetID));

    m_targets.set(connectionID, WTFMove(targetList));
    m_observer.targetListChanged(*this);
}

void RemoteInspectorClient::sendMessageToFrontend(uint64_t connectionID, uint64_t targetID, const char* message)
{
    auto proxy = m_inspectorProxyMap.get(std::make_pair(connectionID, targetID));
    ASSERT(proxy);
    if (!proxy)
        return;
    proxy->sendMessageToFrontend(String::fromUTF8(message));
}

WTF_ALLOW_UNSAFE_BUFFER_USAGE_BEGIN // GTK/WPE port

void RemoteInspectorClient::appendTargertList(GString* html, InspectorType inspectorType, ShouldEscapeSingleQuote escapeSingleQuote) const
{
    if (m_targets.isEmpty())
        g_string_append(html, "<p>No targets found</p>");
    else {
        g_string_append(html, "<table>");
        for (auto connectionID : m_targets.keys()) {
            for (auto& target : m_targets.get(connectionID)) {
                g_string_append_printf(html,
                    "<tbody><tr>"
                    "<td class=\"data\"><div class=\"targetname\">%s</div><div class=\"targeturl\">%s</div></td>"
                    "<td class=\"input\"><input type=\"button\" value=\"Inspect\" onclick=",
                    target.name.data(), target.url.data());

                switch (inspectorType) {
                case InspectorType::UI:
                    g_string_append(html, "\"window.webkit.messageHandlers.inspector.postMessage(");
                    if (escapeSingleQuote == ShouldEscapeSingleQuote::Yes)
                        g_string_append(html, "\\'");
                    else
                        g_string_append_c(html, '\'');
                    g_string_append_printf(html, "%" G_GUINT64_FORMAT ":%" G_GUINT64_FORMAT ":%s", connectionID, target.id, target.type.data());
                    if (escapeSingleQuote == ShouldEscapeSingleQuote::Yes)
                        g_string_append(html, "\\')\"");
                    else
                        g_string_append(html, "')\"");
                    break;
                case InspectorType::HTTP:
                    g_string_append_printf(html,
                        "\"window.open('Main.html?ws=' + window.location.host + '/socket/%" G_GUINT64_FORMAT "/%" G_GUINT64_FORMAT "/%s', "
                        "'_blank', 'location=no,menubar=no,status=no,toolbar=no');\"",
                        connectionID, target.id, target.type.data());
                    break;
                }

                g_string_append(html, "></td></tr></tbody>");
            }
        }
        g_string_append(html, "</table>");
    }
}

GString* RemoteInspectorClient::buildTargetListPage(InspectorType inspectorType) const
{
    GString* html = g_string_new(
        "<html><head><title>Remote inspector</title>"
        "<meta http-equiv=\"Content-Type\" content=\"text/html; charset=utf-8\" />"
        "<style>"
        "  h1 { color: #babdb6; text-shadow: 0 1px 0 white; margin-bottom: 0; }"
        "  html { font-family: -webkit-system-font; font-size: 11pt; color: #2e3436; padding: 20px 20px 0 20px; background-color: #f6f6f4; "
        "         background-image: -webkit-gradient(linear, left top, left bottom, color-stop(0, #eeeeec), color-stop(1, #f6f6f4));"
        "         background-size: 100% 5em; background-repeat: no-repeat; }"
        "  table { width: 100%; border-collapse: collapse; }"
        "  table, td { border: 1px solid #d3d7cf; border-left: none; border-right: none; }"
        "  p { margin-bottom: 30px; }"
        "  td { padding: 15px; }"
        "  td.data { width: 200px; }"
        "  .targetname { font-weight: bold; }"
        "  .targeturl { color: #babdb6; }"
        "  td.input { width: 64px; }"
        "  input { width: 100%; padding: 8px; }"
        "</style>"
        "</head><body><h1>Inspectable targets</h1>"
        "<div id='targetlist'>");
    appendTargertList(html, inspectorType, ShouldEscapeSingleQuote::No);
    g_string_append(html, "</div></body></html>");

    return html;
}

WTF_ALLOW_UNSAFE_BUFFER_USAGE_END

} // namespace WebKit

#endif // ENABLE(REMOTE_INSPECTOR)
