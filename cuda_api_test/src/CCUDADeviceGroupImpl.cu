#include "CCUDADeviceGroupImpl.h"
#include "cudaInc.h"
#include "TestDefine.h"
#include "Logger.h"
#include "CTestCUDAFactory.h"
namespace TestCUDA
{
    REGISTER_TEST_OBJECT(CCUDADeviceGroupImpl, CUDA_DEVICE_GROUP);
    void CCUDADeviceGroupImpl::Test_normal()
    {
        int device = 0;
        CudaIsSafeCall(cudaGetDevice(&device), CCUDADeviceGroupImpl);
#ifdef _DEBUG
        LOG_DEBUG("cudaGetDevice ... ");
#endif
        int device_num = 0;
        CudaIsSafeCall(cudaGetDeviceCount(&device_num), CCUDADeviceGroupImpl);
#ifdef _DEBUG
        LOG_DEBUG("cudaGetDeviceCount ... ");
        LOG_DEBUG("api cudaGetDeviceProperties ...");
#endif
        for (size_t i = 0; i < device_num; i++)
        {
            cudaDeviceProp prop;
            CudaIsSafeCall(cudaGetDeviceProperties(&prop, i), CCUDADeviceGroupImpl);
            LOG_INFO("    -- device name :%s\n", prop.name);
            LOG_INFO("    -- device global memory size :%lf Gb\n", double(prop.totalGlobalMem) / 1024.0f / 1024.0f / 1024.0f);
            LOG_INFO("    -- device sharedMemPerBlock size: %lf Kb\n", double(prop.sharedMemPerBlock) / 1024.0f);
            LOG_INFO("    -- device warpSize: %d\n", prop.warpSize);
            LOG_INFO("    -- device maxThreadsPerBlock: %d\n", prop.maxThreadsPerBlock);
        }
    }
    void CCUDADeviceGroupImpl::Test_NonNormal()
    {
    }
}