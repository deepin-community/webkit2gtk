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

#pragma once

#include "CacheStorageStore.h"
#include "NetworkCacheKey.h"
#include <wtf/FileSystem.h>
#include <wtf/WorkQueue.h>
#include <wtf/text/WTFString.h>

namespace WebCore {
class FormData;
class SharedBuffer;
namespace DOMCacheEngine {
using ResponseBody = std::variant<std::nullptr_t, Ref<FormData>, Ref<SharedBuffer>>;
}
}

namespace WebKit {

class CacheStorageDiskStore final : public CacheStorageStore {
public:
    static Ref<CacheStorageDiskStore> create(const String& cacheName, const String& path, Ref<WorkQueue>&&);
    static size_t computeRealBodySizeForStorage(const WebCore::DOMCacheEngine::ResponseBody&);

private:
    CacheStorageDiskStore(const String& cacheName, const String& path, Ref<WorkQueue>&&);

    // CacheStorageStore
    void readAllRecordInfos(ReadAllRecordInfosCallback&&) final;
    void readRecords(const Vector<CacheStorageRecordInformation>&, ReadRecordsCallback&&) final;
    void deleteRecords(const Vector<CacheStorageRecordInformation>&, WriteRecordsCallback&&) final;
    void writeRecords(Vector<CacheStorageRecord>&&, WriteRecordsCallback&&) final;

    String versionDirectoryPath() const;
    String saltFilePath() const;
    String recordsDirectoryPath() const;
    String recordFilePath(const NetworkCache::Key&) const;
    String recordBlobFilePath(const String&) const;
    String blobsDirectoryPath() const;
    String blobFilePath(const String&) const;
    std::optional<CacheStorageRecord> readRecordFromFileData(std::span<const uint8_t>, FileSystem::MappedFileData&&);
    void readAllRecordInfosInternal(ReadAllRecordInfosCallback&&);
    void readRecordsInternal(const Vector<CacheStorageRecordInformation>&, ReadRecordsCallback&&);

    String m_cacheName;
    String m_path;
    FileSystem::Salt m_salt;
    const Ref<WorkQueue> m_callbackQueue;
    const Ref<WorkQueue> m_ioQueue;
};

}

