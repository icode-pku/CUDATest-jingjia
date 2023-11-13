#include "CTestCUDA.h"
#include "CTestBase.h"
#include "TestDefine.h"
#include <iostream>
#include <unistd.h>
#include "CProcessManager.h"
#include "cudaInc.h"
#include "CTestCUDAFactory.h"
namespace TestCUDA
{

    static void TestValidityFunc(const char *_test_group, const int &_pip_flags)
    {
        try
        {
            bool bRet = true;
            std::shared_ptr<TestCUDA::CTestBase> pTestBase = CTestCUDAFactory::GetInstance().Create_shared(_test_group);
            if (pTestBase != nullptr)
            {
                pTestBase->SetPipFlags(_pip_flags);
                pTestBase->SetupTest();
                //pTestBase->PrintLog();
            }
            else
            {
                printf("Invalid object!!\n");
            }
        }
        catch (const std::exception &e)
        {
            std::cerr << e.what() << '\n';
        }
    }

    bool CTestCUDA::TestValidity()
    {
        std::vector<std::string> error_vec;
        CProcessManager processManager;

        printf("共有API簇:%d\n", CTestCUDAFactory::GetInstance().map_.size());
        std::map<char *, char *> _error_api_names;
        std::map<std::string, std::function<CTestBase *()>>::iterator iter = CTestCUDAFactory::GetInstance().map_.begin();
        for (; iter != CTestCUDAFactory::GetInstance().map_.end();)
        {
            printf("正在处理:%s,id:%d\n", iter->first.c_str(), getpid());
            int pipe_flags[2] = {0};
            if (pipe(pipe_flags) != 0)
            {
                perror("pipe error!");
                return false;
            }
            int nRet = ProcessOneGroup(iter->first.c_str(), pipe_flags, _error_api_names);
            if (nRet == 0)
            { // 子进程终止掉;
                exit(0);
            }
            iter++;
        }
        SerializeResult(_error_api_names);

        return true;
    }

    int CTestCUDA::ProcessOneGroup(const char *_group_desc, const int *_pipe_flags, std::map<char *, char *> &_error_api_name)
    {
        std::function<void(const char *, const int &)> fn = TestValidityFunc;
        if (CProcessManager::CreateProcess(fn, _group_desc, _pipe_flags) > 0)
        {
            close(_pipe_flags[1]);
            // 父进程---读取
            char buffer[512] = {0};
            while (1)
            {
                ssize_t s = read(_pipe_flags[0], buffer, sizeof(buffer));
                if (s == 0)
                {
                    break;
                }
                else if (s > 0)
                {
                    buffer[s] = 0;
                    int first = std::string(buffer).find_first_of(":");
                    int end = std::string(buffer).find_last_of(":");
                    int id = std::atoi(std::string(buffer).substr(0, first).c_str());
                    std::string error_api = std::string(buffer).substr(end + 1, std::string(buffer).size());
                    std::string error = std::string(buffer).substr(first + 1, end - first - 1);
                    _error_api_name.insert(std::make_pair(strdup(error_api.c_str()), strdup(error.c_str())));
                    //kill(id, SIGKILL);
                    continue;
                }
                else
                {
                    printf("main ID:%d\n", getpid());
                    continue;
                }
            }
            return 1;
        }
        else
        {
            return 0;
        }
    }
    void CTestCUDA::SerializeResult(std::map<char *, char *> &_error_api_name)
    {
        std::map<char *, char *>::iterator iter = _error_api_name.begin();
        for (; iter != _error_api_name.end(); iter++)
        {
            std::cout << "api: " << iter->first << " error: " << iter->second << std::endl;
        }
    }
    bool CTestCUDA::TestBenchMark()
    {

        return true;
    }
}