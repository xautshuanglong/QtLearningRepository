#include "stdafx.h"
#include "DataManSdkWrap.h"

//#import "..\\x64\\Debug\\CSharpComTest.tlb"
//using namespace CSharpComTest;

namespace CognexDataManSDK
{
    DataManSdkWrap::DataManSdkWrap()
    {
        mListSystemInfo = gcnew List<SerSystemDiscoverer::SystemInfo ^>();
        mSerSystemDiscover = gcnew SerSystemDiscoverer();
        mSerSystemDiscover->SystemDiscovered += gcnew SerSystemDiscoverer::SystemDiscoveredHandler(this, &DataManSdkWrap::OnSerSystemDiscovered);
    }

    void DataManSdkWrap::Discover()
    {
        mSerSystemDiscover->Discover();
        Console::WriteLine("After calling SerSystemDiscovererWrap::Discover()");

        //CoInitialize(NULL);
        //ManagedClassTest managedClass;
        //managedClass.ManagedMethodTest();
        //CoUninitialize();
    }

    void DataManSdkWrap::OnSerSystemDiscovered(SerSystemDiscoverer::SystemInfo ^systemInfo)
    {
        mListSystemInfo->Add(systemInfo);

        if (systemInfo->Name->Equals("DataMan8050"))
        {
            Console::WriteLine("Bingo: " + systemInfo->Name + "-->" + systemInfo->PortName);
        }
    }
}
