/*
 * Copyright (c) 2021 Apple Inc. All rights reserved.
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

#ifndef PAS_DEBUG_HEAP_H
#define PAS_DEBUG_HEAP_H

#include "pas_utils.h"

PAS_BEGIN_EXTERN_C;

/* Bmalloc has a DebugHeap singleton that can be used to divert bmalloc calls to system malloc.
   When libpas is used in bmalloc, we use this to glue libpas into that mechanism. */

#if PAS_BMALLOC

/* The implementations are provided by bmalloc. */
PAS_API extern bool pas_debug_heap_is_enabled(void);
PAS_API extern void* pas_debug_heap_malloc(size_t size);
PAS_API extern void* pas_debug_heap_memalign(size_t alignment, size_t size);
PAS_API extern void* pas_debug_heap_realloc(void* ptr, size_t size);
PAS_API extern void pas_debug_heap_free(void* ptr);

#else /* PAS_BMALLOC -> so !PAS_BMALLOC */

static inline bool pas_debug_heap_is_enabled(void)
{
    return false;
}

static inline void* pas_debug_heap_malloc(size_t size)
{
    PAS_UNUSED_PARAM(size);
    PAS_ASSERT(!"Should not be reached");
    return NULL;
}

static inline void* pas_debug_heap_memalign(size_t alignment, size_t size)
{
    PAS_UNUSED_PARAM(alignment);
    PAS_UNUSED_PARAM(size);
    PAS_ASSERT(!"Should not be reached");
    return NULL;
}

static inline void* pas_debug_heap_realloc(void* ptr, size_t size)
{
    PAS_UNUSED_PARAM(ptr);
    PAS_UNUSED_PARAM(size);
    PAS_ASSERT(!"Should not be reached");
    return NULL;
}

static inline void pas_debug_heap_free(void* ptr)
{
    PAS_UNUSED_PARAM(ptr);
    PAS_ASSERT(!"Should not be reached");
}

#endif /* PAS_BMALLOC -> so end of !PAS_BMALLOC */

PAS_END_EXTERN_C;

#endif /* PAS_DEBUG_HEAP_H */
