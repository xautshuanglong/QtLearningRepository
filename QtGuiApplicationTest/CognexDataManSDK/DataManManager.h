// CognexDataManSDK.h

#pragma once

#include "DataManSdkDefines.h"

namespace CognexDataManSDK
{
    class COGNEX_EXPORT DataManManager
    {
    public:
        DataManManager();
        ~DataManManager();

        void Discover();
        void Connect();
        void Disconnect();
    };
}
