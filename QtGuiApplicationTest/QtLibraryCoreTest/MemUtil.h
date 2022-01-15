#pragma once

namespace SL
{
    namespace Util
    {
        class MemUtil
        {
        public:
            MemUtil() = delete;
            ~MemUtil();

            template<typename T>
            static void AquirePointer(T** pOutPointer);

            template<typename T>
            static void RelesePointer(T** pInPointer);
        };
    }
}

namespace SL::Util
{
    template<typename T>
    inline void MemUtil::AquirePointer(T ** pOutPointer)
    {
        if (pOutPointer != nullptr)
        {
            *pOutPointer = new T();
        }
    }

    template<typename T>
    inline void MemUtil::RelesePointer(T ** pOutPointer)
    {
        if (pOutPointer != nullptr)
        {
            T* pTempPointer = *pOutPointer;
            if (pTempPointer != nullptr)
            {
                delete pTempPointer;
            }
            *pOutPointer = nullptr;
        }
    }
}
