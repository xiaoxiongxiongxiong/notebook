#pragma once

#include <QDialog>
#include "ui_question_dashboard.h"

class QwenClient;

class question_dashboard : public QDialog
{
    Q_OBJECT

public:
    question_dashboard(QWidget *parent = nullptr);
    ~question_dashboard();

    // 设置问题
    void setQuestion(const QString & str);
    // 获取问题
    QString getQuestion();

    // 设置答案
    void setAnswer(const QString & str);
    // 获取答案
    QString getAnswer();

public slots:
    // 确认
    void onBtnClickedConfirm();
    // 取消
    void onBtnClickedCancel();
    void onBtnClickedAnswer();
    void onRecvAnswer(const QString & answer);
    void onRecvError(const QString & error);

private:
    Ui::question_dashboardClass ui;

    QwenClient * m_qwenClient = nullptr;
    QString m_apiKey = "myapikey";
};

