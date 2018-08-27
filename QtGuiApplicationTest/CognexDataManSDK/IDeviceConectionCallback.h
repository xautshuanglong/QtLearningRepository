#pragma once

/***************************\
*    �豸����״̬�ص�����
\***************************/
class IDeviceConnectionCallback
{
public:
    IDeviceConnectionCallback() {};
    virtual ~IDeviceConnectionCallback() {};

    virtual void OnDeviceDiscoverd() = 0;     // �����豸
    virtual void OnDeviceConnected() = 0;     // �����ѽ���
    virtual void OnDeviceDisconnected() = 0;  // �����ѶϿ�
};
