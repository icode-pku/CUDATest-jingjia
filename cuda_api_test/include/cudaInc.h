#pragma once
#include <stdio.h>
#include <map>
#include <string.h>
#include <functional>
#include <stdexcept>
#include "cuda_runtime.h"
#include "cuda_runtime_api.h"
#include "cuda_device_runtime_api.h"
#include "device_launch_parameters.h"
#include "driver_types.h"
#include <pthread.h>
#include <signal.h>
// extern std::map<std::string, std::string> g_error_map;
//  #define CudaSafeCall(error) TestCUDA::cuda_safe_call(error, __FILE__, __LINE__)
#define CudaSafeCall(error, T) TestCUDA::cuda_safe_call<T>(error, __FILE__, __LINE__, #error)

#define CudaSafeCallEx(call, T) TestCUDA::cuda_safe_callx<T>([&] { return (call); }, __FILE__, __LINE__, #call)

#define CudaKernelCheck(T) CudaSafeCall(cudaGetLastError(), T)

namespace TestCUDA
{

	template <typename T>
	inline void cuda_safe_call(cudaError error, const char *file, const int line, const char *_code_str)
	{

		if (error != cudaSuccess)
		{
			fprintf(stderr, "cuda error %s : %d %s, api: %s \n", file, line, cudaGetErrorString(error), _code_str);
			std::string error_code(_code_str);
			std::string sub_str = error_code.substr(0, error_code.find_first_of("("));
			std::string result_str = sub_str + " " + cudaGetErrorString(error);
			T t(sub_str.c_str(), result_str.c_str());
			throw t;
		}
	}
	template <typename T, class Fn>
	struct FuncPara
	{
		std::function<cudaError_t()> call;
		pthread_cond_t *g_cond;
		pthread_mutex_t *g_mutex;
		std::string code_str;
		cudaError_t error;
	};

	template <typename T, class Fn>
	inline void *CallFunc(void *_arg)
	{
		FuncPara<T, Fn> *para = (FuncPara<T, Fn> *)(_arg);
		para->error = (cudaError_t)para->call();

		// ·¢½áÊøÐÅºÅ;
		pthread_mutex_lock(para->g_mutex);
		pthread_cond_signal(para->g_cond);
		pthread_mutex_unlock(para->g_mutex);
	}

	static long long tm_to_ns(struct timespec tm)
	{
		return tm.tv_sec * 1000000000 + tm.tv_nsec;
	}

	static struct timespec ns_to_tm(long long ns)
	{
		struct timespec tm;
		tm.tv_sec = ns / 1000000000;
		tm.tv_nsec = ns - (tm.tv_sec * 1000000000);
		return tm;
	}
	
	template <typename T, class Fn>
	inline void cuda_safe_callx(Fn &&call, const char *file, const int line, const char *_code_str)
	{
		pthread_cond_t g_cond;
		pthread_mutex_t g_mutex;

		pthread_cond_init(&g_cond, NULL);
		pthread_mutex_init(&g_mutex, NULL);

		std::function fn(call);
		TestCUDA::FuncPara<T, Fn> para{fn, &g_cond, &g_mutex, std::string(_code_str)};

		pthread_t threadID = 0;
		int err = pthread_create(&threadID, nullptr, CallFunc<T, Fn>, (void *)&para);
		pthread_detach(threadID);

		struct timespec start_tm;
		struct timespec end_tm;
		int timeout_ms = 0;

		clock_gettime(CLOCK_REALTIME, &start_tm);
		end_tm = ns_to_tm(tm_to_ns(start_tm) + timeout_ms * 1000000);

		pthread_mutex_lock(&g_mutex);
		if (pthread_cond_timedwait(&g_cond, &g_mutex, &end_tm) == ETIMEDOUT)
		{
			pthread_mutex_unlock(&g_mutex);
			pthread_cancel(threadID);
			void *return_str = nullptr;
			pthread_join(threadID, &return_str);
			pthread_cond_destroy(&g_cond);
			pthread_mutex_destroy(&g_mutex);
			std::string error_code(_code_str);
			std::string sub_str = error_code.substr(0, error_code.find_first_of("("));
			T t(sub_str.c_str(), "connect time out!");
			throw t;
		}
		else
		{
			pthread_mutex_unlock(&g_mutex);
			pthread_cond_destroy(&g_cond);
			pthread_mutex_destroy(&g_mutex);
			if (para.error != cudaSuccess)
			{
				std::string error_code(_code_str);
				std::string sub_str = error_code.substr(0, error_code.find_first_of("("));
				std::string result_str = sub_str + " " + cudaGetErrorString(para.error);
				T t(sub_str.c_str(), result_str.c_str());
				throw t;
			}
		}

		
	}
}