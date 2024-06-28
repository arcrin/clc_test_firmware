#include "RingBuffer.h"
#include <cstring>
#include "CriticalSection.h"

#pragma GCC push_options
#pragma GCC optimize ("-O3")

#define max(a, b) (a > b ? a : b)
#define min(a, b) (a < b ? a : b)

template<typename T>
int8_t RingBuffer<T>::PutUnsafe(const T & Input) const
{
    if (inSize >= BufferSize) {
        return -1;
    }

    Buffer[inIdx] = Input;
    inIdx++;
    if (inIdx == BufferSize)
    {
        inIdx = 0;
    }
    inSize++;
    return 0;
}

template<typename T>
int8_t RingBuffer<T>::Put(const T & Input) const
{
    CriticalSection s;
    return PutUnsafe(Input);
}

template<typename T>
int8_t RingBuffer<T>::Putn(const T * const Input, uint32_t NumElements) const
{
    CriticalSection s;
    const T * Ptr = Input;

    //                                                                       space at end
    uint32_t TransferSize = min(NofFreeElements(), min(NumElements, (BufferSize - inIdx)));
    memcpy(&Buffer[inIdx], Ptr, TransferSize * sizeof(T));
    inSize += TransferSize;
    inIdx += TransferSize;
    if (inIdx == BufferSize) {
        inIdx = 0;
    }
    NumElements -= TransferSize;
    Ptr += TransferSize;

    if (NumElements) {
        // move to front
        TransferSize = min(NofFreeElements(), NumElements);
        memcpy(Buffer, Ptr, TransferSize * sizeof(T));
        inSize += TransferSize;
        inIdx += TransferSize;
        Ptr += TransferSize;
        // no way this could happen. Would have to write more than BufferSize
//        if (inIdx == BufferSize) {
//            inIdx = 0;
//        }
        NumElements -= TransferSize;
    }
    if (NumElements) {
        return -1;
    }
    return 0;
}

template<typename T>
int8_t RingBuffer<T>::GetUnsafe(T * const Output) const
{
    if (inSize > 0) {
        *Output = Buffer[outIdx];
        inSize--;
        outIdx++;
        if (outIdx == BufferSize) {
            outIdx = 0;
        }
    } else {
        return -1;
    }
    return 0;
}

template<typename T>
int8_t RingBuffer<T>::Get(T *Output) const
{
    CriticalSection s;
    return GetUnsafe(Output);
}

template<typename T>
int8_t RingBuffer<T>::Getn(T * const Output, uint32_t NumElements, uint32_t * const ElementsRead) const
{
    CriticalSection s;
    T * Ptr = Output;

    //                                                                       elements at end
    uint32_t TransferSize = min(NofElements(), min(NumElements, (BufferSize - outIdx)));
    memcpy(Ptr, &Buffer[outIdx], TransferSize * sizeof(T));
    inSize -= TransferSize;
    outIdx += TransferSize;
    if (outIdx == BufferSize) {
        outIdx = 0;
    }
    NumElements -= TransferSize;
    Ptr += TransferSize;

    if (NumElements) {
        // move to front
        TransferSize = min(NofElements(), NumElements);
        memcpy(Ptr, Buffer, TransferSize * sizeof(T));
        inSize -= TransferSize;
        outIdx += TransferSize;
        Ptr += TransferSize;
        // no way this could happen. Would have to write more than BufferSize
//        if (outIdx == BufferSize) {
//            outIdx = 0;
//        }
        NumElements -= TransferSize;
    }
    *ElementsRead = Ptr - Output;
    if (NumElements) {
        return -1;
    }
    return 0;
}

template<typename T>
void RingBuffer<T>::Clear() const
{
    CriticalSection s;
    inIdx = 0;
    outIdx = 0;
    inSize = 0;
}

template class RingBuffer<uint8_t>;

#pragma GCC pop_options
