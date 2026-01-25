#include "question_dashboard.h"

question_dashboard::question_dashboard(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    connect(ui.m_btnConfirm, SIGNAL(clicked()), this, SLOT(onBtnClickedConfirm()));
    connect(ui.m_btnCancel, SIGNAL(clicked()), this, SLOT(onBtnClickedCancel()));
}

question_dashboard::~question_dashboard()
{}

void question_dashboard::onBtnClickedConfirm()
{
    accept();
}

void question_dashboard::onBtnClickedCancel()
{
    reject();
}

