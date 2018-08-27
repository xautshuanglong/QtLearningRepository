#pragma once

#include "IDeviceConectionCallback.h"

/***************************\
*    �豸����״̬�ص�����
\***************************/
class IDataManCallback : public IDeviceConnectionCallback
{
public:
    IDataManCallback() {};
    virtual ~IDataManCallback() {};

    virtual void OnSimpleResultCompleted(int id, unsigned char* pData, int dataLen) = 0; // ���ı�����
    virtual void OnComplexResultCompleted() = 0; // ���� �ı���ͼƬ����Ƶ������ͼƬ��
};
