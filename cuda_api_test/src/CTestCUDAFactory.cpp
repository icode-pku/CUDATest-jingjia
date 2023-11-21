#include "CTestCUDAFactory.h"
#include "CTestBase.h"
namespace TestCUDA
{
    CTestCUDAFactory::ptr_type CTestCUDAFactory::Create(const std::string &key)
    {
        if (map_.find(key) == map_.end())
            throw std::invalid_argument("the object is not regist!");

        return map_[key]();
    }

    std::unique_ptr<CTestBase> CTestCUDAFactory::Create_unique(const std::string &key)
    {
        return std::unique_ptr<CTestBase>(CTestCUDAFactory::ptr_type(Create(key)).get());
    }

    std::shared_ptr<CTestBase> CTestCUDAFactory::Create_shared(const std::string &key)
    {
        return Create(key);
    }
}