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
 *
 */

#pragma once

#include <optional>
#include <span>

namespace WebCore {

class BitReader {
public:
    explicit BitReader(std::span<const uint8_t> data)
        : m_data(data)
    {
    }

    std::optional<uint64_t> read(size_t);
    std::optional<bool> readBit();
    template <typename T> std::optional<T> read()
    {
        static_assert(std::is_unsigned<T>::value);
        auto value = readBytes(sizeof(T));
        if (!value)
            return { };
        return static_cast<T>(*value);
    }
    size_t bitOffset() const;
    bool skipBytes(size_t);
    size_t byteOffset() const { return m_index; }

private:
    std::optional<uint64_t> readBytes(size_t bytes) { return read(bytes * 8); }
    std::span<const uint8_t> m_data;
    size_t m_index { 0 };
    uint8_t m_currentByte { 0 };
    size_t m_remainingBits { 0 };
};

}
