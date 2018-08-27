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
        virtual void OnDeviceDiscoverd();     // �����豸
        virtual void OnDeviceConnected();     // �����ѽ���
        virtual void OnDeviceDisconnected();  // �����ѶϿ�
        /* IDataManCallback */
        virtual void OnSimpleResultCompleted(int id, unsigned char* pData, int dataLen); // ���ı�����
        virtual void OnComplexResultCompleted(); // ���� �ı���ͼƬ����Ƶ������ͼƬ��
    };
}

#endif // DATAMAN_MANAGER_H
