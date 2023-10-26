#include "CBenchMarkBase.h"

class CBenchMark_CUDAStreamCreateImpl : public CBenchMarkBase{
    public:
    virtual ~CBenchMark_CUDAStreamCreateImpl(){

    }
    public:
    //
    virtual bool SetupTest(); 
    //
    virtual bool SetupBenchMark();
    //
    virtual bool SetName(const char *_api_name);

    protected:
    CBenchMark_CUDAStreamCreateImpl(){};

}