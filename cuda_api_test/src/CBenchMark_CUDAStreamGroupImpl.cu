#include "cudaInc.h"
#include <iostream>
#include "CBenchMark_CUDAStreamGroupImpl.h"
#define N (1024 * 1024)
#define FULL_DATA_SIZE N * 20
namespace TestCUDA
{
	__global__ void kernelAdd(int *a, int *b, int *c)
	{
		int threadID = blockIdx.x * blockDim.x + threadIdx.x;

		if (threadID < N)
		{
			c[threadID] = (a[threadID] + b[threadID]) / 2;
		}
	}

	bool CBenchMark_CUDAStreamGroupImpl::SetupTest() 
	{
		try
		{
			(CTestBase *)(this)->Log("cudaStreamCreate in...");

			cudaStream_t stream;
			CudaSafeCallEx(cudaStreamCreate(&stream), CBenchMark_CUDAStreamGroupImpl);

			(CTestBase *)(this)->Log("cudaStreamCreate create success...");

			int *host_a, *host_b, *host_c;
			int *dev_a, *dev_b, *dev_c;

			CudaSafeCallEx(cudaMalloc((void **)&dev_a, -1 * sizeof(int)), CBenchMark_CUDAStreamGroupImpl);
			CudaSafeCallEx(cudaMalloc((void **)&dev_b, N * sizeof(int)), CBenchMark_CUDAStreamGroupImpl);
			CudaSafeCallEx(cudaMalloc((void **)&dev_c, N * sizeof(int)), CBenchMark_CUDAStreamGroupImpl);

			CudaSafeCallEx(cudaHostAlloc((void **)&host_a, FULL_DATA_SIZE * sizeof(int), cudaHostAllocDefault), CBenchMark_CUDAStreamGroupImpl);
			CudaSafeCallEx(cudaHostAlloc((void **)&host_b, FULL_DATA_SIZE * sizeof(int), cudaHostAllocDefault), CBenchMark_CUDAStreamGroupImpl);
			CudaSafeCallEx(cudaHostAlloc((void **)&host_c, FULL_DATA_SIZE * sizeof(int), cudaHostAllocDefault), CBenchMark_CUDAStreamGroupImpl);
			if(host_a == nullptr || host_b == nullptr || host_c == nullptr){
				throw CBenchMark_CUDAStreamGroupImpl("cudaHostAlloc", "CPU 内存开辟失败!");
			}
			for (int i = 0; i < FULL_DATA_SIZE; i++)
			{
				host_a[i] = i;
				host_b[i] = FULL_DATA_SIZE - i;
			}

			for (int i = 0; i < FULL_DATA_SIZE; i += N)
			{
				CudaSafeCallEx(cudaMemcpyAsync(dev_a, host_a + i, N * sizeof(int), cudaMemcpyHostToDevice, stream), CBenchMark_CUDAStreamGroupImpl);
				CudaSafeCallEx(cudaMemcpyAsync(dev_b, host_b + i, N * sizeof(int), cudaMemcpyHostToDevice, stream), CBenchMark_CUDAStreamGroupImpl);

				kernelAdd<<<N / 1024, 1024, 0, stream>>>(dev_a, dev_b, dev_c);

				CudaSafeCallEx(cudaMemcpyAsync(host_c + i, dev_c, N * sizeof(int), cudaMemcpyDeviceToHost, stream), CBenchMark_CUDAStreamGroupImpl);
			}

			CudaSafeCallEx(cudaStreamSynchronize(stream), CBenchMark_CUDAStreamGroupImpl);
			(CTestBase *)(this)->Log("cudaStreamCreate excute success...");

			CudaSafeCallEx(cudaFreeHost(host_a), CBenchMark_CUDAStreamGroupImpl);
			CudaSafeCallEx(cudaFreeHost(host_b), CBenchMark_CUDAStreamGroupImpl);
			CudaSafeCallEx(cudaFreeHost(host_c), CBenchMark_CUDAStreamGroupImpl);

			CudaSafeCallEx(cudaFree(dev_a), CBenchMark_CUDAStreamGroupImpl);
			CudaSafeCallEx(cudaFree(dev_b), CBenchMark_CUDAStreamGroupImpl);
			CudaSafeCallEx(cudaFree(dev_c), CBenchMark_CUDAStreamGroupImpl);

			CudaSafeCallEx(cudaStreamDestroy(stream), CBenchMark_CUDAStreamGroupImpl);
			(CTestBase *)(this)->Log("cudaStreamCreate out...");
			this->PrintLog();
		}
		catch (CBenchMark_CUDAStreamGroupImpl &err)
		{
			(CTestBase *)(this)->Log(err.what());
			this->AddError(err.what(), this->m_pip_flags);
		}
		catch(...){
			this->AddError("CUDA_STREAM_GROUP exception!!", this->m_pip_flags);
		}
		//CudaKernelCheck(CBenchMark_CUDAStreamGroupImpl);
		return true;
	}

	bool CBenchMark_CUDAStreamGroupImpl::SetupBenchMark()
	{

		return true;
	}
	bool CBenchMark_CUDAStreamGroupImpl::SetName(const char *_api_name)
	{
		this->m_api_name = std::string(_api_name);
		return true;
	}
	const char *CBenchMark_CUDAStreamGroupImpl::what(){
		return m_error.c_str();
	}
}