#pragma once

#include "IDeviceConectionCallback.h"

/***************************\
*    设备连接状态回调函数
\***************************/
class IDataManCallback : public IDeviceConnectionCallback
{
public:
    IDataManCallback() {};
    virtual ~IDataManCallback() {};

    virtual void OnSimpleResultCompleted(int id, unsigned char* pData, int dataLen) = 0; // 简单文本数据
    virtual void OnComplexResultCompleted() = 0; // 包括 文本、图片、视频（连续图片）
};
