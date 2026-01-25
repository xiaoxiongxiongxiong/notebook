#pragma once

#include <QDialog>
#include "ui_question_dashboard.h"

class question_dashboard : public QDialog
{
    Q_OBJECT

public:
    question_dashboard(QWidget *parent = nullptr);
    ~question_dashboard();

public slots:
    // 确认
    void onBtnClickedConfirm();
    // 取消
    void onBtnClickedCancel();

private:
    Ui::question_dashboardClass ui;
};

