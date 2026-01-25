#include "group_dashboard.h"

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

QString group_dashboard::getGroupName()
{
    return ui.m_edtGroupName->toPlainText();
}

void group_dashboard::onBtnClickedConfirm()
{
    if (ui.m_edtGroupName->toPlainText().isEmpty())
    {

        return;
    }

    accept();
}

void group_dashboard::onBtnClickedCancel()
{
    reject();
}
