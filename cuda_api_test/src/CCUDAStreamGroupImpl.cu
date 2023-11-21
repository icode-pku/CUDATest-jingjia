#include "cudaInc.h"
#include <iostream>
#include "CCUDAStreamGroupImpl.h"
#include "CTestCUDAFactory.h"
#define N (1024 * 1024)
#define FULL_DATA_SIZE N * 20
namespace TestCUDA
{
	REGISTER_TEST_OBJECT(CCUDAStreamGroupImpl, CUDA_STREAM_GROUP);
	__global__ void kernelAdd(int *a, int *b, int *c)
	{
		int threadID = blockIdx.x * blockDim.x + threadIdx.x;

		if (threadID < N)
		{
			c[threadID] = (a[threadID] + b[threadID]) / 2;
		}
	}
	void CCUDAStreamGroupImpl::Test_normal()
	{
#ifdef _DEBUG
		LOG_DEBUG("cudaStreamCreate in...");
#endif
		cudaStream_t stream;
		CudaIsSafeCall(cudaStreamCreate(&stream), CCUDAStreamGroupImpl);
#ifdef _DEBUG
		LOG_DEBUG("cudaStreamCreate create success...");
#endif
		int *host_a, *host_b, *host_c;
		int *dev_a, *dev_b, *dev_c;

		CudaIsSafeCall(cudaMalloc((void **)&dev_a, N * sizeof(int)), CCUDAStreamGroupImpl);
		CudaIsSafeCall(cudaMalloc((void **)&dev_b, N * sizeof(int)), CCUDAStreamGroupImpl);
		CudaIsSafeCall(cudaMalloc((void **)&dev_c, N * sizeof(int)), CCUDAStreamGroupImpl);

		CudaIsSafeCall(cudaHostAlloc((void **)&host_a, FULL_DATA_SIZE * sizeof(int), cudaHostAllocDefault), CCUDAStreamGroupImpl);
		CudaIsSafeCall(cudaHostAlloc((void **)&host_b, FULL_DATA_SIZE * sizeof(int), cudaHostAllocDefault), CCUDAStreamGroupImpl);
		CudaIsSafeCall(cudaHostAlloc((void **)&host_c, FULL_DATA_SIZE * sizeof(int), cudaHostAllocDefault), CCUDAStreamGroupImpl);
		if (host_a == nullptr || host_b == nullptr || host_c == nullptr)
		{
			throw CCUDAStreamGroupImpl("cudaHostAlloc", "CPU 内存开辟失败!");
		}
		for (int i = 0; i < FULL_DATA_SIZE; i++)
		{
			host_a[i] = i;
			host_b[i] = FULL_DATA_SIZE - i;
		}
		cudaEvent_t start_event;
		CudaIsSafeCall(cudaEventCreate(&start_event), CCUDAStreamGroupImpl);
		CudaIsSafeCall(cudaEventRecord(start_event), CCUDAStreamGroupImpl);

		for (int i = 0; i < FULL_DATA_SIZE; i += N)
		{
			CudaIsSafeCall(cudaMemcpyAsync(dev_a, host_a + i, N * sizeof(int), cudaMemcpyHostToDevice, stream), CCUDAStreamGroupImpl);
			CudaIsSafeCall(cudaMemcpyAsync(dev_b, host_b + i, N * sizeof(int), cudaMemcpyHostToDevice, stream), CCUDAStreamGroupImpl);

			kernelAdd<<<N / 1024, 1024, 0, stream>>>(dev_a, dev_b, dev_c);

			CudaIsSafeCall(cudaMemcpyAsync(host_c + i, dev_c, N * sizeof(int), cudaMemcpyDeviceToHost, stream), CCUDAStreamGroupImpl);
		}
		CudaIsSafeCall(cudaEventSynchronize(start_event), CCUDAStreamGroupImpl);
		CudaIsSafeCall(cudaStreamSynchronize(stream), CCUDAStreamGroupImpl);
		CudaIsSafeCall(cudaStreamWaitEvent(stream, start_event), CCUDAStreamGroupImpl);
#ifdef _DEBUG
		LOG_DEBUG("cudaStreamCreate excute success...");
#endif
		CudaIsSafeCall(cudaFreeHost(host_a), CCUDAStreamGroupImpl);
		CudaIsSafeCall(cudaFreeHost(host_b), CCUDAStreamGroupImpl);
		CudaIsSafeCall(cudaFreeHost(host_c), CCUDAStreamGroupImpl);

		CudaIsSafeCall(cudaFree(dev_a), CCUDAStreamGroupImpl);
		CudaIsSafeCall(cudaFree(dev_b), CCUDAStreamGroupImpl);
		CudaIsSafeCall(cudaFree(dev_c), CCUDAStreamGroupImpl);

		CudaIsSafeCall(cudaStreamDestroy(stream), CCUDAStreamGroupImpl);
#ifdef _DEBUG
		LOG_DEBUG("cudaStreamCreate out...");
#endif
		CudaKernelCheck();
	}
	void CCUDAStreamGroupImpl::Test_NonNormal()
	{
	}

}
