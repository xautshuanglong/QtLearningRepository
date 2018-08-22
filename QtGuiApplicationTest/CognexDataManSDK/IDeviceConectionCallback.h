#pragma once

/***************************\
*    设备连接状态回调函数
\***************************/
class IDeviceConnectionCallback
{
public:
    IDeviceConnectionCallback() {};
    virtual ~IDeviceConnectionCallback() = 0;

    virtual void Connected() = 0;     // 连接已建立
    virtual void Disconnected() = 0;  // 连接已断开
};
