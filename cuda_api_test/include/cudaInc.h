#include <stdio.h>
#include <stdexcept>
#include "cuda_runtime.h"
#include "cuda_runtime_api.h"
#include "cuda_device_runtime_api.h"
#include "device_launch_parameters.h"
#define CudaSafeCall(error) TestCUDA::cuda_safe_call(error, __FILE__, __LINE__)
#define CudaKernelCheck() CudaSafeCall(cudaGetLastError())
namespace TestCUDA{
    inline void cuda_safe_call(cudaError error, const char *file, const int line)
		{
			if (error != cudaSuccess) {
				fprintf(stderr, "cuda error %s : %d %s\n", file, line, cudaGetErrorString(error));
				exit(-1);
			}
		}

}