#pragma once
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
        static void ProcessOneGroup(const char *_group_desc, const int *_pipe_flags);
    };
}