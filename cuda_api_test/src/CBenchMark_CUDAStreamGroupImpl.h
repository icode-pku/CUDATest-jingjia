#pragma once
#include "CTestBase.h"
namespace TestCUDA{
class CBenchMark_CUDAStreamGroupImpl : public CTestBase, public std::exception{
    public:
    CBenchMark_CUDAStreamGroupImpl(const char *_api_name, const char *_error, const int &_pip_flags = -1) : m_error(_error),m_pip_flags(_pip_flags){
        SetName(_api_name);
    };
    virtual ~CBenchMark_CUDAStreamGroupImpl(){

    }
    public:
    //
    virtual bool SetupTest();
    //
    virtual bool SetupBenchMark();
    //
    virtual bool SetName(const char *_api_name);
    //
    virtual const char *what();
    protected:
    std::string m_error;
    int m_pip_flags;
    
};
}