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
      m_log_str.push_back(std::string(_logStr));
      return true;
   }

   bool CTestBase::PrintLog(const char *_logFileName)
   {
      if (_logFileName == nullptr)
      {
         std::cout << "*******API Name: " << m_api_name.c_str() << " begin******" << std::endl;
         for (size_t i = 0; i < m_log_str.size(); i++)
         {
            std::cout << m_log_str[i].c_str() << std::endl;
         }
         std::cout << "*******API Name: " << m_api_name.c_str() << " end******" << std::endl;
      }
      else
      {
         std::ofstream fout(_logFileName, std::ios::out);
         if (fout.fail())
         {
            return false;
         }
         fout << m_api_name.c_str() << std::endl;
         for (size_t i = 0; i < m_log_str.size(); i++)
         {
            fout << m_log_str[i].c_str() << std::endl;
         }

         fout.close();
      }

      return true;
   }
   void CTestBase::AddError(const std::string &_error, const int &_pip_flags)
   {
      std::string str = std::to_string(getpid());
      str += std::string(":") + std::string(_error);
      write(_pip_flags, str.c_str(), strlen(str.c_str())); //
      m_error_api.push_back(_error);
   }
   std::vector<std::string> CTestBase::GetError()
   {
      return m_error_api;
   }
}