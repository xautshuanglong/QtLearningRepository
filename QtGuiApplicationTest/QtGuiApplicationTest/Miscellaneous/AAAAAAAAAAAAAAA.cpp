// HelloWorld.cpp : Defines the entry point for the console application.
//

#include <stdio.h>
#include <windows.h>

#include <iostream>
#include <string>
#include <type_traits>

void ScanfTest();
void ClassSizeTestEntry();
void TemplateFunction_is_pointer();
void TemplateFunction_is_member_xxx_pointer();
void TemplateFunction_call_if_member_function();


#ifdef __cplusplus
extern "C" {
#endif
    extern void TestFunc(); // 纯 C 函数名测试，带下划线。
    void StructSizeTestEntryInPureC();
#ifdef __cplusplus
}
#endif

void default_value(int value = 20)
{
    std::cout << "Value: " << value << std::endl;
}

class Base
{
public:
    virtual void display(const std::string& strShow = "I am Base class !")
    {
        std::cout << strShow << std::endl;
        std::cout << "Base display" << std::endl;
    }
    virtual ~Base()
    {
    }
};

class Derive : public Base
{
public:
    virtual void display(const std::string& strShow = "I am Derive class !")
    {
        std::cout << strShow << std::endl;
        std::cout << "Derive display" << std::endl;
    }
    virtual ~Derive()
    {
    }
};

class Third : public Derive
{
public:
    virtual void display(const std::string& strShow = "I am Third class !")
    {
        std::cout << std::endl;
        std::cout << strShow << std::endl;
        std::cout << "Third display" << std::endl;
    }
    virtual ~Third()
    {
    }
};

struct NoConstructor
{
    int intValue;
    int* pIntValue;
    float floatValue;
    int s() { return 0; }
};

void MyPrint() { std::cout << "hello world" << std::endl; }
void MyPrint(int a) { std::cout << "a is " << a << std::endl; }
void MyPrint(int a, int b) { std::cout << "a is " << a << "   b is " << b << std::endl; }
typedef void (*FuncPointer1)();
typedef void (*FuncPointer2)(int);
typedef void (*FuncPointer3)(int, int);
void FuncPointerTestEntry()
{
    MyPrint();
    MyPrint(100);
    MyPrint(200, 300);

    FuncPointer1 pFunc1 = MyPrint;
    FuncPointer2 pFunc2 = MyPrint;
    FuncPointer3 pFunc3 = MyPrint;

    pFunc1();
    pFunc2(400);
    pFunc3(500, 600);
}

int main_deprecate()
{
    //FuncPointerTestEntry();
    //TestFunc();

    //NoConstructor *a = new NoConstructor;
    //a->s();
    //delete a;

    //ScanfTest(); // scanf 与 scanf_s 使用效果对比
    //TemplateFunction_is_pointer();
    //TemplateFunction_is_member_xxx_pointer(); // 模板函数使用测试
    //TemplateFunction_call_if_member_function(); // 是成员函数则调之

    ClassSizeTestEntry();
    StructSizeTestEntryInPureC();

    return 0;
}

void ScanfTest()
{
    // 输入长度大于等于5崩溃。结束符需要一字节'\0'
    //char input[5];
    //scanf("%s", input);
    //printf("%s", input);

    char input[5] = { 0 };
    char input2[5] = { 0 };
    int ret = scanf_s("%s %s", input, (unsigned int)sizeof(input), input2, (unsigned int)sizeof(input2)); // 返回成功的域数量
    printf_s("%d --> %s %s\n", ret, input, input2);

    printf_s("errno = %d\n", errno);
}

void ClassSizeTestEntry()
{
    /********************************************************************************************
        C/C++ 编译器命令行选项
        /d1 reportAllClassLayout
        /d1 reportSingleClassLayoutChild (类名无空格)

        内存对齐主要遵循下面三个原则:
        1. 结构体变量的起始地址能够被其最宽的成员大小整除
        2. 结构体每个成员相对于起始地址的偏移能够被其自身大小整除，如果不能则在前一个成员后面补充字节
        3. 结构体总体大小能够被最宽的成员的大小整除，如不能则在后面补充字节

     ********************************************************************************************/

    class BaseEmpty
    {
    };
    class Base
    {
        char a;
        short x;
    };
    class Child : public Base
    {
        short b;
        short d;
        int c;
        int e;
        virtual void FuncChild() {};
    };
    class Grand : public Child
    {
        virtual void FuncChild() override {};
        virtual void FuncGrand() {};
    };

    std::cout << sizeof(BaseEmpty) << std::endl;
    std::cout << sizeof(Base) << std::endl;
    std::cout << sizeof(Child) << std::endl;
    std::cout << sizeof(Grand) << std::endl;
    std::cout << "============================= Class C++ ============================" << std::endl;

    struct A
    {
    };
    struct B
    {
        char a;
    };
    struct C
    {
        char a;
        short b;
    };
    struct D
    {
        char a;
        short b;
        int c;
    };
    struct E
    {
        int c;
        short b;
        char a;
        char e;
        char g;
        double f;
    };

    std::cout << sizeof(A) << std::endl;
    std::cout << sizeof(B) << std::endl;
    std::cout << sizeof(C) << std::endl;
    std::cout << sizeof(D) << std::endl;
    std::cout << sizeof(E) << std::endl;
    std::cout << "============================= Struct C++ ============================" << std::endl;
}

#ifdef __cplusplus
extern "C" {
#endif // __cplusplus
    void StructSizeTestEntryInPureC()
    {
        struct A
        {
        };

        struct B
        {
            char a;
        };

        struct C
        {
            char a;
            short b;
        };

        struct D
        {
            char a;
            short b;
            int c;
        };

        struct E
        {
            int c;
            short b;
            char a;
            char e;
            char g;
            double f;
        };

        std::cout << sizeof(A) << std::endl;
        std::cout << sizeof(B) << std::endl;
        std::cout << sizeof(C) << std::endl;
        std::cout << sizeof(D) << std::endl;
        std::cout << sizeof(E) << std::endl;
        std::cout << "============================= Struct C ============================" << std::endl;
    }
#ifdef __cplusplus
}
#endif // __cplusplus

void TemplateFunction_is_pointer()
{
    class A {};

    std::cout << "============== is_pointer ==============" << std::endl;
    A* pTrivial = nullptr;
    std::cout << std::boolalpha
        << "is_pointer<A> == "
        << std::is_pointer<A>::value
        << std::endl;
    std::cout << std::boolalpha
        << "is_pointer<A*> == "
        << std::is_pointer<A*>::value
        << std::endl;
    std::cout << std::boolalpha
        << "is_pointer<A&> == "
        << std::is_pointer<A&>::value
        << std::endl;
    std::cout << std::boolalpha
        << "is_pointer<int> == "
        << std::is_pointer<int>::value
        << std::endl;
    std::cout << std::boolalpha
        << "is_pointer<int*> == "
        << std::is_pointer<int*>::value
        << std::endl;
    std::cout << std::boolalpha
        << "is_pointer<int**> == "
        << std::is_pointer<int**>::value
        << std::endl;
    std::cout << std::boolalpha
        << "is_pointer<int[10]> == "
        << std::is_pointer<int[10]>::value
        << std::endl;
    std::cout << std::boolalpha
        << "is_pointer<std::nullptr_t> == "
        << std::is_pointer<std::nullptr_t>::value
        << std::endl;
}

void TemplateFunction_is_member_xxx_pointer()
{
    struct Trivial
    {
        int val;
    };

    struct Functional
    {
        int func() { std::cout << "Functional::func()" << std::endl; }
    };

    std::cout << "============== is_member_pointer ==============" << std::endl;
    int Trivial::* pTrivial = &Trivial::val;
    std::cout << std::boolalpha
        << "is_member_pointer<Trivial*> == "
        << std::is_member_pointer<Trivial*>::value
        << std::endl;
    std::cout << std::boolalpha
        << "is_member_pointer<int Trivial::*> == "
        << std::is_member_pointer<int Trivial::*>::value
        << std::endl;
    std::cout << std::boolalpha
        << "is_member_pointer<void (Trivial::*)()> == "
        << std::is_member_pointer<void (Trivial::*)()>::value
        << std::endl;
    std::cout << std::endl;
    std::cout << std::boolalpha
        << "is_member_pointer <decltype(pTrivial)> == "
        << std::is_member_pointer<decltype(pTrivial)>::value
        << std::endl;
    std::cout << "typeid(decltype(pTrivial)).name() == "
        << typeid(decltype(pTrivial)).name()
        << std::endl;

    std::cout << std::endl << std::endl;
    std::cout << "============== is_member_object_pointer ==============" << std::endl;
    std::cout << "is_member_object_pointer <Trivial*> == "
        << std::boolalpha
        << std::is_member_object_pointer<Trivial*>::value
        << std::endl;
    std::cout << "is_member_object_pointer <int Trivial::*> == "
        << std::boolalpha
        << std::is_member_object_pointer<int Trivial::*>::value
        << std::endl;
    std::cout << "is_member_object_pointer <int (Functional::*)()> == "
        << std::boolalpha
        << std::is_member_object_pointer<int (Functional::*)()>::value
        << std::endl;

    std::cout << std::endl << std::endl;
    std::cout << "============== is_member_function_pointer ==============" << std::endl;
    std::cout << "is_member_function_pointer <Trivial*> == "
        << std::boolalpha
        << std::is_member_function_pointer<Trivial*>::value
        << std::endl;
    std::cout << "is_member_function_pointer <int Trivial::*> == "
        << std::boolalpha
        << std::is_member_function_pointer<int Trivial::*>::value
        << std::endl;
    std::cout << "is_member_function_pointer <int (Functional::*)()> == "
        << std::boolalpha
        << std::is_member_function_pointer<int (Functional::*)()>::value
        << std::endl;
}

template<typename T>
std::string call_if_has_str(T& t)
{
    std::cout << "............ 1. common template" << std::endl;
    return "null";
}

template<typename T>
std::string call_if_has_str(std::string& t)
{
    std::cout << "............ 2. std::string template" << std::endl;
    return t;
}

template<typename T, std::string(T::* U)() = &T::str>
std::string call_if_has_str(typename std::enable_if<std::is_class<T>::value && !std::is_same<T, std::string>::value, T>::type& t)
{
    std::cout << "............ 3. T::str template" << std::endl;
    return t.str();
}

template<typename T>
std::string call_if_has_str(typename std::enable_if<std::is_arithmetic<T>::value, T>::type& t)
{
    std::cout << "............ 4. number template" << std::endl;
    return std::to_string(t);
}

#define HAS_MEMBER_FUNCTION(XXX)                                \
template<typename T>                                            \
struct has_member_function_##XXX                                \
{                                                               \
    template<typename U, std::string (U::*)() = &U::XXX>        \
    static constexpr bool check(U*) { return true; }            \
    static constexpr bool check(...) { return false; }          \
    static constexpr bool value = check(static_cast<T*>(0));    \
};
HAS_MEMBER_FUNCTION(str)
#undef HAS_MEMBER_FUNCTION

void TemplateFunction_call_if_member_function()
{
    class Box
    {
    public:
        std::string str()
        {
            return "yes";
        }
    };

    class Bin
    {
    public:
        std::string str1()
        {
            return "no";
        }

        std::string str(int intValue)
        {
            return std::to_string(intValue);
        }

        //std::string str()
        //{
        //    return "yes";
        //}
    };

    std::cout << std::endl << std::endl;
    std::cout << "============== call_if_member_function ==============" << std::endl;

    std::string stringObj = "this is a std::string object";
    std::cout << "call_if_has_str<stringObj> : " << call_if_has_str<std::string>(stringObj) << std::endl;

    bool boolObj = false;
    std::cout << "call_if_has_str<boolObj> : " << call_if_has_str<bool>(boolObj) << std::endl;

    int intObj = 1024;
    std::cout << "call_if_has_str<intObj> : " << call_if_has_str<int>(intObj) << std::endl;

    float floatObj = 3.14f;
    std::cout << "call_if_has_str<floatObj> : " << call_if_has_str<float>(floatObj) << std::endl;

    double doubleObj = 3.1415926543;
    std::cout << "call_if_has_str<doubleObj> : " << call_if_has_str<double>(doubleObj) << std::endl;

    Box boxObj;
    std::cout << "call_if_has_str<boxObj> : " << call_if_has_str<Box>(boxObj) << std::endl;

    Bin binObj;
    std::cout << "call_if_has_str<binObj> : " << call_if_has_str<Bin>(binObj) << std::endl;

    std::cout << std::boolalpha << "has_member_function_str<Box> : " << has_member_function_str<Box>::value << std::endl;
    std::cout << std::boolalpha << "has_member_function_str<Bin> : " << has_member_function_str<Bin>::value << std::endl;
}
