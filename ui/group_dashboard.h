#pragma once

#include <QDialog>
#include "ui_group_dashboard.h"

class group_dashboard : public QDialog
{
    Q_OBJECT

public:
    group_dashboard(QWidget *parent = nullptr);
    ~group_dashboard();

    QString getGroupName();

public slots:
    // 确认
    void onBtnClickedConfirm();
    // 取消
    void onBtnClickedCancel();

private:
    Ui::group_dashboardClass ui;
};

