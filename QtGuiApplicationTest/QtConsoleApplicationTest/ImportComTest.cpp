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
    //comTest.CreateInstance(__uuidof(ComClassTest));       // ͨ�� COM ��� GUID ����ʵ��
    comTest.CreateInstance("CSharpComTest.ComClassTest"); // ͨ�� ProgId (C#��ָ��) ����ʵ��
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
