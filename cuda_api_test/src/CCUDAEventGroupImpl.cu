
#include "cudaInc.h"
#include <iostream>
#include "CCUDAEventGroupImpl.h"
#include "CTestCUDAFactory.h"
#define N (1024 * 1024)
#define FULL_DATA_SIZE N * 20
namespace TestCUDA
{
    REGISTER_TEST_OBJECT(CCUDAEventGroupImpl, CUDA_EVENT_GROUP);
    __global__ void kernelAdd_event(int *a, int *b, int *c)
    {
        int threadID = blockIdx.x * blockDim.x + threadIdx.x;

        if (threadID < N)
        {
            c[threadID] = (a[threadID] + b[threadID]) / 2;
        }
    }
    void CCUDAEventGroupImpl::Test_normal()
    {
#ifdef _DEBUG
        LOG_DEBUG("CCUDAEventGroupImpl in...");
#endif
        cudaEvent_t start_event, end_event;
        CudaIsSafeCall(cudaEventCreate(&start_event), CCUDAEventGroupImpl);
        CudaIsSafeCall(cudaEventCreate(&end_event), CCUDAEventGroupImpl);
#ifdef _DEBUG
        LOG_DEBUG("CCUDAEventGroupImpl create success...");
#endif
        int *host_a, *host_b, *host_c;
        int *dev_a, *dev_b, *dev_c;

        CudaIsSafeCall(cudaMalloc((void **)&dev_a, N * sizeof(int)), CCUDAEventGroupImpl);
        CudaIsSafeCall(cudaMalloc((void **)&dev_b, N * sizeof(int)), CCUDAEventGroupImpl);
        CudaIsSafeCall(cudaMalloc((void **)&dev_c, N * sizeof(int)), CCUDAEventGroupImpl);

        CudaIsSafeCall(cudaHostAlloc((void **)&host_a, FULL_DATA_SIZE * sizeof(int), cudaHostAllocDefault), CCUDAEventGroupImpl);
        CudaIsSafeCall(cudaHostAlloc((void **)&host_b, FULL_DATA_SIZE * sizeof(int), cudaHostAllocDefault), CCUDAEventGroupImpl);
        CudaIsSafeCall(cudaHostAlloc((void **)&host_c, FULL_DATA_SIZE * sizeof(int), cudaHostAllocDefault), CCUDAEventGroupImpl);
        if (host_a == nullptr || host_b == nullptr || host_c == nullptr)
        {
            throw CCUDAEventGroupImpl("cudaHostAlloc", "CPU 内存开辟失败!");
        }
        for (int i = 0; i < FULL_DATA_SIZE; i++)
        {
            host_a[i] = i;
            host_b[i] = FULL_DATA_SIZE - i;
        }

        CudaIsSafeCall(cudaEventRecord(start_event), CCUDAEventGroupImpl);
        for (int i = 0; i < FULL_DATA_SIZE; i += N)
        {
            CudaIsSafeCall(cudaMemcpy(dev_a, host_a + i, N * sizeof(int), cudaMemcpyHostToDevice), CCUDAEventGroupImpl);
            CudaIsSafeCall(cudaMemcpy(dev_b, host_b + i, N * sizeof(int), cudaMemcpyHostToDevice), CCUDAEventGroupImpl);

            kernelAdd_event<<<N / 1024, 1024>>>(dev_a, dev_b, dev_c);

            CudaIsSafeCall(cudaMemcpy(host_c + i, dev_c, N * sizeof(int), cudaMemcpyDeviceToHost), CCUDAEventGroupImpl);
        }
        CudaIsSafeCall(cudaEventSynchronize(start_event), CCUDAEventGroupImpl);
        CudaIsSafeCall(cudaEventRecord(end_event), CCUDAEventGroupImpl);

        float time_gpu;
        CudaIsSafeCall(cudaEventElapsedTime(&time_gpu, start_event, end_event), CCUDAEventGroupImpl);

        // 事件计时
        // printf("GPU time: %.3f ms \n", time_gpu);
#ifdef _DEBUG
        LOG_DEBUG("CCUDAEventGroupImpl excute success...");
#endif
        CudaIsSafeCall(cudaFreeHost(host_a), CCUDAEventGroupImpl);
        CudaIsSafeCall(cudaFreeHost(host_b), CCUDAEventGroupImpl);
        CudaIsSafeCall(cudaFreeHost(host_c), CCUDAEventGroupImpl);

        CudaIsSafeCall(cudaFree(dev_a), CCUDAEventGroupImpl);
        CudaIsSafeCall(cudaFree(dev_b), CCUDAEventGroupImpl);
        CudaIsSafeCall(cudaFree(dev_c), CCUDAEventGroupImpl);

        CudaIsSafeCall(cudaEventDestroy(start_event), CCUDAEventGroupImpl);
        CudaIsSafeCall(cudaEventDestroy(end_event), CCUDAEventGroupImpl);
#ifdef _DEBUG
        LOG_DEBUG("CCUDAEventGroupImpl out...");
#endif
    }
    void CCUDAEventGroupImpl::Test_NonNormal()
    {
    }

}
