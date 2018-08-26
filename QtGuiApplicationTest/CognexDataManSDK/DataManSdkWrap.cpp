#include "stdafx.h"
#include "DataManSdkWrap.h"

#include <vcclr.h>

namespace CognexDataManSDK
{
    DataManSdkWrap::DataManSdkWrap()
        : mListSystemInfo(nullptr)
        , mDataManSystem(nullptr)
        , mSerSystemDiscover(nullptr)
        , mSystemConnector(nullptr)
        , mResultCollector(nullptr)
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
        Console::WriteLine("EnterFunction: DataManSdkWrap::OnSystemDiscovered() " + systemInfo->Name + "-->" + systemInfo->PortName);

        if (systemInfo->Name->Contains("DM8050"))
        {
            Console::WriteLine("Bingo: " + systemInfo->Name + "-->" + systemInfo->PortName);

            try
            {
                mSystemConnector = gcnew SerSystemConnector(systemInfo->PortName, systemInfo->Baudrate);
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
    }

    void DataManSdkWrap::OnSystemConnected(Object ^sender, EventArgs ^args)
    {
        Console::WriteLine("EnterFunction: " + "DataManSdkWrap::OnSystemConnected()");
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
        String^ resultString("");
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
                resultString = this->GetReadStringFromResultXml(tempResult->GetDataAsString());
                break;
            case ResultTypes::ReadString:
                resultString = tempResult->GetDataAsString();
                break;
            }
        }

        String ^resultStr = String::Format("Partial result dropped: {0}, id={1}, TEXT:{2}",
                                           tempResult->Id->Type.ToString(),
                                           tempResult->Id->Id,
                                           resultString);
        Console::WriteLine(resultStr);
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
                    //if (!string.IsNullOrEmpty(full_string_node.InnerText))
                    //{
                    //    byte[] code = Convert.FromBase64String(full_string_node.InnerText);
                    //    return _system.Encoding.GetString(code, 0, code.Length);
                    //}
                    //else
                    //{
                    //    return "";
                    //}
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
