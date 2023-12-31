#pragma once
#include "CTestBase.h"
#include "TestDefine.h"
namespace TestCUDA
{
    class CCUDAEventGroupImpl : public CTestBase
    {
    public:
        CCUDAEventGroupImpl(const char *_api_name, const char *_error = nullptr)
        {
            SetName(_api_name);
            SetError(_error);
        };
        CCUDAEventGroupImpl()
        {
        }
        virtual ~CCUDAEventGroupImpl()
        {
        }

    public:
        virtual void Test_normal();
        virtual void Test_NonNormal();
    };

}