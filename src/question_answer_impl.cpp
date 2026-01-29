#include "question_answer_impl.h"

#include <fstream>

#include "notebook_utils.h"

bool CQuestionAnswerImpl::open(const std::string & path, const bool & flag)
{
	if (path.empty())
	{
		notebook_format_string(m_strErrorMsg, "Input path is empty");
		return false;
	}

    if (!std::filesystem::exists(path))
    {
        m_strPath = path;
        return true;
    }

	std::ifstream ifs(path, std::ios::binary);
	if (!flag && !m_lstGroups.ParseFromIstream(&ifs))
	{
		notebook_format_string(m_strErrorMsg, "Parse %s failed", path.c_str());
        ifs.close();
        return false;
	}

    if (flag && !m_lstQuestions.ParseFromIstream(&ifs))
    {
        notebook_format_string(m_strErrorMsg, "Parse %s failed", path.c_str());
        ifs.close();
        return false;
    }

	ifs.close();

	m_strPath = path;

	return true;
}

void CQuestionAnswerImpl::close()
{
    if (m_lstGroups.items().empty() && m_lstQuestions.items().empty())
    {
        std::filesystem::remove(m_strPath);
        return;
    }

    std::ofstream ofs(m_strPath, std::ios::binary | std::ios::out);
    if (m_lstGroups.items_size() > 0)
        m_lstGroups.SerializeToOstream(&ofs);
    if (m_lstQuestions.items_size() > 0)
        m_lstQuestions.SerializeToOstream(&ofs);
    ofs.close();
}

const std::string & CQuestionAnswerImpl::groupName() const
{
	return m_strGroup;
}

bool CQuestionAnswerImpl::getGroups(std::unordered_map<std::string, std::string> & groups)
{
	if (m_strPath.empty())
	{
		m_strErrorMsg = "Not opened yet";
		return false;
	}

	for (const auto & item : m_lstGroups.items())
	{
		groups.emplace(item.id(), item.group());
	}

	return true;
}

bool CQuestionAnswerImpl::getQuestions(std::vector<CQuestionAnswerParam> & questions)
{
    if (m_strPath.empty())
    {
        m_strErrorMsg = "Not opened yet";
        return false;
    }

	for (const auto & item : m_lstQuestions.items())
	{
		CQuestionAnswerParam qap{};
		qap.m_strId = item.id();
		qap.m_strGroup = item.group();
		qap.m_strQuestion = item.question();
		qap.m_strAnswer = item.answer();
		questions.emplace_back(qap);
	}

	return true;
}

bool CQuestionAnswerImpl::addGroup(const std::string & gid, const std::string & name)
{
    if (m_strPath.empty())
    {
        m_strErrorMsg = "Not opened yet";
        return false;
    }

	const auto & items = m_lstGroups.items();
	auto found = std::find_if(items.begin(), items.end(), [&gid](const xn::QuestionAnswerGroup & qag)
	{
		return qag.id() == gid;
	});
	if (items.end() != found)
	{
		notebook_format_string(m_strErrorMsg, "Repeated group id '%s'", gid.c_str());
		return false;
	}

	auto * item = m_lstGroups.add_items();
	item->set_id(gid);
	item->set_group(name);

	return true;
}

bool CQuestionAnswerImpl::deleteGroup(const std::string & gid)
{
    if (m_strPath.empty())
    {
        m_strErrorMsg = "Not opened yet";
        return false;
    }

    const auto & items = m_lstGroups.items();
    auto found = std::find_if(items.begin(), items.end(), [&gid](const xn::QuestionAnswerGroup & qag)
    {
        return qag.id() == gid;
    });
    if (items.end() == found)
    {
        notebook_format_string(m_strErrorMsg, "Found group id '%s' failed", gid.c_str());
        return false;
    }

    auto index = static_cast<int>(std::distance(items.begin(), found));
    m_lstGroups.mutable_items()->SwapElements(index, m_lstGroups.items_size() - 1);
    m_lstGroups.mutable_items()->RemoveLast();

	return true;
}

bool CQuestionAnswerImpl::updateGroup(const std::string & gid, const std::string & name)
{
    if (m_strPath.empty())
    {
        m_strErrorMsg = "Not opened yet";
        return false;
    }

    const auto & items = m_lstGroups.items();
    auto found = std::find_if(items.begin(), items.end(), [&gid](const xn::QuestionAnswerGroup & qag)
    {
        return qag.id() == gid;
    });
    if (items.end() == found)
    {
        notebook_format_string(m_strErrorMsg, "Found group id '%s' failed", gid.c_str());
        return false;
    }

    auto index = static_cast<int>(std::distance(items.begin(), found));
    auto * group = m_lstGroups.mutable_items(index);
    group->set_group(name);

    return true;
}

bool CQuestionAnswerImpl::addQuestion(const CQuestionAnswerParam & qap)
{
    if (m_strPath.empty())
    {
        m_strErrorMsg = "Not opened yet";
        return false;
    }

    const auto & qid = qap.m_strId;
    const auto & items = m_lstQuestions.items();
    auto found = std::find_if(items.begin(), items.end(), [&qid](const xn::QuestionAnswer & qa)
    {
        return qa.id() == qid;
    });
    if (items.end() != found)
    {
        notebook_format_string(m_strErrorMsg, "Repeated question id '%s'", qid.c_str());
        return false;
    }

    auto * item = m_lstQuestions.add_items();
    item->set_id(qap.m_strId);
    item->set_group(qap.m_strGroup);
    item->set_question(qap.m_strQuestion);
    item->set_answer(qap.m_strAnswer);

	return true;
}

bool CQuestionAnswerImpl::deleteQuestion(const std::string & qid)
{
    if (m_strPath.empty())
    {
        m_strErrorMsg = "Not opened yet";
        return false;
    }

    const auto & items = m_lstQuestions.items();
    auto found = std::find_if(items.begin(), items.end(), [&qid](const xn::QuestionAnswer & qa)
    {
        return qa.id() == qid;
    });
    if (items.end() == found)
    {
        notebook_format_string(m_strErrorMsg, "Get question by id '%s' failed", qid.c_str());
        return false;
    }

    auto index = static_cast<int>(std::distance(items.begin(), found));
    m_lstQuestions.mutable_items()->SwapElements(index, m_lstQuestions.items_size() - 1);
    m_lstQuestions.mutable_items()->RemoveLast();

	return true;
}

bool CQuestionAnswerImpl::queryQuestion(const std::string & qid, std::string & answer)
{
    if (m_strPath.empty())
    {
        m_strErrorMsg = "Not opened yet";
        return false;
    }

    const auto & items = m_lstQuestions.items();
    auto found = std::find_if(items.begin(), items.end(), [&qid](const xn::QuestionAnswer & qa)
    {
        return qa.id() == qid;
    });
    if (items.end() == found)
    {
        notebook_format_string(m_strErrorMsg, "Get question by id '%s' failed", qid.c_str());
        return false;
    }

    answer = (*found).answer();

	return true;
}

bool CQuestionAnswerImpl::updateQuestion(const std::string & qid, const CQuestionAnswerParam & dst)
{
    if (m_strPath.empty())
    {
        m_strErrorMsg = "Not opened yet";
        return false;
    }

    if (qid != dst.m_strId)
    {
        notebook_format_string(m_strErrorMsg, "Src id '%s' and dst id '%s' is mismatch", qid.c_str(), dst.m_strId.c_str());
        return false;
    }

    auto & items = m_lstQuestions.items();
    auto found = std::find_if(items.begin(), items.end(), [&qid](const xn::QuestionAnswer & qa)
    {
        return qa.id() == qid;
    });
    if (items.end() == found)
    {
        notebook_format_string(m_strErrorMsg, "Get question by id '%s' failed", qid.c_str());
        return false;
    }

    auto index = static_cast<int>(std::distance(items.begin(), found));
    auto * qap = m_lstQuestions.mutable_items(index);
    qap->set_group(dst.m_strGroup);
    qap->set_question(dst.m_strQuestion);
    qap->set_answer(dst.m_strAnswer);

	return true;
}

const char * CQuestionAnswerImpl::err() const
{
    return m_strErrorMsg.c_str();
}
