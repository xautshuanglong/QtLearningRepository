#ifndef DATAMAN_SDK_WRAP_H
#define DATAMAN_SDK_WRAP_H

using namespace System;
using namespace System::Xml;
using namespace System::Threading;
using namespace System::Collections::Generic;

//#using "..\\libs\\DataManSDK\\Cognex.DataMan.SDK.PC.dll"
//#using "..\\libs\\DataManSDK\\Cognex.DataMan.SDK.Utils.PC.dll"
//#using "..\\libs\\DataManSDK\\Cognex.DataMan.SDK.Discovery.PC.dll"
using namespace Cognex::DataMan::SDK;
using namespace Cognex::DataMan::SDK::Utils;
using namespace Cognex::DataMan::SDK::Discovery;

namespace CognexDataManSDK
{
    ref class DataManSdkWrap
    {
    public:
        DataManSdkWrap();
        void Discover();

    private:
        void CleanupConnection();
        void OnSystemDiscovered(SerSystemDiscoverer::SystemInfo ^systemInfo);
        void OnSystemConnected(Object ^sender, EventArgs ^args);
        void OnSystemDisconnected(Object ^sender, EventArgs ^args);
        void OnSystemWentOnline(Object ^sender, EventArgs ^args);
        void OnSystemWentOffline(Object ^sender, EventArgs ^args);
        void OnKeepAliveResponseMissed(Object ^sender, EventArgs ^args);
        void OnBinaryDataTransferProgress(Object ^sender, BinaryDataTransferProgressEventArgs ^args);
        void OffProtocolByteReceived(Object ^sender, OffProtocolByteReceivedEventArgs ^args);
        void AutomaticResponseArrived(Object ^sender, AutomaticResponseArrivedEventArgs ^args);
        void OnComplexResultCompleted(Object ^sender, ComplexResult ^result);
        void OnSimpleResultDropped(Object ^sender, SimpleResult ^result);
        String^ GetReadStringFromResultXml(String ^resultXml);

    private:
        List<SerSystemDiscoverer::SystemInfo ^>^ mListSystemInfo;
        DataManSystem^                           mDataManSystem;
        SerSystemDiscoverer^                     mSerSystemDiscover;
        ISystemConnector^                        mSystemConnector;
        ResultCollector^                         mResultCollector;
        Object^                                  mResultInfoSyncLock;
    };
}

#endif // DATAMAN_SDK_WRAP_H
