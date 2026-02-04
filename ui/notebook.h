#pragma once

#include <QtWidgets/QMainWindow>
#include "ui_notebook.h"

class CQuestionBankImpl;
class CQuestionGroupParam;

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
    // 修改分组
    void onBtnClickedModifyGroup();

    // 添加问题
    void onBtnClickedAddQuestion();
    // 删除问题
    void onBtnClickedDeleteQuestion();
    // 修改问题
    void onBtnClickedModifyQuestion();

    void onTreeWidgetItemClicked(QTreeWidgetItem * item, int col);

private:
    // 初始化题库
    bool initQuestionBank();
    // 构建题库树
    void initQuestionTree(QTreeWidgetItem * parent_item, const CQuestionGroupParam * qgp);

    // 获取上一个/下一个item flag-true上一个 flag-false下一个
    QTreeWidgetItem * findTreeWidgetItem(QTreeWidgetItem * item, bool flag);

    // 获取唯一id
    std::string uid();
    // 从QString转std::string
    std::string getString(const QString & str);

private:
    Ui::notebookClass ui;

    QTreeWidgetItem * m_ptrQuestion = nullptr;
    QMenu * m_ptrMenu = nullptr;
    CQuestionBankImpl * m_ptrBank = nullptr;
};

