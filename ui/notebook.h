#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_notebook.h"

class notebook : public QMainWindow
{
    Q_OBJECT

public:
    notebook(QWidget *parent = nullptr);
    ~notebook();

public slots:
    void onBtnClickedLastQuestion();
    void onBtnClickedNextQuestion();
    void onBtnClickedShowAnswer();

    // 添加分组
    void onBtnClickedAddGroup();
    // 删除分组
    void onBtnClickedDeleteGroup();
    // 移动分组
    void onBtnClickedMoveGroup();
    // 修改分组
    void onBtnClickedModifyGroup();

    // 添加问题
    void onBtnClickedAddQuestion();
    // 删除问题
    void onBtnClickedDeleteQuestion();
    // 移动问题
    void onBtnClickedMoveQuestion();
    // 修改问题
    void onBtnClickedModifyQuestion();

    void onTreeWidgetItemClicked(QTreeWidgetItem * item, int col);

private:
    Ui::notebookClass ui;

    QTreeWidgetItem * m_ptrQuestion = nullptr;
    QMenu * m_ptrMenu = nullptr;
};

