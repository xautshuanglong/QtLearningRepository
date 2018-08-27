#pragma once

#include "DataManSdkDefines.h"

/***************************\
*    �豸����״̬�ص�����
\***************************/
class COGNEX_EXPORT IDeviceConnectionCallback
{
public:
    IDeviceConnectionCallback() {};
    virtual ~IDeviceConnectionCallback() {};

    virtual void OnDeviceDiscoverd() = 0;     // �����豸
    virtual void OnDeviceConnected() = 0;     // �����ѽ���
    virtual void OnDeviceDisconnected() = 0;  // �����ѶϿ�
};
