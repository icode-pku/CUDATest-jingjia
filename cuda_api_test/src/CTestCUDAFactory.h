#pragma once
#include <map>
#include <string>
#include <functional>
#include <memory>
#include <stdexcept>
namespace TestCUDA
{
    class CTestBase;
    class CTestCUDAFactory
    {
    public:
        template <typename T>
        class CTestCUDAFactoryRegister_t
        {
            public:
            CTestCUDAFactoryRegister_t(const std::string &key)
            {
                CTestCUDAFactory::GetInstance().map_.emplace(key, []
                                                             { return new T(); });
            }

            template <typename... Args>
            CTestCUDAFactoryRegister_t(const std::string &key, Args... args)
            {
                CTestCUDAFactory::GetInstance().map_.emplace(key, [=]
                                                             { return new T(args...); });
            }
        };

        CTestBase *Create(const std::string &key);
        std::unique_ptr<CTestBase> Create_unique(const std::string &key);
        std::shared_ptr<CTestBase> Create_shared(const std::string &key);
        static CTestCUDAFactory &GetInstance()
        {
            static CTestCUDAFactory instance;
            return instance;
        }

    private:
        CTestCUDAFactory(){};
        CTestCUDAFactory(const CTestCUDAFactory &) = delete;
        CTestCUDAFactory(CTestCUDAFactory &&) = delete;

    public:
        std::map<std::string, std::function<CTestBase *()>> map_;
    };
    

#define REGISTER_TEST_OBJECT_VNAME(T) reg_msg_##T##_
#define REGISTER_TEST_OBJECT(T, key, ...) static CTestCUDAFactory::CTestCUDAFactoryRegister_t<T> REGISTER_TEST_OBJECT_VNAME(T)(key, ##__VA_ARGS__);
};
