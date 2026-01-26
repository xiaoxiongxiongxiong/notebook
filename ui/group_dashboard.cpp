#include "group_dashboard.h"
#include <QMessageBox>

group_dashboard::group_dashboard(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);
    connect(ui.m_btnGroupConfirm, SIGNAL(clicked()), this, SLOT(onBtnClickedConfirm()));
    connect(ui.m_btnGroupCancel, SIGNAL(clicked()), this, SLOT(onBtnClickedCancel()));
}

group_dashboard::~group_dashboard()
{}

void group_dashboard::setGroupName(const QString & name)
{
    ui.m_edtGroupName->setText(name);
}

QString group_dashboard::getGroupName()
{
    return ui.m_edtGroupName->toPlainText();
}

void group_dashboard::onBtnClickedConfirm()
{
    if (ui.m_edtGroupName->toPlainText().isEmpty())
    {
        QMessageBox::critical(this, QStringLiteral("警告"), QStringLiteral("未设定分组名称！"));
        return;
    }

    accept();
}

void group_dashboard::onBtnClickedCancel()
{
    reject();
}
