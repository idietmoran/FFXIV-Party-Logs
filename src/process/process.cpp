#include "process.h"

#include <iostream>

// constructor
Process::Process(std::string processName)
{
    getProcessId(processName);
}


// deconstructor
Process::~Process()
{
    
}

// Gets the process id of the processname
void Process::getProcessId(std::string processName)
{
    hWnd_ = FindWindow(0, processName.c_str());
    GetWindowThreadProcessId(hWnd_, &pid_);
    pHandle_ = OpenProcess(PROCESS_VM_READ, FALSE, pid_);

    if(pHandle_ == NULL)
    {
        std::cout << "FFXIV is not currently running...\n";
        std::cout << "Please restart the program while FFXIV is running.\n";
        Sleep(1300);
        std::cout << "Exiting...\n";
        Sleep(4000);
        exit(0);
    }
}

HANDLE Process::getHandle()
{
    return pHandle_;
}

DWORD Process::getPid()
{
    return pid_;
}