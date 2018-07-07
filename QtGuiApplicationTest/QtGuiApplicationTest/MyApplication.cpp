#include "MyApplication.h"


MyApplication::MyApplication(int &argc, char **argv, int appFlag /* = ApplicationFlags */)
    : QApplication(argc, argv, appFlag)
{
}

MyApplication::~MyApplication()
{
}
