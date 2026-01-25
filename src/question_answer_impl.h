#pragma once

#include <string>
#include <vector>

class CQuestionAnswerImpl
{
public:
    CQuestionAnswerImpl() = default;
    ~CQuestionAnswerImpl() = default;

    // 打开
    bool open(const std::string & path);
    // 关闭
    void close();

    // 分组名字
    const std::string & groupName() const;

    // 包含的分组
    const std::vector<std::string> & getGroups();

    // 包含的问题
    const std::vector<std::string> & getQuestions();

    // 添加分组
    bool addGroup(const std::string & group);
    // 删除分组
    bool deleteGroup(const std::string & group);

    // 添加问题
    bool addQuestion(const std::string & group, const std::string & question, const std::string & answer);
    // 删除问题
    bool deleteQuestion(const std::string & group, const std::string & question);
    // 查询问题
    std::string queryQuestion(const std::string & group, const std::string & question);
    // 修改问题
    bool modifyQuestion(const std::string & src_group, const std::string & src_question, const std::string & src_answer,
                        const std::string & dst_group, const std::string & dst_question, const std::string & dst_answer);

private:
    // 包含分组
    std::vector<std::string> m_vecGroups;
    // 包含问题
    std::vector<std::string> m_vecQuestions;
    // 所属分组
    std::string m_strGroup;
    // 存放位置
    std::string m_strPath;

    // 错误信息
    std::string m_strErrorMsg;
};
