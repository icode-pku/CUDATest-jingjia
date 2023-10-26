#include "cudaInc.h"
#include "CBenchMark_CUDAStreamCreateImpl.h"
#define N (1024*1024)  
#define FULL_DATA_SIZE N*20  
__global__ void kernelAdd(int* a, int *b, int*c)
{
	int threadID = blockIdx.x * blockDim.x + threadIdx.x;
 
	if (threadID < N)
	{
		c[threadID] = (a[threadID] + b[threadID]) / 2;
	}
}

CBenchMarkBase *CBenchMark_CUDAStreamCreateImpl::CreateSelf()
{
	return new CBenchMark_CUDAStreamCreateImpl();
}
void CBenchMark_CUDAStreamCreateImpl::DestoryPtr(CBenchMarkBase *_ptr){
	if(_ptr != nullptr){
		delete _ptr;
		_ptr = nullptr;
	}
}

bool CBenchMark_CUDAStreamCreateImpl::SetupTest(){
	this->Log("cudaStreamCreate in...");

    cudaStream_t stream;
	CudaSafeCall(cudaStreamCreate(&stream));

	this->Log("cudaStreamCreate create success...");

	int *host_a, *host_b, *host_c;
	int *dev_a, *dev_b, *dev_c;
 
	CudaSafeCall(cudaMalloc((void**)&dev_a, N * sizeof(int)));
	CudaSafeCall(cudaMalloc((void**)&dev_b, N * sizeof(int)));
	CudaSafeCall(cudaMalloc((void**)&dev_c, N * sizeof(int)));

	CudaSafeCall(cudaHostAlloc((void**)&host_a, FULL_DATA_SIZE * sizeof(int), cudaHostAllocDefault));
	CudaSafeCall(cudaHostAlloc((void**)&host_b, FULL_DATA_SIZE * sizeof(int), cudaHostAllocDefault));
	CudaSafeCall(cudaHostAlloc((void**)&host_c, FULL_DATA_SIZE * sizeof(int), cudaHostAllocDefault));
 
	for (int i = 0; i < FULL_DATA_SIZE; i++)
	{
		host_a[i] = i;
		host_b[i] = FULL_DATA_SIZE - i;
	}
 
	for (int i = 0; i < FULL_DATA_SIZE; i += N)
	{
		CudaSafeCall(cudaMemcpyAsync(dev_a, host_a + i, N * sizeof(int), cudaMemcpyHostToDevice, stream));
		CudaSafeCall(cudaMemcpyAsync(dev_b, host_b + i, N * sizeof(int), cudaMemcpyHostToDevice, stream));
 
		kernelAdd << <N / 1024, 1024, 0, stream >> > (dev_a, dev_b, dev_c);
 
		CudaSafeCall(cudaMemcpyAsync(host_c + i, dev_c, N * sizeof(int), cudaMemcpyDeviceToHost, stream));
	}
 
	CudaSafeCall(cudaStreamSynchronize(stream));
	this->Log("cudaStreamCreate excute success...");

	CudaSafeCall(cudaFreeHost(host_a));
	CudaSafeCall(cudaFreeHost(host_b));
	CudaSafeCall(cudaFreeHost(host_c));
 
	CudaSafeCall(cudaFree(dev_a));
	CudaSafeCall(cudaFree(dev_b));
	CudaSafeCall(cudaFree(dev_c));
 
	CudaSafeCall(cudaStreamDestroy(stream));
	this->Log("cudaStreamCreate out...");
    return true;
}

bool CBenchMark_CUDAStreamCreateImpl::SetupBenchMark(){


	return true;
}
bool CBenchMark_CUDAStreamCreateImpl::SetName(const char *_api_name){
	this->m_api_name = std::string(_api_name);
	return true;
}