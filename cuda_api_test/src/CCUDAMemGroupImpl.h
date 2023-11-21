#pragma once
#include "CTestBase.h"
#include "TestDefine.h"
namespace TestCUDA
{
    class CCUDAMemGroupImpl : public CTestBase
    {
    public:
        CCUDAMemGroupImpl(const char *_api_name, const char *_error = nullptr) : CTestBase()
        {
            SetName(_api_name);
            SetError(_error);
        };
        CCUDAMemGroupImpl(){};
        virtual ~CCUDAMemGroupImpl()
        {
        }

    public:
        virtual void Test_normal();
        virtual void Test_NonNormal();
    };

}