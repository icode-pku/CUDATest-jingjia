#pragma once
#include "CTestBase.h"
#include "TestDefine.h"
namespace TestCUDA
{
    class CCUDADeviceGroupImpl : public CTestBase
    {
    public:
        CCUDADeviceGroupImpl(const char *_api_name, const char *_error = nullptr)
        {
            SetName(_api_name);
            SetError(_error);
        };
        CCUDADeviceGroupImpl()
        {
        }
        virtual ~CCUDADeviceGroupImpl()
        {
        }

    public:
        virtual void Test_normal();
        virtual void Test_NonNormal();
    };

}