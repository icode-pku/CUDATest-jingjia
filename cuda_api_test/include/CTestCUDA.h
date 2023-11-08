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
    };
}