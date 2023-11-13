
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

    bool CCUDAEventGroupImpl::SetupTest()
    {
        BEGIN_EXCEPTION
        (this)->Log("CCUDAEventGroupImpl in...");

        cudaEvent_t start_event, end_event;
        CudaSafeCallEx(cudaEventCreate(&start_event), CCUDAEventGroupImpl);
        CudaSafeCallEx(cudaEventCreate(&end_event), CCUDAEventGroupImpl);

        (CTestBase *)(this)->Log("CCUDAEventGroupImpl create success...");

        int *host_a, *host_b, *host_c;
        int *dev_a, *dev_b, *dev_c;

        CudaSafeCallEx(cudaMalloc((void **)&dev_a, N * sizeof(int)), CCUDAEventGroupImpl);
        CudaSafeCallEx(cudaMalloc((void **)&dev_b, N * sizeof(int)), CCUDAEventGroupImpl);
        CudaSafeCallEx(cudaMalloc((void **)&dev_c, N * sizeof(int)), CCUDAEventGroupImpl);

        CudaSafeCallEx(cudaHostAlloc((void **)&host_a, FULL_DATA_SIZE * sizeof(int), cudaHostAllocDefault), CCUDAEventGroupImpl);
        CudaSafeCallEx(cudaHostAlloc((void **)&host_b, FULL_DATA_SIZE * sizeof(int), cudaHostAllocDefault), CCUDAEventGroupImpl);
        CudaSafeCallEx(cudaHostAlloc((void **)&host_c, FULL_DATA_SIZE * sizeof(int), cudaHostAllocDefault), CCUDAEventGroupImpl);
        if (host_a == nullptr || host_b == nullptr || host_c == nullptr)
        {
            throw CCUDAEventGroupImpl("cudaHostAlloc", "CPU 内存开辟失败!");
        }
        for (int i = 0; i < FULL_DATA_SIZE; i++)
        {
            host_a[i] = i;
            host_b[i] = FULL_DATA_SIZE - i;
        }

        CudaSafeCallEx(cudaEventRecord(start_event), CCUDAEventGroupImpl);
        for (int i = 0; i < FULL_DATA_SIZE; i += N)
        {
            CudaSafeCallEx(cudaMemcpy(dev_a, host_a + i, N * sizeof(int), cudaMemcpyHostToDevice), CCUDAEventGroupImpl);
            CudaSafeCallEx(cudaMemcpy(dev_b, host_b + i, N * sizeof(int), cudaMemcpyHostToDevice), CCUDAEventGroupImpl);

            kernelAdd_event<<<N / 1024, 1024>>>(dev_a, dev_b, dev_c);

            CudaSafeCallEx(cudaMemcpy(host_c + i, dev_c, N * sizeof(int), cudaMemcpyDeviceToHost), CCUDAEventGroupImpl);
        }
        CudaSafeCallEx(cudaEventSynchronize(start_event), CCUDAEventGroupImpl);
        CudaSafeCallEx(cudaEventRecord(end_event), CCUDAEventGroupImpl);

        float time_gpu;
        CudaSafeCallEx(cudaEventElapsedTime(&time_gpu, start_event, end_event), CCUDAEventGroupImpl);

        // 事件计时
        //printf("GPU time: %.3f ms \n", time_gpu);

        (CTestBase *)(this)->Log("CCUDAEventGroupImpl excute success...");

        CudaSafeCallEx(cudaFreeHost(host_a), CCUDAEventGroupImpl);
        CudaSafeCallEx(cudaFreeHost(host_b), CCUDAEventGroupImpl);
        CudaSafeCallEx(cudaFreeHost(host_c), CCUDAEventGroupImpl);

        CudaSafeCallEx(cudaFree(dev_a), CCUDAEventGroupImpl);
        CudaSafeCallEx(cudaFree(dev_b), CCUDAEventGroupImpl);
        CudaSafeCallEx(cudaFree(dev_c), CCUDAEventGroupImpl);

        CudaSafeCallEx(cudaEventDestroy(start_event), CCUDAEventGroupImpl);
        CudaSafeCallEx(cudaEventDestroy(end_event), CCUDAEventGroupImpl);
        (CTestBase *)(this)->Log("CCUDAEventGroupImpl out...");
        //this->PrintLog();
        END_EXCEPTION(CTestBase);
        CudaKernelCheck();
        return true;
    }

    bool CCUDAEventGroupImpl::SetupBenchMark()
    {
        return true;
    }
}
