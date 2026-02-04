#pragma once

#include <string>
#include <vector>
#include <memory>
#include <unordered_map>

class CQuestionAnswerImpl;

class CQuestionGroupParam
{
public:
    // 分组名称
    std::string m_strName;
    // 分组id
    std::string m_strId;

    // 路径
    std::string m_strPath;

    // 问题集合
    std::vector<std::shared_ptr<CQuestionAnswerImpl>> m_vecQuestions;

    // 包含分组
    std::vector<CQuestionGroupParam> m_vecGroups;
};

typedef enum _GROUP_CTRL_TYPE
{
    GROUP_CTRL_NONE = -1,
    GROUP_CTRL_ADD,
    GROUP_CTRL_DELETE,
    GROUP_CTRL_UPDATE,
    GROUP_CTRL_MAX
} GROUP_CTRL_TYPE;

class CQuestionBankImpl
{
public:
    CQuestionBankImpl() = default;
    ~CQuestionBankImpl() = default;

    // 初始化
    bool init(const std::string & path);

    // 销毁
    void uninit();

    // 获取题库  分组<题目描述>
    bool getQuestionsBank(CQuestionGroupParam & res);

    // 添加问题
    bool addQuestion(const std::string & gid, const std::string & id, const std::string & question, const std::string & answer);

    // 删除问题
    bool deleteQuestion(const std::string & gid, const std::string & id);

    // 查询问题
    bool queryQuestion(const std::string & gid, const std::string & id, std::string & answer);

    // 更新问题
    bool updateQuestion(const std::string & gid, const std::string & id, const std::string & question, const std::string & answer);

    // 添加分组 pid ==>父节点id
    bool addGroup(const std::string & pid, const std::string & name, const std::string & id);

    // 删除分组
    bool deleteGroup(const std::string & pid, const std::string & id);

    // 更新分组
    bool updateGroup(const std::string & pid, const std::string & id, const std::string & name);

    // 错误信息
    const char * err() const;

private:
    bool initQuestionBank(const std::string & parent_name, CQuestionGroupParam & qgp,
                          const std::string & path, const std::string & name, bool is_file);

    // 销毁题库
    void uninitQuestionBank(CQuestionGroupParam & qgp);

    // 查找问题上下文
    CQuestionAnswerImpl * findQustionAnswerContext(CQuestionGroupParam & qgp, const std::string & gid);

    // 添加分组，qgp为父组
    bool addGroup(const std::string & id, const std::string & name, CQuestionGroupParam & qgp);
    // 删除分组
    bool deleteGroup(const std::string & id, CQuestionGroupParam & qgp);
    // 更新分组
    bool updateGroup(const std::string & id, const std::string & name, CQuestionGroupParam & qgp);

    // 操作分组
    bool ctrlGroup(const std::string & pid, const std::string & gid, const std::string & name,
                   GROUP_CTRL_TYPE type, CQuestionGroupParam & qgp);

private:
    // 错误信息
    std::string m_strErrorMsg;

    // 题库
    CQuestionGroupParam m_clsQuestionBank;

    // id信息存放路径
    std::string m_strIdPath;
    // <id:group>存放文件
    std::shared_ptr<CQuestionAnswerImpl> m_ptrIds = nullptr;
    // <id:group>
    std::unordered_map<std::string, std::string> m_mapIds;
};

