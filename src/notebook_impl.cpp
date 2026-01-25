#include "notebook_impl.h"
#include "xn.pb.h"

#include <filesystem>

bool CNodebookImpl::init(const std::string & path)
{
    try 
    {
        for (const auto & entry : std::filesystem::recursive_directory_iterator(path))
        {
            if (entry.is_regular_file())
            {
                std::cout << "[FILE] " << entry.path().string() << "\n";
            }
            else if (entry.is_directory())
            {
                std::cout << "[DIR]  " << entry.path().string() << "\n";
            }
        }
    }
    catch (const std::filesystem::filesystem_error & e)
    {
        std::cerr << "Error: " << e.what() << '\n';
        return false;
    }

    return true;
}

void CNodebookImpl::uninit()
{
}

bool CNodebookImpl::getQuestionsBank(std::unordered_map<std::string, std::vector<std::string>> & res)
{
    return true;
}

bool CNodebookImpl::addQuestion(const std::string & group, const std::string & question, const std::string & answer)
{
    return true;
}

bool CNodebookImpl::deleteQuestion(const std::string & group, const std::string & question)
{
    return true;
}

bool CNodebookImpl::queryQuestion(const std::string & group, const std::string & question, std::string & answer)
{
    return true;
}

bool CNodebookImpl::deleteGroup(const std::string & group)
{
    return true;
}

bool CNodebookImpl::moveGroup(const std::string & src, const std::string & dst)
{
    return true;
}
