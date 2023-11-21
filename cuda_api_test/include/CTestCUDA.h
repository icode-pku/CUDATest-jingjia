#pragma once
#include <map>
#include <string>
namespace TestCUDA
{
    class CTestCUDA
    {
    public:
        CTestCUDA(){};
        ~CTestCUDA(){};

    public:
        static bool TestValidity(const char *_cuda_api_group = nullptr);
        static bool TestBenchMark();

    protected:
        static int ProcessOneGroup(const char *_group_desc, const int *_pipe_flags, std::map<std::string, std::string> &_error_api_name);
        static void SerializeResult(std::map<std::string, std::string> &_error_api_name);
    };
}