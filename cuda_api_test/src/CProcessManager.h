#pragma once
#include <stdio.h>
#include <unistd.h>
#include <functional>
#include <signal.h>
//#include <function.h>
#include <map>
#include <string>
class CProcessManager
{
public:
    CProcessManager()
    {
    }
    ~CProcessManager()
    {
    }

public:
    static int CreateProcess(std::function<void(const char *, const int )> &_fn, const char *_test_group, const int *_pip_flags)
    {
        int nRet =  fork();
        if (nRet == 0)
        {
            close(_pip_flags[0]);
            _fn(_test_group, _pip_flags[1]);
            return 0;
        }
        return nRet;
    }
    static void DestroyProcess(const int &_pid){
        kill(_pid, SIGKILL);
    }
    //
    void AddProcess(const char *_process_desc, const int &_pid);
public:
    std::map<std::string, int> m_process_map;
};