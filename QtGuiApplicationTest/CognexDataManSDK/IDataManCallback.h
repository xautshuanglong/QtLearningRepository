#pragma once

#include "IDeviceConectionCallback.h"

/***************************\
*    设备连接状态回调函数
\***************************/
class IDataManCallback : public IDeviceConnectionCallback
{
public:
    IDataManCallback() {};
    virtual ~IDataManCallback() = 0;
};
