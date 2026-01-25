#include "notebook.h"

#include <QHBoxLayout>
#include <QResizeEvent>
#include <QTreeWidgetItem>
#include <QStyleFactory>
#include <QMenu>

#include "group_dashboard.h"
#include "question_dashboard.h"

notebook::notebook(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    connect(ui.m_btnLast, SIGNAL(clicked()), this, SLOT(onBtnClickedLastQuestion()));
    connect(ui.m_btnCtrl, SIGNAL(clicked()), this, SLOT(onBtnClickedShowAnswer()));
    connect(ui.m_btnNext, SIGNAL(clicked()), this, SLOT(onBtnClickedNextQuestion()));

    ui.m_treeQuestionBank->setStyle(QStyleFactory::create("windows"));
    m_ptrQuestion = new QTreeWidgetItem(ui.m_treeQuestionBank);
    m_ptrQuestion->setText(0, QStringLiteral("题库"));
    m_ptrQuestion->setData(0, Qt::ForegroundRole, QBrush(Qt::blue));
    m_ptrQuestion->setData(0, Qt::UserRole, QVariant::fromValue(true));

    m_ptrMenu = new QMenu(ui.m_treeQuestionBank);
    QAction * add_group = new QAction(QStringLiteral("添加分组"));
    // add_folder->setIcon(QIcon(":/ctv_cloud_transcode_test/res/folder.ico"));
    m_ptrMenu->addAction(add_group);
    connect(add_group, &QAction::triggered, this, &notebook::onBtnClickedAddGroup);

    QAction * delete_group = new QAction(QStringLiteral("删除分组"));
    //add_file->setIcon(QIcon(":/ctv_cloud_transcode_test/res/file.ico"));
    m_ptrMenu->addAction(delete_group);
    connect(delete_group, &QAction::triggered, this, &notebook::onBtnClickedDeleteGroup);

    QAction * modify_group = new QAction(QStringLiteral("修改分组"));
    m_ptrMenu->addAction(modify_group);
    connect(modify_group, &QAction::triggered, this, &notebook::onBtnClickedModifyGroup);

    QAction * move_group = new QAction(QStringLiteral("移动分组"));
    m_ptrMenu->addAction(move_group);
    connect(move_group, &QAction::triggered, this, &notebook::onBtnClickedMoveGroup);

    m_ptrMenu->addSeparator();

    QAction * add_question = new QAction(QStringLiteral("添加问题"));
    m_ptrMenu->addAction(add_question);
    connect(add_question, &QAction::triggered, this, &notebook::onBtnClickedAddQuestion);

    QAction * delete_question = new QAction(QStringLiteral("删除问题"));
    m_ptrMenu->addAction(delete_question);
    connect(delete_question, &QAction::triggered, this, &notebook::onBtnClickedDeleteQuestion);

    QAction * modify_question = new QAction(QStringLiteral("修改问题"));
    m_ptrMenu->addAction(modify_question);
    connect(modify_question, &QAction::triggered, this, &notebook::onBtnClickedModifyQuestion);

    QAction * move_question = new QAction(QStringLiteral("移动问题"));
    m_ptrMenu->addAction(move_question);
    connect(move_question, &QAction::triggered, this, &notebook::onBtnClickedModifyQuestion);

    connect(ui.m_treeQuestionBank, &QTreeWidget::itemPressed, this, &notebook::onTreeWidgetItemClicked);
}

notebook::~notebook()
{
    if (nullptr != m_ptrQuestion)
    {
        delete m_ptrQuestion;
        m_ptrQuestion = nullptr;
    }

    if (nullptr != m_ptrMenu)
    {
        delete m_ptrMenu;
        m_ptrMenu = nullptr;
    }
}

void notebook::onBtnClickedLastQuestion()
{

}

void notebook::onBtnClickedNextQuestion()
{

}

void notebook::onBtnClickedShowAnswer()
{

}

void notebook::onBtnClickedAddGroup()
{
    group_dashboard gd;
    int res = gd.exec();
    if (QDialog::Accepted != res)
        return;

    auto * item = ui.m_treeQuestionBank->currentItem();
    QTreeWidgetItem * child = new QTreeWidgetItem(item);
    child->setText(0, gd.getGroupName());
    child->setData(0, Qt::UserRole, QVariant::fromValue(true));
    item->setExpanded(true);
}

void notebook::onBtnClickedDeleteGroup()
{
    auto * item = ui.m_treeQuestionBank->currentItem();
    auto * parent_item = item->parent();
    if (nullptr == parent_item)
        return;

    item->takeChildren();
    parent_item->removeChild(item);
}

void notebook::onBtnClickedMoveGroup()
{
}

void notebook::onBtnClickedModifyGroup()
{
    group_dashboard gd;
    int res = gd.exec();
    if (QDialog::Accepted != res)
        return;

    auto * item = ui.m_treeQuestionBank->currentItem();
    item->setText(0, gd.getGroupName());
    item->setData(0, Qt::UserRole, QVariant::fromValue(true));
    item->setExpanded(true);
}

void notebook::onBtnClickedAddQuestion()
{
    question_dashboard qd;
    int res = qd.exec();
    if (QDialog::Accepted != res)
        return;

    auto * item = ui.m_treeQuestionBank->currentItem();
    QTreeWidgetItem * child = new QTreeWidgetItem(item);
    child->setText(0, "frueiuruofiu");
    child->setData(0, Qt::UserRole, QVariant::fromValue(false));
    item->setExpanded(true);
}

void notebook::onBtnClickedDeleteQuestion()
{
    auto * item = ui.m_treeQuestionBank->currentItem();
    auto * parent_item = item->parent();
    if (nullptr == parent_item)
        return;

    item->takeChildren();
    parent_item->removeChild(item);
}

void notebook::onBtnClickedMoveQuestion()
{
}

void notebook::onBtnClickedModifyQuestion()
{
}

void notebook::onTreeWidgetItemClicked(QTreeWidgetItem * item, int col)
{
    if (nullptr == m_ptrMenu)
        return;
    if (Qt::RightButton != qApp->mouseButtons())
        return;

    auto val = item->data(0, Qt::UserRole);
    if (!val.isValid())
        return;

    QList<QAction *> actions = m_ptrMenu->actions();
    for (int i = 0; i < 6; i++)
        actions[i]->setVisible(val.toBool());

    m_ptrMenu->exec(QCursor::pos());
}
