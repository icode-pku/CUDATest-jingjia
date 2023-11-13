#pragma once
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
namespace TestCUDA{
class CTestBase :public std::exception{
    public:
    CTestBase() = default;
    virtual ~CTestBase(){};

    public:
    // static CTestBase* CreateSelf(const char *_api_name, const int &_pip_flags);
    // static void DestoryPtr(CTestBase *_ptr);
    //
    virtual bool RunBenchMarkTest(int argc, char *argv[]);
    //
    virtual bool SetupTest() = 0;
    //
    virtual bool SetupBenchMark() = 0;
    //
    virtual bool SetName(const char *_api_name);
    //
    virtual bool SetError(const char *_error);
    //
    virtual bool SetPipFlags(const int &_flags);
    //
    virtual bool Log(const char *_logStr);
    //
    virtual bool PrintLog(const char *_logFileName = nullptr);
    //
    virtual void AddError(const char *_error);
    //
    virtual const char *what();
    //
    virtual std::vector<const char*> GetError();
     
    protected:
    std::shared_ptr<char> m_api_name;
    std::shared_ptr<char> m_error;
    int m_pip_flags;
    std::vector<const char*> m_log_str;
    std::vector<const char *> m_error_api;
};
}