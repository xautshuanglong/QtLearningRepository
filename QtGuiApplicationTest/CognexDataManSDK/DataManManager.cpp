// This is the main DLL file.

#include "stdafx.h"

#include <iostream>
#include <vcclr.h>
#include <list>

#include "DataManManager.h"
#include "DataManSdkWrap.h"

namespace CognexDataManSDK
{
    gcroot<DataManSdkWrap ^> gpDataManSdkWrap = nullptr;

    DataManManager::DataManManager()
    {
        gpDataManSdkWrap = gcnew DataManSdkWrap();
    }

    DataManManager::~DataManManager()
    {
        delete gpDataManSdkWrap;
        gpDataManSdkWrap = nullptr;
    }

    void DataManManager::Discover()
    {
        gpDataManSdkWrap->Discover();
    }

    void DataManManager::Connect()
    {
        ;
    }

    void DataManManager::Disconnect()
    {
        ;
    }
}
