#pragma once
#include <vector>
#include <string>
#include <memory.h>
#include <stdexcept>
namespace TestCUDA{
class CTestBase{
    public:
    CTestBase(){};
    virtual ~CTestBase(){};

    public:
    // static CTestBase* CreateSelf(const char *_api_name, const int &_pip_flags);
    // static void DestoryPtr(CTestBase *_ptr);
    //
    virtual bool RunBenchMarkTest(int argc, char *argv[]);
    //
    virtual bool SetupTest(const int &_pip_flags) = 0;
    //
    virtual bool SetupBenchMark() = 0;
    //
    virtual bool SetName(const char *_api_name) = 0;
    //
    virtual bool Log(const char *_logStr);
    //
    virtual bool PrintLog(const char *_logFileName = nullptr);
    //
    virtual void AddError(const std::string &_error, const int &_pip_flags);
    //
    virtual std::vector<std::string> GetError();
     
    protected:
    std::vector<std::string> m_log_str;
    std::string m_api_name;
    std::vector<std::string> m_error_api;
};
}