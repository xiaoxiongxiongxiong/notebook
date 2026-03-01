#include "question_dashboard.h"
#include <QMessageBox>
#include "qwenclient.h"

question_dashboard::question_dashboard(QWidget *parent)
    : QDialog(parent)
{
    ui.setupUi(this);

    setWindowFlags(windowFlags() & ~Qt::WindowContextHelpButtonHint);

    // 初始化 Qwen 客户端
    m_qwenClient = new QwenClient(m_apiKey, this);

    connect(ui.m_btnConfirm, SIGNAL(clicked()), this, SLOT(onBtnClickedConfirm()));
    connect(ui.m_btnCancel, SIGNAL(clicked()), this, SLOT(onBtnClickedCancel()));
    // 连接信号槽
    connect(ui.m_btnAnswer, SIGNAL(clicked()), this, SLOT(onBtnClickedAnswer()));
    connect(m_qwenClient, &QwenClient::answerReceived, this, &question_dashboard::onRecvAnswer);
    connect(m_qwenClient, &QwenClient::requestError, this, &question_dashboard::onRecvError);
}

question_dashboard::~question_dashboard()
{
    if (nullptr != m_qwenClient)
    {
        delete m_qwenClient;
        m_qwenClient = nullptr;
    }
}

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

void question_dashboard::onBtnClickedAnswer()
{
    QString question = ui.m_edtQuestion->toPlainText().trimmed();
    if (question.isEmpty())
    {
        QMessageBox::warning(this, QStringLiteral("输入为空"), QStringLiteral("请输入问题后再发送！"));
        return;
    }

    ui.m_edtAnswer->setText(QStringLiteral("等待通义千问回复..."));

    ui.m_btnAnswer->setEnabled(false);
    m_qwenClient->askQuestion(question);
}

void question_dashboard::onRecvAnswer(const QString & answer)
{
    ui.m_btnAnswer->setEnabled(true);
    ui.m_edtAnswer->setPlainText(answer);
}

void question_dashboard::onRecvError(const QString & error)
{
    ui.m_btnAnswer->setEnabled(true);
    ui.m_edtAnswer->setPlainText(QStringLiteral("错误：%1").arg(error));
    QMessageBox::critical(this, QStringLiteral("API 调用失败"), error);
}
