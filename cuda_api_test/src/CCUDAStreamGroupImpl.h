#pragma once
#include "CTestBase.h"
#include "TestDefine.h"
namespace TestCUDA
{
    class CCUDAStreamGroupImpl : public CTestBase
    {
    public:
        CCUDAStreamGroupImpl(const char *_api_name, const char *_error = nullptr)
        {
            SetName(_api_name);
            SetError(_error);
            // AddError(_error);
        };
        CCUDAStreamGroupImpl()
        {
        }
        virtual ~CCUDAStreamGroupImpl()
        {
        }

    public:
        virtual void Test_normal();
        virtual void Test_NonNormal();
    };

}