#include "CBenchMarkBase.h"
#include "benchmark/benchmark.h"
#include <iostream>
#include <fstream>
#include <stdio.h>
 bool CBenchMarkBase::RunBenchMarkTest(int argc, char *argv[]){
    benchmark::Initialize(&argc, argv);
    benchmark::RunSpecifiedBenchmarks();
    benchmark::Shutdown();
    return true;
 }

 bool CBenchMarkBase::Log(const char *_logStr){
   m_log_str.push_back(std::string(_logStr));
   return true;
 }

 bool CBenchMarkBase::PrintLog(const char *_logFileName){
   std::ofstream fout(_logFileName, std::ios::out);
   if(fout.fail()){
      return false;
   }
   fout <<m_api_name.c_str()<<std::endl;
   for (size_t i = 0; i < m_log_str.size(); i++)
   {
      fout << m_log_str[i].c_str()<<std::endl;
   }
   
   fout.close();
   return true;
 }

