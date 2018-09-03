#include "stdafx.h"
#include "DataManSdkWrap.h"

#include <vcclr.h>
#include <cstdlib>

namespace CognexDataManSDK
{
    DataManSdkWrap::DataManSdkWrap()
        : mListSystemInfo(nullptr)
        , mDataManSystem(nullptr)
        , mSerSystemDiscover(nullptr)
        , mSystemConnector(nullptr)
        , mResultCollector(nullptr)
        , mpEventListener(nullptr)
    {
        mListSystemInfo = gcnew List<SerSystemDiscoverer::SystemInfo ^>();
        mSerSystemDiscover = gcnew SerSystemDiscoverer();
        mSerSystemDiscover->SystemDiscovered += gcnew SerSystemDiscoverer::SystemDiscoveredHandler(this, &DataManSdkWrap::OnSystemDiscovered);
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

    void DataManSdkWrap::Connect(String^ portName, int baudRate)
    {
        try
        {
            mSystemConnector = gcnew SerSystemConnector(portName, baudRate);
            mDataManSystem = gcnew DataManSystem(mSystemConnector);

            mDataManSystem->DefaultTimeout = 5000;

            // Subscribe to events that are signalled when the system is connected / disconnected.
            mDataManSystem->SystemConnected += gcnew SystemConnectedHandler(this, &DataManSdkWrap::OnSystemConnected);
            mDataManSystem->SystemDisconnected += gcnew SystemDisconnectedHandler(this, &DataManSdkWrap::OnSystemDisconnected);
            mDataManSystem->SystemWentOnline += gcnew SystemWentOnlineHandler(this, &DataManSdkWrap::OnSystemWentOnline);
            mDataManSystem->SystemWentOffline += gcnew SystemWentOfflineHandler(this, &DataManSdkWrap::OnSystemWentOffline);
            mDataManSystem->KeepAliveResponseMissed += gcnew KeepAliveResponseMissedHandler(this, &DataManSdkWrap::OnKeepAliveResponseMissed);
            mDataManSystem->BinaryDataTransferProgress += gcnew BinaryDataTransferProgressHandler(this, &DataManSdkWrap::OnBinaryDataTransferProgress);
            mDataManSystem->OffProtocolByteReceived += gcnew OffProtocolByteReceivedHandler(this, &DataManSdkWrap::OffProtocolByteReceived);
            mDataManSystem->AutomaticResponseArrived += gcnew AutomaticResponseArrivedHandler(this, &DataManSdkWrap::AutomaticResponseArrived);

            // Subscribe to events that are signalled when the device sends auto-responses.
            ResultTypes requestedResultTypes = ResultTypes::ReadXml | ResultTypes::Image | ResultTypes::ImageGraphics;
            mResultCollector = gcnew ResultCollector(mDataManSystem, requestedResultTypes);
            mResultCollector->ComplexResultCompleted += gcnew ComplexResultCompletedEventHandler(this, &DataManSdkWrap::OnComplexResultCompleted);
            mResultCollector->SimpleResultDropped += gcnew SimpleResultDroppedEventHandler(this, &DataManSdkWrap::OnSimpleResultDropped);

            mDataManSystem->SetKeepAliveOptions(true, 3000, 1000);
            //mDataManSystem->SetKeepAliveOptions(false, 3000, 1000);
            mDataManSystem->SetResultTypes(requestedResultTypes);
            mDataManSystem->Connect();
        }
        catch (Exception ^ex)
        {
            Console::WriteLine("Failed to connect: " + ex->ToString());
            CleanupConnection();
        }
    }

    void DataManSdkWrap::Disconnect()
    {
        if (mDataManSystem == nullptr || mDataManSystem->State != ConnectionState::Connected)
        {
            return;
        }

        mDataManSystem->Disconnect();
        CleanupConnection();
    }

    void DataManSdkWrap::SetEventListener(IDataManCallback *pEventListener)
    {
        mpEventListener = pEventListener;
    }

    void DataManSdkWrap::CleanupConnection()
    {
        if (nullptr != mDataManSystem)
        {
            mDataManSystem->SystemConnected -= gcnew SystemConnectedHandler(this, &DataManSdkWrap::OnSystemConnected);
            mDataManSystem->SystemDisconnected -= gcnew SystemDisconnectedHandler(this, &DataManSdkWrap::OnSystemDisconnected);
            mDataManSystem->SystemWentOnline -= gcnew SystemWentOnlineHandler(this, &DataManSdkWrap::OnSystemWentOnline);
            mDataManSystem->SystemWentOffline -= gcnew SystemWentOfflineHandler(this, &DataManSdkWrap::OnSystemWentOffline);
            mDataManSystem->KeepAliveResponseMissed -= gcnew KeepAliveResponseMissedHandler(this, &DataManSdkWrap::OnKeepAliveResponseMissed);
            mDataManSystem->BinaryDataTransferProgress -= gcnew BinaryDataTransferProgressHandler(this, &DataManSdkWrap::OnBinaryDataTransferProgress);
            mDataManSystem->OffProtocolByteReceived -= gcnew OffProtocolByteReceivedHandler(this, &DataManSdkWrap::OffProtocolByteReceived);
            mDataManSystem->AutomaticResponseArrived -= gcnew AutomaticResponseArrivedHandler(this, &DataManSdkWrap::AutomaticResponseArrived);
        }

        mSystemConnector = nullptr;
        mDataManSystem = nullptr;
    }

    void DataManSdkWrap::OnSystemDiscovered(SerSystemDiscoverer::SystemInfo ^systemInfo)
    {
        mListSystemInfo->Add(systemInfo);
        Console::WriteLine("EnterFunction: DataManSdkWrap::OnSystemDiscovered() "
                           + systemInfo->Name + " "
                           + systemInfo->PortName + " "
                           + systemInfo->Baudrate + " "
                           + systemInfo->DeviceTypeId + " "
                           + systemInfo->SerialNumber + " "
                           + systemInfo->Type);

        if (systemInfo->Name->Contains("DM8050"))
        {
            Console::WriteLine("Bingo: " + systemInfo->Name + "-->" + systemInfo->PortName);
            Connect(systemInfo->PortName, systemInfo->Baudrate);
        }
        if (mpEventListener != nullptr)
        {
            mpEventListener->OnDeviceDiscoverd();
        }
    }

    void DataManSdkWrap::OnSystemConnected(Object ^sender, EventArgs ^args)
    {
        Console::WriteLine("EnterFunction: " + "DataManSdkWrap::OnSystemConnected()");
        if (mpEventListener != nullptr)
        {
            mpEventListener->OnDeviceConnected();
        }
    }

    void DataManSdkWrap::OnSystemDisconnected(Object ^sender, EventArgs ^args)
    {
        Console::WriteLine("EnterFunction: " + "DataManSdkWrap::OnSystemDisconnected()");
    }

    void DataManSdkWrap::OnSystemWentOnline(Object ^sender, EventArgs ^args)
    {
        Console::WriteLine("EnterFunction: " + "DataManSdkWrap::OnSystemWentOnline()");
    }

    void DataManSdkWrap::OnSystemWentOffline(Object ^sender, EventArgs ^args)
    {
        Console::WriteLine("EnterFunction: " + "DataManSdkWrap::OnSystemWentOffline()");
    }

    void DataManSdkWrap::OnKeepAliveResponseMissed(Object ^sender, EventArgs ^args)
    {
        Console::WriteLine("EnterFunction: " + "DataManSdkWrap::v()");
    }

    void DataManSdkWrap::OnBinaryDataTransferProgress(Object ^sender, BinaryDataTransferProgressEventArgs ^args)
    {
        //Console::WriteLine("EnterFunction: " + "DataManSdkWrap::OnBinaryDataTransferProgress()" + args->ResponseId);
        String ^progressMsg = String::Format("ResID:{0} Bytes:{1}/{2}", args->ResponseId, args->BytesTransferred, args->TotalDataSize);
        Console::WriteLine(progressMsg);
    }

    void DataManSdkWrap::OffProtocolByteReceived(Object ^sender, OffProtocolByteReceivedEventArgs ^args)
    {
        Console::WriteLine("EnterFunction: " + "DataManSdkWrap::OffProtocolByteReceived()");
    }

    void DataManSdkWrap::AutomaticResponseArrived(Object ^sender, AutomaticResponseArrivedEventArgs ^args)
    {
        Console::WriteLine("EnterFunction: " + "DataManSdkWrap::AutomaticResponseArrived()");
    }

    void DataManSdkWrap::OnComplexResultCompleted(Object ^sender, ComplexResult ^result)
    {
        //Console::WriteLine("EnterFunction: " + "DataManSdkWrap::OnComplexResultCompleted()");

        ResultTypes collected_results = ResultTypes::None;
        SimpleResult^ tempResult = nullptr;
        String^ resultStr("");
        int resCount = result->SimpleResults->Count;
        for (int i = 0; i < resCount; ++i)
        {
            tempResult = result->SimpleResults[i];
          //  collected_results |= (ResultTypes)(tempResult->Id->Type);

            switch (tempResult->Id->Type)
            {
            case ResultTypes::Image:
                break;
            case ResultTypes::ImageGraphics:
                break;
            case ResultTypes::ReadXml:
                resultStr = this->GetReadStringFromResultXml(tempResult->GetDataAsString());
                break;
            case ResultTypes::ReadString:
                resultStr = tempResult->GetDataAsString();
                break;
            }
        }

        String ^tempStr = String::Format("Partial result dropped: {0}, id={1}, TEXT:{2}",
                                           tempResult->Id->Type.ToString(),
                                           tempResult->Id->Id,
                                           resultStr);
        Console::WriteLine(tempStr);
        if (mpEventListener != nullptr)
        {
            char* pTest = nullptr;
            size_t converted = 0;
            size_t resultLen = resultStr->Length;
            pin_ptr<const wchar_t> resultBytes = ::PtrToStringChars(resultStr);
            pTest = new char[resultLen + 1];
            pTest[resultLen] = '\0';
            //std::wcstombs(pTest, resultBytes, resultLen);
            ::wcstombs_s(&converted, pTest, resultLen+1, resultBytes, resultLen);
            mpEventListener->OnSimpleResultCompleted(tempResult->Id->Id, pTest, resultStr->Length);
            delete pTest;
        }

        //static int count = 0;
        //try
        //{
        //    if ((count & 1) == 0)
        //    {
        //        Console::WriteLine("Command: SET LIGHT.INTERNAL-ENABLE ON");
        //        mDataManSystem->SendCommand("SET LIGHT.INTERNAL-ENABLE ON");
        //    }
        //    else
        //    {
        //        Console::WriteLine("Command: SET LIGHT.INTERNAL-ENABLE OFF");
        //        mDataManSystem->SendCommand("SET LIGHT.INTERNAL-ENABLE OFF");
        //    }
        //}
        //catch (Exception^ e)
        //{
        //    Console::WriteLine("Error: " + e->Message + " at " + e->StackTrace);
        //}
        //++count;
    }

    void DataManSdkWrap::OnSimpleResultDropped(Object ^sender, SimpleResult ^result)
    {
        //Console::WriteLine("EnterFunction: " + "DataManSdkWrap::OnSimpleResultDropped()");
        String ^resultStr = String::Format("Partial result dropped: {0}, id={1}", result->Id->Type.ToString(), result->Id->Id);
        Console::WriteLine(resultStr);
    }

    String^ DataManSdkWrap::GetReadStringFromResultXml(String^ resultXml)
    {
        try
        {
            XmlDocument^ doc = gcnew XmlDocument();

            doc->LoadXml(resultXml);

            XmlNode^ full_string_node = doc->SelectSingleNode("result/general/full_string");

            if (full_string_node != nullptr && mDataManSystem != nullptr&& mDataManSystem->State == ConnectionState::Connected)
            {
                XmlAttribute^ encoding = full_string_node->Attributes["encoding"];
                if (encoding != nullptr && encoding->Value == "base64")
                {
                    if (!String::IsNullOrEmpty(full_string_node->InnerText))
                    {
                        array<Byte>^ resultBytes = Convert::FromBase64String(full_string_node->InnerText);
                        return mDataManSystem->Encoding->GetString(resultBytes, 0, resultBytes->Length);
                    }
                    else
                    {
                        return "";
                    }
                }

                return full_string_node->InnerText;
            }
        }
        catch (Exception ^e)
        {
            Console::WriteLine("ErrorMsg: " + e->Message + e->StackTrace);
        }

        return "";
    }
}
