
#include "cudaInc.h"
#include <iostream>
#include "CCUDAMemGroupImpl.h"
#include "CTestCUDAFactory.h"
#define N (1024 * 1024)
#define FULL_DATA_SIZE N * 20
namespace TestCUDA
{
    REGISTER_TEST_OBJECT(CCUDAMemGroupImpl, CUDA_MEM_GROUP);
    __global__ void kernelAdd_mem(int *a, int *b, int *c)
    {
        int threadID = blockIdx.x * blockDim.x + threadIdx.x;

        if (threadID < N)
        {
            c[threadID] = (a[threadID] + b[threadID]) / 2;
        }
    }
    void CCUDAMemGroupImpl::Test_normal()
    {
#ifdef _DEBUG
        LOG_DEBUG("CCUDAMemGroupImpl in...");
#endif
        cudaEvent_t start_event, end_event;
        CudaIsSafeCall(cudaEventCreate(&start_event), CCUDAMemGroupImpl);
        CudaIsSafeCall(cudaEventCreate(&end_event), CCUDAMemGroupImpl);
#ifdef _DEBUG
        LOG_DEBUG("CCUDAMemGroupImpl create success...");
#endif
        int *host_a, *host_b, *host_c;
        int *dev_a, *dev_b, *dev_c;

        CudaIsSafeCall(cudaMalloc((void **)&dev_a, -1 * sizeof(int)), CCUDAMemGroupImpl);
        CudaIsSafeCall(cudaMalloc((void **)&dev_b, N * sizeof(int)), CCUDAMemGroupImpl);
        CudaIsSafeCall(cudaMalloc((void **)&dev_c, N * sizeof(int)), CCUDAMemGroupImpl);

        CudaIsSafeCall(cudaHostAlloc((void **)&host_a, FULL_DATA_SIZE * sizeof(int), cudaHostAllocDefault), CCUDAMemGroupImpl);
        CudaIsSafeCall(cudaMallocHost((void **)&host_b, FULL_DATA_SIZE * sizeof(int), cudaHostAllocDefault), CCUDAMemGroupImpl);
        CudaIsSafeCall(cudaHostAlloc((void **)&host_c, FULL_DATA_SIZE * sizeof(int), cudaHostAllocDefault), CCUDAMemGroupImpl);
        if (host_a == nullptr || host_b == nullptr || host_c == nullptr)
        {
            throw CCUDAMemGroupImpl("cudaHostAlloc", "CPU 内存开辟失败!");
        }
        for (int i = 0; i < FULL_DATA_SIZE; i++)
        {
            host_a[i] = i;
            host_b[i] = FULL_DATA_SIZE - i;
        }

        CudaIsSafeCall(cudaEventRecord(start_event), CCUDAMemGroupImpl);
        for (int i = 0; i < FULL_DATA_SIZE; i += N)
        {
            CudaIsSafeCall(cudaMemcpy(dev_a, host_a + i, N * sizeof(int), cudaMemcpyHostToDevice), CCUDAMemGroupImpl);
            CudaIsSafeCall(cudaMemcpy(dev_b, host_b + i, N * sizeof(int), cudaMemcpyHostToDevice), CCUDAMemGroupImpl);

            kernelAdd_mem<<<N / 1024, 1024>>>(dev_a, dev_b, dev_c);

            CudaIsSafeCall(cudaMemcpy(host_c + i, dev_c, N * sizeof(int), cudaMemcpyDeviceToHost), CCUDAMemGroupImpl);
        }
        CudaIsSafeCall(cudaEventSynchronize(start_event), CCUDAMemGroupImpl);
        CudaIsSafeCall(cudaEventRecord(end_event), CCUDAMemGroupImpl);

        float time_gpu;
        CudaIsSafeCall(cudaEventElapsedTime(&time_gpu, start_event, end_event), CCUDAMemGroupImpl);

        // 事件计时
        // printf("GPU time: %.3f ms \n", time_gpu);
#ifdef _DEBUG
        LOG_DEBUG("CCUDAMemGroupImpl excute success...");
#endif
        CudaIsSafeCall(cudaFreeHost(host_a), CCUDAMemGroupImpl);
        CudaIsSafeCall(cudaFreeHost(host_b), CCUDAMemGroupImpl);
        CudaIsSafeCall(cudaFreeHost(host_c), CCUDAMemGroupImpl);

        CudaIsSafeCall(cudaFree(dev_a), CCUDAMemGroupImpl);
        CudaIsSafeCall(cudaFree(dev_b), CCUDAMemGroupImpl);
        CudaIsSafeCall(cudaFree(dev_c), CCUDAMemGroupImpl);

        CudaIsSafeCall(cudaEventDestroy(start_event), CCUDAMemGroupImpl);
        CudaIsSafeCall(cudaEventDestroy(end_event), CCUDAMemGroupImpl);
#ifdef _DEBUG
        LOG_DEBUG("CCUDAMemGroupImpl out...");
#endif
    }
    void CCUDAMemGroupImpl::Test_NonNormal()
    {
    }
}
