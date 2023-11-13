#include "TestDefine.h"
//#include "CBenchMark_CUDAStreamGroupImpl.h"
#include "CTestBase.h"
#include "benchmark/benchmark.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
namespace TestCUDA
{
   
   // CTestBase* CTestBase::CreateSelf(const char *_api_name, const int &_pip_flags)
   // {
   //    try
   //      {
   //          bool bRet = true;
   //          std::shared_ptr<TestCUDA::CTestBase> pTestBase = CTestCUDAFactory::Create_shared(_api_name);
   //          if (pTestBase != nullptr)
   //          {
   //              pTestBase->SetupTest();
   //          }
   //          else
   //          {
   //              printf("Invalid object!!\n");
   //          }
   //      }
   //      catch (const std::exception &e)
   //      {
   //          std::cerr << e.what() << '\n';
   //      }
   //    // if (strcmp(_api_name, CUDA_STREAM_GROUP) == 0)
   //    // {
   //    //    return new CBenchMark_CUDAStreamGroupImpl(CUDA_STREAM_GROUP, "", _pip_flags);
   //    // }
   //    // else
   //    // {
   //    //    return nullptr;
   //    // }
   // }
   // void CTestBase::DestoryPtr(CTestBase *_ptr)
   // {
   //    if (_ptr != nullptr)
   //    {
   //       delete _ptr;
   //       _ptr = nullptr;
   //    }
   // }
   bool CTestBase::RunBenchMarkTest(int argc, char *argv[])
   {
      //benchmark::Initialize(&argc, argv);
      //benchmark::RunSpecifiedBenchmarks();
      //benchmark::Shutdown();
      return true;
   }

   bool CTestBase::Log(const char *_logStr)
   {
      m_log_str.push_back(_logStr);
      return true;
   }

   bool CTestBase::PrintLog(const char *_logFileName)
   {
      if (_logFileName == nullptr)
      {
        std::cout << "*******API Name: " << m_api_name << " begin******" << std::endl;
         for (size_t i = 0; i < m_log_str.size(); i++)
         {
            std::cout << m_log_str[i] << std::endl;
         }
         std::cout << "*******API Name: " << m_api_name << " end******" << std::endl;
      }
      else
      {
         std::ofstream fout(_logFileName, std::ios::out);
         if (fout.fail())
         {
            return false;
         }
         fout << m_api_name << std::endl;
         for (size_t i = 0; i < m_log_str.size(); i++)
         {
            fout << m_log_str[i] << std::endl;
         }

         fout.close();
      }

      return true;
   }
   void CTestBase::AddError(const char *_error)
   {
      std::string str = std::to_string(getpid());
      str += std::string(":") + std::string(_error);
      const char *result = strdup(str.c_str());
      printf("m_pip_flags: %d\n",m_pip_flags);
      write(m_pip_flags, result, strlen(result)); 
      m_error_api.push_back(_error);
   }
   std::vector<const char*> CTestBase::GetError()
   {
      return m_error_api;
   }
   bool CTestBase::SetName(const char *_api_name)
	{
      m_api_name.reset(new char [strlen(_api_name) + 1]);
		strcpy(m_api_name.get(), _api_name);
		return true;
	}
   bool CTestBase::SetError(const char *_error){
      m_error.reset(new char [strlen(_error) + 1]);
		strcpy(m_error.get(), _error);
      return true;
   }
   bool CTestBase::SetPipFlags(const int &_flags){
      m_pip_flags = _flags;
   }
	const char *CTestBase::what()
	{
		std::string result = std::string(m_error.get());
		result += ":";
		result += this->m_api_name.get();
		const char *error_ch = strdup(result.c_str());
		return error_ch;
	}
}