#pragma once

#include "DataManSdkDefines.h"

/***************************\
*    设备连接状态回调函数
\***************************/
class COGNEX_EXPORT IDeviceConnectionCallback
{
public:
    IDeviceConnectionCallback() {};
    virtual ~IDeviceConnectionCallback() {};

    virtual void OnDeviceDiscoverd() = 0;     // 发现设备
    virtual void OnDeviceConnected() = 0;     // 连接已建立
    virtual void OnDeviceDisconnected() = 0;  // 连接已断开
};
