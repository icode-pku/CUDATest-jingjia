#include "TestDefine.h"
// #include "CBenchMark_CUDAStreamGroupImpl.h"
#include "CTestBase.h"
#include "benchmark/benchmark.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
namespace TestCUDA
{

   bool CTestBase::RunBenchMarkTest(int argc, char *argv[])
   {
      // benchmark::Initialize(&argc, argv);
      // benchmark::RunSpecifiedBenchmarks();
      // benchmark::Shutdown();
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
            std::cout << m_log_str[i].c_str() << std::endl;
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
            fout << m_log_str[i].c_str() << std::endl;
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
      write(m_pip_flags, result, strlen(result));
      m_error_api.insert(_error);
   }
   std::set<std::string> CTestBase::GetError()
   {
      return std::set<std::string>();//m_error_api;
   }
   bool CTestBase::SetName(const char *_api_name)
   {
      m_api_name.reset(new char[strlen(_api_name) + 1]);
      strcpy(m_api_name.get(), _api_name);
      return true;
   }
   bool CTestBase::SetError(const char *_error)
   {
      m_error.reset(new char[strlen(_error) + 1]);
      strcpy(m_error.get(), _error);
      return true;
   }
   bool CTestBase::SetPipFlags(const int _flags)
   {
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
   bool CTestBase::SetupTest()
   {
      BEGIN_EXCEPTION
      Test_normal();
      Test_NonNormal();
      END_EXCEPTION(CTestBase);
      return true;
   }
   void CTestBase::Add(const std::string &_api_name)
   {
      this->m_success_api.insert(_api_name);
   }
   void CTestBase::SerializeResult(const char *_result_fileName)
   {
      if (m_api_name.get() == nullptr)
      {
         return;
      }
      std::string fileName = std::string(m_api_name.get()) + "_cuda_api_test_result.csv";
      if (_result_fileName != nullptr)
      {
         fileName = std::string(_result_fileName);
      }
      std::ofstream fout(fileName.c_str(), std::ios::out);
      if (fout.fail())
      {
         return;
      }
      fout << "SUCCESS API:" << std::endl;
      std::set<std::string>::iterator iter = this->m_success_api.begin();
      for (; iter != this->m_success_api.end(); iter++)
      {
         fout << iter->c_str() << std::endl;
      }
      fout << "Error API:" << std::endl;
      iter = this->m_error_api.begin();
      for (; iter != this->m_error_api.end(); iter++)
      {
         fout << iter->c_str() << std::endl;
      }
      fout.close();
   }
}