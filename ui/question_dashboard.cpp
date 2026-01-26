#include "question_dashboard.h"
#include <QMessageBox>

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

void question_dashboard::setQuestion(const QString & str)
{
    ui.m_edtQuestion->setText(str);
}

QString question_dashboard::getQuestion()
{
    return ui.m_edtQuestion->toPlainText();
}

void question_dashboard::setAnswer(const QString & str)
{
    ui.m_edtAnswer->setText(str);
}

QString question_dashboard::getAnswer()
{
    return ui.m_edtAnswer->toPlainText();
}

void question_dashboard::onBtnClickedConfirm()
{
    if (ui.m_edtQuestion->toPlainText().isEmpty())
    {
        QMessageBox::critical(this, QStringLiteral("警告"), QStringLiteral("未添加问题描述！"));
        return;
    }

    accept();
}

void question_dashboard::onBtnClickedCancel()
{
    reject();
}

