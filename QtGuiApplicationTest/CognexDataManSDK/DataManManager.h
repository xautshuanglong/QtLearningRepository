// CognexDataManSDK.h
#ifndef DATAMAN_MANAGER_H
#define DATAMAN_MANAGER_H

#include "DataManSdkDefines.h"
#include "IDataManCallback.h"

namespace CognexDataManSDK
{
    class COGNEX_EXPORT DataManManager : public IDataManCallback
    {
    public:
        DataManManager();
        ~DataManManager();

        void Discover();
        void Connect();
        void Disconnect();
        void AddEventListener(IDataManCallback *pEventListener);

        /* IDeviceConnectionCallback */
        virtual void OnDeviceDiscoverd();     // 发现设备
        virtual void OnDeviceConnected();     // 连接已建立
        virtual void OnDeviceDisconnected();  // 连接已断开
        /* IDataManCallback */
        virtual void OnSimpleResultCompleted(int id, unsigned char* pData, int dataLen); // 简单文本数据
        virtual void OnComplexResultCompleted(); // 包括 文本、图片、视频（连续图片）
    };
}

#endif // DATAMAN_MANAGER_H
