#pragma once
#include "CTestBase.h"
#include "TestDefine.h"
namespace TestCUDA
{
    class CCUDAEventGroupImpl : public CTestBase, public std::exception
    {
        public:
            CCUDAEventGroupImpl(const char *_api_name, const char *_error = nullptr)
            {
                SetName(_api_name);
                SetError(_error);
                //AddError(_error);
            };
            CCUDAEventGroupImpl(){

            }
            virtual ~CCUDAEventGroupImpl()
            {
            }

        public:
            //
            virtual bool SetupTest();
            //
            virtual bool SetupBenchMark();        
    };
   
}