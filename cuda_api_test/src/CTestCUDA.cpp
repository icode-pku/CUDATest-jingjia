#include "CTestCUDA.h"
#include "CTestBase.h"
#include "TestDefine.h"
#include <iostream>
#include <unistd.h>
#include "CProcessManager.h"
#include "cudaInc.h"
namespace TestCUDA
{
    static void TestValidity_StreamGroup(const int &_pip_flags)
    {
        bool bRet = true;
        TestCUDA::CTestBase *pTestBase = TestCUDA::CTestBase::Create(CUDA_STREAM_GROUP, _pip_flags);
        bRet &= pTestBase->SetupTest();
        TestCUDA::CTestBase::DestoryPtr(pTestBase);
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
        std::function<void(const int &)> fn = TestValidity_StreamGroup;
        if (CProcessManager::CreateProcess(fn, pipe_flags[1]) > 0)
        {
            // processManager.AddProcess(CUDA_STREAM_GROUP, );
            printf("main id: %d\n", getpid());
            // 父进程---读取
            char buffer[64] = {0};
            while (1)
            {
                ssize_t s = read(pipe_flags[0], buffer, sizeof(buffer));
                if (s == 0)
                {
                    printf("main pro\n");
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
                else
                {
                    break;
                }
            }
            cudaGetLastError();
            printf("over!!\n");
            for (int i = 0; i < error_vec.size(); ++i)
            { // 可序列化
                std::cout << error_vec[i].c_str() << std::endl;
            }
        }

        return true;
    }
    bool CTestCUDA::TestBenchMark()
    {

        return true;
    }
}