#include "question_answer_impl.h"

#include <fstream>

#include "xn.pb.h"
#include "notebook_utils.h"

bool CQuestionAnswerImpl::open(const std::string & path)
{
	if (path.empty())
	{
		notebook_format_string(m_strErrorMsg, "Input path is empty");
		return false;
	}

	xn::QuestionAnswerGroupList group_list;
	xn::QuestionAnswerList qa_list;

	std::ifstream ifs(path, std::ios::binary);
	if (!qa_list.ParseFromIstream(&ifs))
	{
		notebook_format_string(m_strErrorMsg, "Parse %s failed", path.c_str());
		ifs.close();
		return false;
	}

	if (!group_list.ParseFromIstream(&ifs))
	{
		notebook_format_string(m_strErrorMsg, "Parse %s failed", path.c_str());
		ifs.close();
		return false;
	}

	for (const auto & group : group_list.items())
	{
		m_vecGroups.emplace_back(group.group());
	}

	for (const auto & qa : qa_list.items())
	{
		m_vecQuestions.emplace_back(qa.question());
	}
	ifs.close();

	m_strPath = path;

	return true;
}

void CQuestionAnswerImpl::close()
{
}

const std::string & CQuestionAnswerImpl::groupName() const
{
	return m_strGroup;
}

const std::vector<std::string> & CQuestionAnswerImpl::getGroups()
{
	return m_vecGroups;
}

const std::vector<std::string> & CQuestionAnswerImpl::getQuestions()
{
	return m_vecQuestions;
}

bool CQuestionAnswerImpl::addGroup(const std::string & group)
{
	return true;
}

bool CQuestionAnswerImpl::deleteGroup(const std::string & group)
{
	return true;
}

bool CQuestionAnswerImpl::addQuestion(const std::string & group, const std::string & question, const std::string & answer)
{
	return true;
}

bool CQuestionAnswerImpl::deleteQuestion(const std::string & group, const std::string & question)
{
	if (group != m_strGroup)
	{
		notebook_format_string(m_strErrorMsg, "Invalid group name %s", group.c_str());
		return false;
	}

	if (m_vecQuestions.end() == std::find(m_vecQuestions.begin(), m_vecQuestions.end(), question))
	{
		notebook_format_string(m_strErrorMsg, "Found question failed");
		return false;
	}

	return true;
}

std::string CQuestionAnswerImpl::queryQuestion(const std::string & group, const std::string & question)
{
	std::string answer;

	if (group != m_strGroup)
	{
		notebook_format_string(m_strErrorMsg, "Invalid group name %s", group.c_str());
		return answer;
	}

	if (m_vecQuestions.end() == std::find(m_vecQuestions.begin(), m_vecQuestions.end(), question))
	{
		notebook_format_string(m_strErrorMsg, "Found question failed");
		return answer;
	}

	xn::QuestionAnswerList qa_list;
	std::ifstream ifs(m_strPath, std::ios::binary);
	if (!qa_list.ParseFromIstream(&ifs))
	{
		notebook_format_string(m_strErrorMsg, "Parse %s failed", m_strPath.c_str());
		ifs.close();
		return answer;
	}

	for (const auto & qa : qa_list.items())
	{
		if (question != qa.question())
			continue;

		answer = qa.answer();
		break;
	}
	ifs.close();

	return answer;
}

bool CQuestionAnswerImpl::modifyQuestion(const std::string & src_group, const std::string & src_question, const std::string & src_answer,
										 const std::string & dst_group, const std::string & dst_question, const std::string & dst_answer)
{
	return true;
}