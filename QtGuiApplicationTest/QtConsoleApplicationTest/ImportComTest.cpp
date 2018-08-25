#include "ImportComTest.h"

#include <combaseapi.h>
#include <iostream>

#import "..\\x64\\Debug\\CSharpComTest.tlb"
using namespace CSharpComTest;

ImportComTest::ImportComTest()
{
}

ImportComTest::~ImportComTest()
{
}

void ImportComTest::TestEnter()
{
    this->ComMethodTest();
}

void ImportComTest::TestExit()
{
    ;
}

void ImportComTest::ComMethodTest()
{
    CoInitialize(NULL);

    IComInterfacePtr comTest;
    //comTest.CreateInstance(__uuidof(ComClassTest));       // 通过 COM 类的 GUID 创建实例
    comTest.CreateInstance("CSharpComTest.ComClassTest"); // 通过 ProgId (C#中指定) 创建实例
    int sum = comTest->Add(1, 3);
    int diff = comTest->Minus(3, 5);
    int multiple = comTest->Multiple(3, 4);
    float devide = comTest->Devide(3, 2);
    std::cout << sum << std::endl;
    std::cout << diff << std::endl;
    std::cout << multiple << std::endl;
    std::cout << devide << std::endl;

    CoUninitialize();
}
