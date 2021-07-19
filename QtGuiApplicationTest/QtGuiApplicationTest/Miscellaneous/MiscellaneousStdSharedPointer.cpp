#include "MiscellaneousStdSharedPointer.h"
#include "ui_MiscellaneousStdSharedPointer.h"

#include "LogUtil.h"

MiscellaneousStdSharedPointer::MiscellaneousStdSharedPointer(QWidget *parent)
    : MiscellaneousBase(parent)
    , ui(new Ui::MiscellaneousStdSharedPointer())
{
    ui->setupUi(this);
}

MiscellaneousStdSharedPointer::~MiscellaneousStdSharedPointer()
{
    delete ui;
}

QString MiscellaneousStdSharedPointer::GetTitle()
{
    return QObject::tr("Shared Pointer");
}

QString MiscellaneousStdSharedPointer::GetTitleTooltip()
{
    return QObject::tr("How to us standard cpp shared pointer.");
}

MiscellaneousTestGroup MiscellaneousStdSharedPointer::GetGroupID()
{
    return MiscellaneousTestGroup::Standard_Cpp;
}

MiscellaneousTestItem MiscellaneousStdSharedPointer::GetItemID()
{
    return MiscellaneousTestItem::StdCpp_Shared_Pointer;
}

void MiscellaneousStdSharedPointer::on_btnResetTest_clicked()
{
    //std::shared_ptr<AutoDestuctTest> p1 = std::make_shared<AutoDestuctTest>("aaa", 100);
    //p1 = std::make_shared<AutoDestuctTest>("bbb", 200);

    //p1.reset(new AutoDestuctTest("reset", 300));
    //p1 = std::make_shared<AutoDestuctTest>("assign", 400);

    AutoDestuctTest* pOriginal = new AutoDestuctTest("original", 500);
    //std::shared_ptr<AutoDestuctTest> p2(pOriginal);
    //p2.reset(pOriginal); // 重置为原指针，会导致指针悬挂

    //std::shared_ptr<AutoDestuctTest> p3(pOriginal);
    //std::shared_ptr<AutoDestuctTest> p4(pOriginal);
    //p3 = p4; // 与 p2.reset(pOriginal) 相同，同样导致指针悬挂
}

void MiscellaneousStdSharedPointer::on_btnRefCountTest_clicked()
{
    int refCount = 0;

    AutoDestuctTest* pNormal = new AutoDestuctTest("normal", 100);
    std::shared_ptr<AutoDestuctTest> p1(pNormal);
    refCount = p1.use_count();
    std::shared_ptr<AutoDestuctTest> p2 = p1;
    refCount = p2.use_count();
    refCount = p1.use_count();

    //AutoDestuctTest* pUnnormal = new AutoDestuctTest("unnormal", 400);
    //std::shared_ptr<AutoDestuctTest> p3(pUnnormal);
    //std::shared_ptr<AutoDestuctTest> p4(pUnnormal);
    //refCount = p3.use_count();
    //refCount = p4.use_count();
}

AutoDestuctTest::AutoDestuctTest(const std::string& name, int value)
    : mName(name)
    , mValue(value)
{
    LogUtil::Debug(CODE_LOCATION, "Construct 0x%p name:%s  value:%d", this, mName.c_str(), mValue);
}

AutoDestuctTest::~AutoDestuctTest()
{
    LogUtil::Debug(CODE_LOCATION, "Destruct 0x%p name:%s  value:%d", this, mName.c_str(), mValue);
}
