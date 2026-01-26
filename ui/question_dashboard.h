#pragma once

#include <QDialog>
#include "ui_question_dashboard.h"

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

private:
    Ui::question_dashboardClass ui;
};

