#ifndef MISCELLANEOUSQ_OBJECT_CAST_H
#define MISCELLANEOUSQ_OBJECT_CAST_H

#include "MiscellaneousBase.h"

namespace Ui { class MiscellaneousQObjectCast; };

class MiscellaneousQObjectCast : public MiscellaneousBase
{
	Q_OBJECT

public:
	MiscellaneousQObjectCast(QWidget *parent = Q_NULLPTR);
	~MiscellaneousQObjectCast();

	virtual QString GetTitle() override;
	virtual QString GetTitleTooltip() override;
	virtual MiscellaneousTestGroup GetGroupID() override;
	virtual MiscellaneousTestItem GetItemID() override;

private slots:
	void on_btnSameModule_clicked();
	void on_btnDllQObject_clicked();
	void on_btnDllQObjectExtra_clicked();

private:
	Ui::MiscellaneousQObjectCast *ui;
};

#endif // !MISCELLANEOUSQ_OBJECT_CAST_H
