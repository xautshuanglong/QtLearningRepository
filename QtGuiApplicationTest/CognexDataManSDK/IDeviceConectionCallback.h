#pragma once

/***************************\
*    �豸����״̬�ص�����
\***************************/
class IDeviceConnectionCallback
{
public:
    IDeviceConnectionCallback() {};
    virtual ~IDeviceConnectionCallback() = 0;

    virtual void Connected() = 0;     // �����ѽ���
    virtual void Disconnected() = 0;  // �����ѶϿ�
};
