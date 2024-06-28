#pragma once

#include <cstdint>

// TODO For single producer, single consumer case, probably only need to lock when updating the index variables.
//  Concurrent access ot the array is restricted by those variables anyway
template<typename T>
class RingBuffer
{
public:
    constexpr RingBuffer() = delete;
    constexpr RingBuffer(T *Buffer, uint32_t BufferSize) :
	Buffer(Buffer), BufferSize(BufferSize), inIdx(0), outIdx(0), inSize(0)
	{};
    ~RingBuffer() = default;

    int8_t Put(const T & Input) const;
    int8_t Putn(const T *Input, uint32_t NumElements) const;

    int8_t Get(T *Output) const;
    int8_t Getn(T *Output, uint32_t NumElements, uint32_t *ElementsRead) const;

    __attribute__((always_inline)) inline uint32_t NofElements() const
    {
        return inSize;
    }

    __attribute__((always_inline)) inline uint32_t NofFreeElements() const
    {
        return (uint32_t)(BufferSize - inSize);
    }
    void Clear() const;
    bool Available() const
    {
        return (bool)NofElements();
    }

private:
    mutable T *Buffer;
	const uint32_t BufferSize;
    mutable uint32_t inIdx;  /* input index */
    mutable uint32_t outIdx; /* output index */
    mutable uint32_t inSize; /* size data in buffer */

    int8_t PutUnsafe(const T & Input) const;
    int8_t GetUnsafe(T *Output) const;
};
