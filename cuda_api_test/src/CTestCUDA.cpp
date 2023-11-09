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
                pTestBase->SetupTest(_pip_flags);
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
        int pipe_flags[2] = {0};
        if (pipe(pipe_flags) != 0)
        {
            perror("pipe error!");
            return false;
        }
        
        std::map<std::string, std::function<CTestBase *()>>::iterator iter = CTestCUDAFactory::GetInstance().map_.begin();
        for (; iter != CTestCUDAFactory::GetInstance().map_.end(); iter++)
        {
            ProcessOneGroup(iter->first.c_str(), pipe_flags);
        }

        return true;
    }

    void CTestCUDA::ProcessOneGroup(const char *_group_desc, const int *_pipe_flags)
    {
        std::function<void(const char *, const int &)> fn = TestValidityFunc;
        if (CProcessManager::CreateProcess(fn, _group_desc, _pipe_flags[1]) > 0)
        {
            printf("main id: %d\n", getpid());
            close(_pipe_flags[1]);
            // 父进程---读取
            char buffer[64] = {0};
            while (1)
            {
                ssize_t s = read(_pipe_flags[0], buffer, sizeof(buffer));
                if (s == 0)
                {
                    printf("main\n");
                    break;
                }
                else if (s > 0)
                {
                    buffer[s] = 0;
                    int id = std::atoi(std::string(buffer).substr(0, std::string(buffer).find_first_of(":")).c_str());
                    printf("child say to father: %s\n", buffer);
                    kill(id, SIGKILL);
                    break;
                }
            }

            printf("over!!%d\n", getpid());
        }
    }
    bool CTestCUDA::TestBenchMark()
    {

        return true;
    }
}