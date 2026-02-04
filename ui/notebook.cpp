#include "notebook.h"

#include <QHBoxLayout>
#include <QResizeEvent>
#include <QTreeWidgetItem>
#include <QStyleFactory>
#include <QMessageBox>
#include <QMenu>
#include <QUuid>

#include "group_dashboard.h"
#include "question_dashboard.h"
#include "notebook_impl.h"
#include "question_answer_impl.h"

#define NOTEBOOK_ROLE_ID     (Qt::UserRole + 1)
#define NOTEBOOK_ROLE_ANSWER (Qt::UserRole + 2)

notebook::notebook(QWidget *parent)
    : QMainWindow(parent)
{
    ui.setupUi(this);

    ui.m_edtQuestion->setReadOnly(true);
    ui.m_edtAnswer->setReadOnly(true);

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

    connect(ui.m_treeQuestionBank, &QTreeWidget::itemPressed, this, &notebook::onTreeWidgetItemClicked);

    initQuestionBank();
}

notebook::~notebook()
{
    if (nullptr != m_ptrBank)
    {
        m_ptrBank->uninit();
        delete m_ptrBank;
        m_ptrBank = nullptr;
    }

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
    auto * cur_item = ui.m_treeQuestionBank->currentItem();
    auto * item = findTreeWidgetItem(cur_item, true);
    if (nullptr == item)
    {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("已到第一个！"));
        return;
    }

    ui.m_edtQuestion->setText(item->text(0));
    ui.m_edtAnswer->clear();

}

void notebook::onBtnClickedNextQuestion()
{
    auto * cur_item = ui.m_treeQuestionBank->currentItem();
    auto * item = findTreeWidgetItem(cur_item, false);
    if (nullptr == item)
    {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("已到最后一个！"));
        return;
    }

    ui.m_edtQuestion->setText(item->text(0));
    ui.m_edtAnswer->clear();
}

void notebook::onBtnClickedShowAnswer()
{
    auto * item = ui.m_treeQuestionBank->currentItem();
    if (nullptr == item)
    {
        QMessageBox::critical(this, QStringLiteral("警告"), QStringLiteral("获取问题失败！"));
        return;
    }

    auto answer = item->data(0, NOTEBOOK_ROLE_ANSWER).toString();
    if (answer.isEmpty())
    {
        QMessageBox::information(this, QStringLiteral("提示"), QStringLiteral("暂未录入答案！"));
        return;
    }

    ui.m_edtAnswer->setText(answer);
}

void notebook::onBtnClickedAddGroup()
{
    group_dashboard gd;
    int res = gd.exec();
    if (QDialog::Accepted != res)
        return;

    auto grp_name = gd.getGroupName();
    auto * item = ui.m_treeQuestionBank->currentItem();
    const auto id = uid();
    std::string pid;
    if (nullptr != item->parent())
    {
        auto tmp = item->data(0, NOTEBOOK_ROLE_ID);
        pid = tmp.toString().toStdString();
    }
    if (!m_ptrBank->addGroup(pid, getString(grp_name), id))
    {
        QMessageBox::critical(this, QStringLiteral("警告"), QStringLiteral("添加分组失败：%1！").arg(m_ptrBank->err()));
        return;
    }

    QTreeWidgetItem * child = new QTreeWidgetItem(item);
    child->setIcon(0, QIcon(":/notebook/res/group.ico"));
    child->setText(0, grp_name);
    child->setToolTip(0, grp_name);

    child->setData(0, Qt::UserRole, QVariant::fromValue(true));
    child->setData(0, NOTEBOOK_ROLE_ID, QVariant::fromValue(QString::fromStdString(id)));
    item->setExpanded(true);
}

void notebook::onBtnClickedDeleteGroup()
{
    auto * item = ui.m_treeQuestionBank->currentItem();
    auto * parent_item = item->parent();
    if (nullptr == parent_item)
        return;

    bool res = false;
    auto pid = parent_item->data(0, NOTEBOOK_ROLE_ID);
    auto tmp = item->data(0, NOTEBOOK_ROLE_ID);
    if (pid.isValid())
        res = m_ptrBank->deleteGroup(pid.toString().toStdString(), tmp.toString().toStdString());
    else
        res = m_ptrBank->deleteGroup({}, tmp.toString().toStdString());
    if (!res)
    {
        QMessageBox::critical(this, QStringLiteral("警告"), QStringLiteral("删除分组失败：%1！").arg(m_ptrBank->err()));
        return;
    }

    item->takeChildren();
    parent_item->removeChild(item);
}

void notebook::onBtnClickedModifyGroup()
{
    auto * item = ui.m_treeQuestionBank->currentItem();
    group_dashboard gd;
    gd.setGroupName(item->text(0));
    int res = gd.exec();
    if (QDialog::Accepted != res)
        return;

    std::string pid;
    auto tmp = item->data(0, NOTEBOOK_ROLE_ID);
    auto * parent_item = item->parent();
    if (nullptr != parent_item)
    {
        auto ptmp = parent_item->data(0, NOTEBOOK_ROLE_ID);
        if (ptmp.isValid())
            pid = ptmp.toString().toStdString();
    }
    if (!m_ptrBank->updateGroup(pid, tmp.toString().toStdString(), getString(gd.getGroupName())))
    {
        QMessageBox::critical(this, QStringLiteral("警告"), QStringLiteral("更新分组失败：%1！").arg(m_ptrBank->err()));
        return;
    }

    item->setText(0, gd.getGroupName());
    item->setToolTip(0, gd.getGroupName());
    item->setExpanded(true);
}

void notebook::onBtnClickedAddQuestion()
{
    question_dashboard qd;
    int res = qd.exec();
    if (QDialog::Accepted != res)
        return;

    const auto id = uid();
    auto * item = ui.m_treeQuestionBank->currentItem();
    auto gid = item->data(0, NOTEBOOK_ROLE_ID);
    if (!m_ptrBank->addQuestion(gid.toString().toStdString(), id, getString(qd.getQuestion()), getString(qd.getAnswer())))
    {
        QMessageBox::critical(this, QStringLiteral("警告"), QStringLiteral("添加问题失败：%1！").arg(m_ptrBank->err()));
        return;
    }

    QTreeWidgetItem * child = new QTreeWidgetItem(item);
    child->setIcon(0, QIcon(":/notebook/res/question.ico"));
    child->setText(0, qd.getQuestion());
    child->setToolTip(0, qd.getQuestion());
    child->setData(0, Qt::UserRole, QVariant::fromValue(false));
    child->setData(0, NOTEBOOK_ROLE_ID, QVariant::fromValue(QString::fromStdString(id)));
    child->setData(0, NOTEBOOK_ROLE_ANSWER, QVariant::fromValue(qd.getAnswer()));
    item->setExpanded(true);
}

void notebook::onBtnClickedDeleteQuestion()
{
    auto * item = ui.m_treeQuestionBank->currentItem();
    auto * parent_item = item->parent();
    if (nullptr == parent_item)
        return;

    auto gid = parent_item->data(0, NOTEBOOK_ROLE_ID);
    auto qid = item->data(0, NOTEBOOK_ROLE_ID);
    if (!m_ptrBank->deleteQuestion(gid.toString().toStdString(), qid.toString().toStdString()))
    {
        QMessageBox::critical(this, QStringLiteral("警告"), QStringLiteral("删除问题失败：%1！").arg(m_ptrBank->err()));
        return;
    }

    item->takeChildren();
    parent_item->removeChild(item);
}

void notebook::onBtnClickedModifyQuestion()
{
    auto * item = ui.m_treeQuestionBank->currentItem();
    question_dashboard qd;
    qd.setQuestion(item->text(0));
    auto answer = item->data(0, NOTEBOOK_ROLE_ANSWER);
    qd.setAnswer(answer.toString());
    int res = qd.exec();
    if (QDialog::Accepted != res)
        return;

    auto * parent_item = item->parent();
    if (nullptr == parent_item)
    {
        QMessageBox::critical(this, QStringLiteral("警告"), QStringLiteral("获取分组失败！"));
        return;
    }

    auto gid = parent_item->data(0, NOTEBOOK_ROLE_ID).toString().toStdString();
    auto qid = item->data(0, NOTEBOOK_ROLE_ID).toString().toStdString();
    if (!m_ptrBank->updateQuestion(gid, qid, getString(qd.getQuestion()), getString(qd.getAnswer())))
    {
        QMessageBox::critical(this, QStringLiteral("警告"), QStringLiteral("修改问题失败：%1！").arg(m_ptrBank->err()));
        return;
    }

    item->setText(0, qd.getQuestion());
    item->setToolTip(0, qd.getQuestion());
    item->setData(0, NOTEBOOK_ROLE_ANSWER, qd.getAnswer());

}

void notebook::onTreeWidgetItemClicked(QTreeWidgetItem * item, int col)
{
    if (nullptr == m_ptrMenu)
        return;

    auto val = item->data(0, Qt::UserRole);
    if (!val.isValid())
        return;

    if (Qt::LeftButton == qApp->mouseButtons())
    {
        if (!val.toBool())
            ui.m_edtQuestion->setText(item->text(0));
        else
            ui.m_edtQuestion->clear();
        ui.m_edtAnswer->clear();

        return;
    }

    if (Qt::RightButton != qApp->mouseButtons())
        return;

    QList<QAction *> actions = m_ptrMenu->actions();
    for (int i = 0; i < 5; i++)
        actions[i]->setVisible(val.toBool());
    actions[5]->setVisible(!val.toBool());
    actions[6]->setVisible(!val.toBool());

    m_ptrMenu->exec(QCursor::pos());
}

bool notebook::initQuestionBank()
{
    if (nullptr != m_ptrBank)
        return true;

    auto strPath = QCoreApplication::applicationDirPath() + "/QuestionBank";
    m_ptrBank = new(std::nothrow) CQuestionBankImpl();
    if (nullptr == m_ptrBank)
    {
        QMessageBox::critical(this, QStringLiteral("警告"), QStringLiteral("初始化题库失败！"));
        return false;
    }

    if (!m_ptrBank->init(getString(strPath)))
    {
        QMessageBox::critical(this, QStringLiteral("警告"), QStringLiteral("初始化题库失败！"));
        delete m_ptrBank;
        m_ptrBank = nullptr;
        return false;
    }

    CQuestionGroupParam qgp;
    if (!m_ptrBank->getQuestionsBank(qgp))
    {
        QMessageBox::critical(this, QStringLiteral("警告"), QStringLiteral("初始化题库失败！"));
        m_ptrBank->uninit();
        delete m_ptrBank;
        m_ptrBank = nullptr;
        return false;
    }

    initQuestionTree(m_ptrQuestion, &qgp);

    return true;
}

void notebook::initQuestionTree(QTreeWidgetItem * parent_item, const CQuestionGroupParam * qgp)
{
    for (const auto & grp : qgp->m_vecGroups)
    {
        QTreeWidgetItem * child = new QTreeWidgetItem(parent_item);
        child->setIcon(0, QIcon(":/notebook/res/group.ico"));
        child->setText(0, QString::fromStdString(grp.m_strName));
        child->setToolTip(0, QString::fromStdString(grp.m_strName));
        child->setData(0, Qt::UserRole, QVariant::fromValue(true));
        child->setData(0, NOTEBOOK_ROLE_ID, QVariant::fromValue(QString::fromStdString(grp.m_strId)));
        parent_item->setExpanded(true);
        initQuestionTree(child, &grp);
    }

    for (const auto & qa : qgp->m_vecQuestions)
    {
        std::vector<CQuestionAnswerParam> qaps;
        qa->getQuestions(qaps);
        for (const auto & qap : qaps)
        {
            QTreeWidgetItem * child = new QTreeWidgetItem(parent_item);
            child->setIcon(0, QIcon(":/notebook/res/question.ico"));
            child->setText(0, QString::fromStdString(qap.m_strQuestion));
            child->setToolTip(0, QString::fromStdString(qap.m_strQuestion));
            child->setData(0, Qt::UserRole, QVariant::fromValue(false));
            child->setData(0, NOTEBOOK_ROLE_ID, QVariant::fromValue(QString::fromStdString(qap.m_strId)));
            child->setData(0, NOTEBOOK_ROLE_ANSWER, QVariant::fromValue(QString::fromStdString(qap.m_strAnswer)));
            parent_item->setExpanded(true);
        }
    }
}

QTreeWidgetItem * notebook::findTreeWidgetItem(QTreeWidgetItem * item, bool flag)
{
    if (nullptr == item)
        return nullptr;

    int val = flag ? -1 : 1;
    QTreeWidgetItem * cur_item = item;

    while (cur_item)
    {
        // 获取父节点
        auto * parent_item = cur_item->parent();
        if (nullptr == parent_item)
            return nullptr;

        // 如果超出当前分组范围取上一级
        auto index = parent_item->indexOfChild(cur_item) + val;
        if (index < 0 || index >= parent_item->childCount())
        {
            cur_item = parent_item;
            val = flag ? -1 : 1;
            continue;
        }

        // 新的节点为问题节点直接返回
        cur_item = parent_item->child(index);
        auto is_group = cur_item->data(0, Qt::UserRole);
        if (!is_group.toBool())
        {
            ui.m_treeQuestionBank->setCurrentItem(cur_item);
            return cur_item;
        }

        if (cur_item->childCount() > 0)
        {
            cur_item = cur_item->child(flag ? cur_item->childCount() - 1 : 0);
            is_group = cur_item->data(0, Qt::UserRole);
            if (!is_group.toBool())
            {
                ui.m_treeQuestionBank->setCurrentItem(cur_item);
                return cur_item;
            }
            val = 0;
        }
    }

    return nullptr;
}

std::string notebook::uid()
{
    auto id = QUuid::createUuid().toString(QUuid::WithoutBraces);
    std::string res;
    auto tmp = id.toLocal8Bit();
    res.assign(tmp.constData(), tmp.length());

    return res;
}

std::string notebook::getString(const QString & str)
{
    std::string res;
    auto tmp = str.toUtf8();
    res.assign(tmp.constData(), tmp.length());

    return res;
}
