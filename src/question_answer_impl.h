#pragma once

#include <string>
#include <vector>

#include "xn.pb.h"

class CQuestionAnswerParam
{
public:
    // 唯一id
    std::string m_strId;
    // 问题描述
    std::string m_strQuestion;
    // 答案
    std::string m_strAnswer;
    // 所属分组
    std::string m_strGroup;
};

class CQuestionAnswerImpl
{
public:
    CQuestionAnswerImpl() = default;
    ~CQuestionAnswerImpl() = default;

    // 打开
    bool open(const std::string & path);
    // 关闭
    void close();

    // 获取当前分组名字
    const std::string & groupName() const;
    // 获取所包含的分组 <id:name>
    bool getGroups(std::vector<std::pair<std::string, std::string>> & groups);
    // 获取问题
    bool getQuestions(std::vector<CQuestionAnswerParam> & questions);

    // 添加分组
    bool addGroup(const std::string & gid, const std::string & name);
    // 删除分组
    bool deleteGroup(const std::string & gid);
    // 修改分组
    bool updateGroup(const std::string & gid, const std::string & name);

    // 添加问题
    bool addQuestion(const CQuestionAnswerParam & qap);
    // 删除问题
    bool deleteQuestion(const std::string & qid);
    // 查询问题
    bool queryQuestion(const std::string & qid, std::string & answer);
    // 修改问题
    bool updateQuestion(const std::string & qid, const CQuestionAnswerParam & dst);

private:
    // 所属分组
    std::string m_strGroup;
    // 存放位置
    std::string m_strPath;

    xn::QuestionAnswerGroupList m_lstGroups;
    xn::QuestionAnswerList m_lstQuestions;

    // 错误信息
    std::string m_strErrorMsg;
};
