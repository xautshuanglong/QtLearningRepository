#pragma once

#include <QApplication>

class MyApplication : public QApplication
{
public:
    MyApplication(int &argc, char **argv, int appFlag = ApplicationFlags);
    ~MyApplication();
};

