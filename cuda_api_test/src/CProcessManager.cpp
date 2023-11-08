#include "CProcessManager.h"
void CProcessManager::AddProcess(const char *_process_desc, const int &_pid){
    this->m_process_map.insert(std::make_pair(_process_desc, _pid));
}