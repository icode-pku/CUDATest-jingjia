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
		(CTestBase *)(this)->Log("cudaStreamCreate in...");

		cudaStream_t stream;
		CudaSafeCallEx(cudaStreamCreate(&stream), CCUDAStreamGroupImpl);

		(CTestBase *)(this)->Log("cudaStreamCreate create success...");

		int *host_a, *host_b, *host_c;
		int *dev_a, *dev_b, *dev_c;

		CudaSafeCallEx(cudaMalloc((void **)&dev_a, N * sizeof(int)), CCUDAStreamGroupImpl);
		CudaSafeCallEx(cudaMalloc((void **)&dev_b, N * sizeof(int)), CCUDAStreamGroupImpl);
		CudaSafeCallEx(cudaMalloc((void **)&dev_c, N * sizeof(int)), CCUDAStreamGroupImpl);

		CudaSafeCallEx(cudaHostAlloc((void **)&host_a, FULL_DATA_SIZE * sizeof(int), cudaHostAllocDefault), CCUDAStreamGroupImpl);
		CudaSafeCallEx(cudaHostAlloc((void **)&host_b, FULL_DATA_SIZE * sizeof(int), cudaHostAllocDefault), CCUDAStreamGroupImpl);
		CudaSafeCallEx(cudaHostAlloc((void **)&host_c, FULL_DATA_SIZE * sizeof(int), cudaHostAllocDefault), CCUDAStreamGroupImpl);
		if (host_a == nullptr || host_b == nullptr || host_c == nullptr)
		{
			throw CCUDAStreamGroupImpl("cudaHostAlloc", "CPU 内存开辟失败!");
		}
		for (int i = 0; i < FULL_DATA_SIZE; i++)
		{
			host_a[i] = i;
			host_b[i] = FULL_DATA_SIZE - i;
		}

		for (int i = 0; i < FULL_DATA_SIZE; i += N)
		{
			CudaSafeCallEx(cudaMemcpyAsync(dev_a, host_a + i, N * sizeof(int), cudaMemcpyHostToDevice, stream), CCUDAStreamGroupImpl);
			CudaSafeCallEx(cudaMemcpyAsync(dev_b, host_b + i, N * sizeof(int), cudaMemcpyHostToDevice, stream), CCUDAStreamGroupImpl);

			kernelAdd<<<N / 1024, 1024, 0, stream>>>(dev_a, dev_b, dev_c);

			CudaSafeCallEx(cudaMemcpyAsync(host_c + i, dev_c, N * sizeof(int), cudaMemcpyDeviceToHost, stream), CCUDAStreamGroupImpl);
		}

		CudaSafeCallEx(cudaStreamSynchronize(stream), CCUDAStreamGroupImpl);
		(CTestBase *)(this)->Log("cudaStreamCreate excute success...");

		CudaSafeCallEx(cudaFreeHost(host_a), CCUDAStreamGroupImpl);
		CudaSafeCallEx(cudaFreeHost(host_b), CCUDAStreamGroupImpl);
		CudaSafeCallEx(cudaFreeHost(host_c), CCUDAStreamGroupImpl);

		CudaSafeCallEx(cudaFree(dev_a), CCUDAStreamGroupImpl);
		CudaSafeCallEx(cudaFree(dev_b), CCUDAStreamGroupImpl);
		CudaSafeCallEx(cudaFree(dev_c), CCUDAStreamGroupImpl);

		CudaSafeCallEx(cudaStreamDestroy(stream), CCUDAStreamGroupImpl);
		(CTestBase *)(this)->Log("cudaStreamCreate out...");
		// this->PrintLog();
		CudaKernelCheck();
	}
	void CCUDAStreamGroupImpl::Test_NonNormal(){

	}
	bool CCUDAStreamGroupImpl::SetupTest()
	{
		BEGIN_EXCEPTION
		Test_normal();
		Test_NonNormal();
		END_EXCEPTION(CTestBase);
		return true;
	}

	bool CCUDAStreamGroupImpl::SetupBenchMark()
	{

		return true;
	}

}
