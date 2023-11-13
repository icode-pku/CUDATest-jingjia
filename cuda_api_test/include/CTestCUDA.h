#pragma once
#include <map>
namespace TestCUDA
{
    class CTestCUDA
    {
    public:
        CTestCUDA(){};
        ~CTestCUDA(){};

    public:
        static bool TestValidity();
        static bool TestBenchMark();

    protected:
        static int ProcessOneGroup(const char *_group_desc, const int *_pipe_flags, std::map<char *, char *> &_error_api_name);
        static void SerializeResult(std::map<char *, char *> &_error_api_name);
    };
}