#pragma once

#include "DataManSdkDefines.h"
#include "IDeviceConectionCallback.h"

/***************************\
*    �豸����״̬�ص�����
\***************************/
class COGNEX_EXPORT IDataManCallback : public IDeviceConnectionCallback
{
public:
    IDataManCallback() {};
    virtual ~IDataManCallback() {};

    virtual void OnSimpleResultCompleted(int id, const char* pData, int dataLen) = 0; // ���ı�����
    virtual void OnComplexResultCompleted() = 0; // ���� �ı���ͼƬ����Ƶ������ͼƬ��
};
