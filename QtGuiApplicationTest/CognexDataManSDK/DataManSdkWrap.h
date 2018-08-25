#ifndef DATAMAN_SDK_WRAP_H
#define DATAMAN_SDK_WRAP_H

using namespace System;
using namespace System::Collections::Generic;

#using "..\\libs\\DataManSDK\\Cognex.DataMan.SDK.PC.dll"
#using "..\\libs\\DataManSDK\\Cognex.DataMan.SDK.Discovery.PC.dll"
using namespace Cognex::DataMan::SDK;
using namespace Cognex::DataMan::SDK::Discovery;

namespace CognexDataManSDK
{
    ref class DataManSdkWrap
    {
    public:
        DataManSdkWrap();
        void Discover();

    private:
        void OnSerSystemDiscovered(SerSystemDiscoverer::SystemInfo ^systemInfo);

    private:
        List<SerSystemDiscoverer::SystemInfo ^>^ mListSystemInfo;
        DataManSystem^                           mDataManSystem;
        SerSystemDiscoverer^                     mSerSystemDiscover;
        ISystemConnector^                        mSystemConnector;
    };
}

#endif // DATAMAN_SDK_WRAP_H
