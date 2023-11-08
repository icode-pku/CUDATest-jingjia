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
    static int CreateProcess(std::function<void(const int &)> &_fn, const int &_pip_flags)
    {
        int nRet = fork();
        if (nRet == 0)
        {
            _fn(_pip_flags);
            return getpid();
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