
#include <iostream>
#include <stdio.h>  
#include <math.h>  
#include "cudaInc.h"
#include "CTestCUDA.h"
#include "cuda_runtime.h"
#include "cuda_runtime_api.h"
#include "cuda_device_runtime_api.h"
#include "device_launch_parameters.h"
int main(int argc, char *argv[])
{
    TestCUDA::CTestCUDA::TestValidity();
    TestCUDA::CTestCUDA::TestBenchMark();
    return 0;
}