#pragma once
#include <vector>
#include <string>
#include <memory>
#include <stdexcept>
#include <set>
#include "Logger.h"
namespace TestCUDA
{
    class CTestBase : public std::exception
    {
    public:
        CTestBase() = default;
        virtual ~CTestBase(){};

    public:
        //
        virtual bool RunBenchMarkTest(int argc, char *argv[]);
        //
        virtual bool SetupTest();
        //
        virtual bool SetupBenchMark(){};
        //
        virtual bool SetName(const char *_api_name);
        //
        virtual bool SetError(const char *_error);
        //
        virtual bool SetPipFlags(const int _flags);
        //
        virtual bool Log(const char *_logStr);
        //
        virtual bool PrintLog(const char *_logFileName = nullptr);
        //
        virtual void AddError(const char *_error);
        //
        virtual const char *what();
        //
        virtual void Test_normal() = 0;
        //
        virtual void Test_NonNormal() = 0;
        //
        virtual std::set<std::string> GetError();
        //
        virtual void Add(const std::string &_api_name);
        //
        virtual void SerializeResult(const char *_result_fileName = nullptr);

    public:
        std::shared_ptr<char> m_api_name;
        std::shared_ptr<char> m_error;
        int m_pip_flags;
        std::vector<std::string> m_log_str;
        std::set<std::string> m_error_api;
        std::set<std::string> m_success_api;
    };
}