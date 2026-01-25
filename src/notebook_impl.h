#pragma once

#include <string>
#include <vector>
#include <unordered_map>

class CNodebookImpl
{
public:
    CNodebookImpl() = default;
    ~CNodebookImpl() = default;

    // 初始化
    bool init(const std::string & path);

    // 销毁
    void uninit();

    // 获取题库  分组<题目描述>
    bool getQuestionsBank(std::unordered_map<std::string, std::vector<std::string>> & res);

    // 添加问题
    bool addQuestion(const std::string & group, const std::string & question, const std::string & answer);

    // 删除问题
    bool deleteQuestion(const std::string & group, const std::string & question);

    // 查询问题
    bool queryQuestion(const std::string & group, const std::string & question, std::string & answer);

    // 删除分组
    bool deleteGroup(const std::string & group);

    // 移动分组
    bool moveGroup(const std::string & src, const std::string & dst);

protected:


private:


};

