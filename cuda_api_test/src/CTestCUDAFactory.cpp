#include "CTestCUDAFactory.h"
#include "CTestBase.h"
namespace TestCUDA
{
    CTestBase *CTestCUDAFactory::Create(const std::string &key)
    {
        if (map_.find(key) == map_.end())
            throw std::invalid_argument("the object is not regist!");

        return map_[key]();
    }

    std::unique_ptr<CTestBase> CTestCUDAFactory::Create_unique(const std::string &key)
    {
        return std::unique_ptr<CTestBase>(Create(key));
    }

    std::shared_ptr<CTestBase> CTestCUDAFactory::Create_shared(const std::string &key)
    {
        return std::shared_ptr<CTestBase>(Create(key));
    }
}