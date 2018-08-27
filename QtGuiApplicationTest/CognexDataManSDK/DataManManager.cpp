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
        gpDataManSdkWrap->SetEventListener(this);
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

    void DataManManager::AddEventListener(IDataManCallback *pEventListener)
    {
        ;
    }

    /* IDeviceConnectionCallback */
    void DataManManager::OnDeviceDiscoverd()
    {
        std::cout << "EnterFunction: " << __FUNCSIG__ << std::endl;
    }

    void DataManManager::OnDeviceConnected()
    {
        std::cout << "EnterFunction: " << __FUNCSIG__ << std::endl;
    }

    void DataManManager::OnDeviceDisconnected()
    {
        std::cout << "EnterFunction: " << __FUNCSIG__ << std::endl;
    }

    /* IDataManCallback */
    void DataManManager::OnSimpleResultCompleted(int id, unsigned char* pData, int dataLen)
    {
        std::cout << "EnterFunction: " << __FUNCSIG__ << std::endl;
    }

    void DataManManager::OnComplexResultCompleted()
    {
        std::cout << "EnterFunction: " << __FUNCSIG__ << std::endl;
    }
}
