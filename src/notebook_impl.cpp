#include "notebook_impl.h"
#include "notebook_utils.h"
#include "question_answer_impl.h"

#include <filesystem>

bool CQuestionBankImpl::init(const std::string & path)
{
    if (!std::filesystem::exists(path) && !std::filesystem::create_directory(path))
    {
        notebook_format_string(m_strErrorMsg, "Create directory '%s' failed", path.c_str());
        return false;
    }

    m_strIdPath = path + "\\ids.dat";
    m_ptrIds = std::make_shared<CQuestionAnswerImpl>();
    if (!m_ptrIds->open(m_strIdPath, false))
    {
        notebook_format_string(m_strErrorMsg, "%s", m_ptrIds->err());
        m_ptrIds.reset();
        m_ptrIds = nullptr;
        return false;
    }

    if (!m_ptrIds->getGroups(m_mapIds))
    {
        notebook_format_string(m_strErrorMsg, "%s", m_ptrIds->err());
        m_ptrIds->close();
        m_ptrIds.reset();
        m_ptrIds = nullptr;
        return false;
    }

    std::filesystem::path root_path = path;
    auto root_name = root_path.filename().string();
    m_clsQuestionBank.m_strName = root_name;
    m_clsQuestionBank.m_strPath = path;

    try 
    {
        for (const auto & entry : std::filesystem::recursive_directory_iterator(path))
        {
            auto parent_name = entry.path().parent_path().filename().string();
            auto cur_name = entry.path().filename().string();
            if (entry.is_regular_file())
            {
                if ("ids.dat" == cur_name)
                    continue;

                if (!initQuestionBank(parent_name, m_clsQuestionBank, entry.path().string(), cur_name, true))
                {
                    return false;
                }
            }
            else if (entry.is_directory())
            {
                if (parent_name == root_name)
                {
                    CQuestionGroupParam qgp;
                    qgp.m_strPath = entry.path().string();
                    qgp.m_strName = cur_name;
                    for (const auto & kv : m_mapIds)
                    {
                        if (cur_name != kv.second)
                            continue;
                        qgp.m_strId = kv.first;
                        break;
                    }
                    m_clsQuestionBank.m_vecGroups.emplace_back(qgp);
                    continue;
                }

                if (!initQuestionBank(parent_name, m_clsQuestionBank, entry.path().string(), cur_name, false))
                {
                    return false;
                }
            }
        }
    }
    catch (const std::filesystem::filesystem_error & e)
    {
        notebook_format_string(m_strErrorMsg, "%s", e.what());
        m_ptrIds->close();
        m_ptrIds.reset();
        m_ptrIds = nullptr;
        return false;
    }

    return true;
}

void CQuestionBankImpl::uninit()
{
    if (m_ptrIds)
    {
        m_ptrIds->close();
        m_ptrIds.reset();
        m_ptrIds = nullptr;
    }

    uninitQuestionBank(m_clsQuestionBank);
}

bool CQuestionBankImpl::getQuestionsBank(CQuestionGroupParam & res)
{
    res = m_clsQuestionBank;
    return true;
}

bool CQuestionBankImpl::addQuestion(const std::string & gid, const std::string & id, const std::string & question, const std::string & answer)
{
    if (m_mapIds.end() == m_mapIds.find(gid))
    {
        notebook_format_string(m_strErrorMsg, "Found group id '%s' failed", gid.c_str());
        return false;
    }

    auto * ctx = findQustionAnswerContext(m_clsQuestionBank, gid);
    if (nullptr == ctx)
    {
        notebook_format_string(m_strErrorMsg, "Found context by group id '%s' failed", gid.c_str());
        return false;
    }
    
    CQuestionAnswerParam qap;
    qap.m_strGroup = gid;
    qap.m_strId = id;
    qap.m_strQuestion = question;
    qap.m_strAnswer = answer;
    return ctx->addQuestion(qap);
}

bool CQuestionBankImpl::deleteQuestion(const std::string & gid, const std::string & id)
{
    if (m_mapIds.end() == m_mapIds.find(gid))
    {
        notebook_format_string(m_strErrorMsg, "Found group id '%s' failed", gid.c_str());
        return false;
    }

    auto * ctx = findQustionAnswerContext(m_clsQuestionBank, gid);
    if (nullptr == ctx)
    {
        notebook_format_string(m_strErrorMsg, "Found context by group id '%s' failed", gid.c_str());
        return false;
    }

    return ctx->deleteQuestion(id);
}

bool CQuestionBankImpl::queryQuestion(const std::string & gid, const std::string & id, std::string & answer)
{
    if (m_mapIds.end() == m_mapIds.find(gid))
    {
        notebook_format_string(m_strErrorMsg, "Found group id '%s' failed", gid.c_str());
        return false;
    }

    auto * ctx = findQustionAnswerContext(m_clsQuestionBank, gid);
    if (nullptr == ctx)
    {
        notebook_format_string(m_strErrorMsg, "Found context by group id '%s' failed", gid.c_str());
        return false;
    }

    return ctx->queryQuestion(id, answer);
}

bool CQuestionBankImpl::updateQuestion(const std::string & gid, const std::string & id, 
                                       const std::string & question, const std::string & answer)
{
    if (m_mapIds.end() == m_mapIds.find(gid))
    {
        notebook_format_string(m_strErrorMsg, "Found group id '%s' failed", gid.c_str());
        return false;
    }

    auto * ctx = findQustionAnswerContext(m_clsQuestionBank, gid);
    if (nullptr == ctx)
    {
        notebook_format_string(m_strErrorMsg, "Found context by group id '%s' failed", gid.c_str());
        return false;
    }

    CQuestionAnswerParam qap;
    qap.m_strGroup = gid;
    qap.m_strId = id;
    qap.m_strQuestion = question;
    qap.m_strAnswer = answer;
    return ctx->updateQuestion(id, qap);
}

bool CQuestionBankImpl::addGroup(const std::string & pid, const std::string & name, const std::string & id)
{
    if (m_mapIds.end() != m_mapIds.find(id))
    {
        notebook_format_string(m_strErrorMsg, "Repeated group id '%s'", id.c_str());
        return false;
    }

    for (const auto & kv : m_mapIds)
    {
        if (kv.second != name)
            continue;

        notebook_format_string(m_strErrorMsg, "Repeated group name '%s'", name.c_str());
        return false;
    }

    return ctrlGroup(pid, id, name, GROUP_CTRL_ADD, m_clsQuestionBank);
}

bool CQuestionBankImpl::deleteGroup(const std::string & pid, const std::string & id)
{
    return ctrlGroup(pid, id, {}, GROUP_CTRL_DELETE, m_clsQuestionBank);
}

bool CQuestionBankImpl::updateGroup(const std::string & pid, const std::string & id, const std::string & name)
{
    return ctrlGroup(pid, id, name, GROUP_CTRL_UPDATE, m_clsQuestionBank);
}

const char * CQuestionBankImpl::err() const
{
    return m_strErrorMsg.c_str();
}

bool CQuestionBankImpl::initQuestionBank(const std::string & parent_name, CQuestionGroupParam & qgp,
                                         const std::string & path, const std::string & name, bool is_file)
{
    if (qgp.m_strName == parent_name)
    {
        if (is_file)
        {
            auto impl = std::make_shared<CQuestionAnswerImpl>();
            if (!impl)
            {
                return false;
            }
            if (!impl->open(path))
            {
                impl.reset();
                return false;
            }

            qgp.m_vecQuestions.emplace_back(impl);
        }
        else
        {
            CQuestionGroupParam tmp;
            tmp.m_strName = name;
            tmp.m_strPath = path;
            for (const auto & kv : m_mapIds)
            {
                if (name != kv.second)
                    continue;
                tmp.m_strId = kv.first;
                break;
            }
            qgp.m_vecGroups.emplace_back(tmp);
        }
        return true;
    }

    auto found = std::find_if(qgp.m_vecGroups.begin(), qgp.m_vecGroups.end(),
                              [&parent_name, &path, &name, &is_file, this](CQuestionGroupParam & grp)
    {
        return initQuestionBank(parent_name, grp, path, name, is_file);
    });
    return found != qgp.m_vecGroups.end();
}

void CQuestionBankImpl::uninitQuestionBank(CQuestionGroupParam & qgp)
{
    for (auto & ctx : qgp.m_vecQuestions)
    {
        ctx->close();
        ctx.reset();
    }
    qgp.m_vecQuestions.clear();

    for (auto & grp : qgp.m_vecGroups)
    {
        uninitQuestionBank(grp);
    }
}

CQuestionAnswerImpl * CQuestionBankImpl::findQustionAnswerContext(CQuestionGroupParam & qgp, const std::string & gid)
{
    if (qgp.m_strId == gid)
    {
        if (qgp.m_vecQuestions.empty())
        {
            auto path = qgp.m_strPath + "\\" + qgp.m_strId + ".dat";
            auto impl = std::make_shared<CQuestionAnswerImpl>();
            if (!impl)
            {
                return false;
            }
            if (!impl->open(path))
            {
                impl.reset();
                return false;
            }

            qgp.m_vecQuestions.emplace_back(impl);
        }

        return qgp.m_vecQuestions[0].get();
    }

    CQuestionAnswerImpl* ctx = nullptr;
    for (auto& grp : qgp.m_vecGroups)
    {
        ctx = findQustionAnswerContext(grp, gid);
        if (nullptr != ctx)
            break;
    }

    return ctx;
}

bool CQuestionBankImpl::addGroup(const std::string & id, const std::string & name, CQuestionGroupParam & qgp)
{
    m_mapIds.emplace(id, name);
    m_ptrIds->addGroup(id, name);

    CQuestionGroupParam grp;
    grp.m_strId = id;
    grp.m_strName = name;
    grp.m_strPath = qgp.m_strPath + "\\" + name;
    qgp.m_vecGroups.emplace_back(grp);

    if (!std::filesystem::exists(grp.m_strPath) && !std::filesystem::create_directory(grp.m_strPath))
    {
        notebook_format_string(m_strErrorMsg, "Create directory '%s' failed", qgp.m_strPath.c_str());
        return false;
    }
    return true;
}

bool CQuestionBankImpl::deleteGroup(const std::string & id, CQuestionGroupParam & qgp)
{
    m_mapIds.erase(id);
    m_ptrIds->deleteGroup(id);

    auto found = std::find_if(qgp.m_vecGroups.begin(), qgp.m_vecGroups.end(),
                              [&id](const CQuestionGroupParam & grp)
    {
        return grp.m_strId == id;
    });
    if (qgp.m_vecGroups.end() == found)
    {
        notebook_format_string(m_strErrorMsg, "Found group by id '%s' failed", id.c_str());
        return false;
    }

    for (auto & ctx : (*found).m_vecQuestions)
    {
        ctx->close();
        ctx.reset();
    }
    (*found).m_vecQuestions.clear();

    for (auto & grp : (*found).m_vecGroups)
    {
        deleteGroup(grp.m_strId, *found);
    }

    try
    {
        if (std::filesystem::exists((*found).m_strPath) && std::filesystem::is_directory((*found).m_strPath))
        {
            std::filesystem::remove_all((*found).m_strPath); // 递归删除整个目录树
        }
    }
    catch (const std::filesystem::filesystem_error & e)
    {
        notebook_format_string(m_strErrorMsg, "%s", e.what());
        return false;
    }

    qgp.m_vecGroups.erase(found);

    return true;
}

bool CQuestionBankImpl::updateGroup(const std::string & id, const std::string & name, CQuestionGroupParam & qgp)
{
    auto found = std::find_if(qgp.m_vecGroups.begin(), qgp.m_vecGroups.end(),
                              [&id](const CQuestionGroupParam & grp)
    {
        return grp.m_strId == id;
    });
    if (qgp.m_vecGroups.end() == found)
    {
        notebook_format_string(m_strErrorMsg, "Found group by id '%s' failed", id.c_str());
        return false;
    }

    m_ptrIds->updateGroup(id, name);

    (*found).m_strName = name;
    std::string path = qgp.m_strPath + "\\" + name;

    try
    {
        std::filesystem::rename((*found).m_strPath, path);
    }
    catch (const std::filesystem::filesystem_error & e)
    {
        notebook_format_string(m_strErrorMsg, "%s", e.what());
        return false;
    }

    return true;
}

bool CQuestionBankImpl::ctrlGroup(const std::string & pid, const std::string & gid, const std::string & name, 
                                  GROUP_CTRL_TYPE type, CQuestionGroupParam & qgp)
{
    if (pid.empty() || pid == qgp.m_strId)
    {
        switch (type)
        {
        case GROUP_CTRL_ADD:
            return addGroup(gid, name, qgp);
        case GROUP_CTRL_DELETE:
            return deleteGroup(gid, qgp);
        case GROUP_CTRL_UPDATE:
            return updateGroup(gid, name, qgp);
        default:
            return false;
        }

        return true;
    }

    auto found = std::find_if(qgp.m_vecGroups.begin(), qgp.m_vecGroups.end(),
                              [&pid, &gid, &name, type, this](CQuestionGroupParam & grp)
    {
        return ctrlGroup(pid, gid, name, type, grp);
    });
    return qgp.m_vecGroups.end() != found;
}
