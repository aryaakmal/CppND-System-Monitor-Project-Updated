#include <unistd.h>
#include <cctype>
#include <sstream>
#include <string>
#include <vector>

#include "process.h"
#include "linux_parser.h"

using std::string;
using std::to_string;
using std::vector;

// Constructor for Process Class
Process::Process(int pid) : pid_(pid) {};

// DONE: Return this process's ID
int Process::Pid() { return pid_; }

// DONE: Return this process's CPU utilization
float Process::CpuUtilization() { 
  float active_time = float(LinuxParser::ActiveJiffies(pid_));  //note: seconds not jiffies.
  float total_time = float(LinuxParser::UpTime() - LinuxParser::UpTime(pid_));
  cpuUtil_ = active_time/total_time;
  //return active_time/total_time;
  return cpuUtil_;
}

// DONE: Return the command that generated this process
string Process::Command() { 
  return LinuxParser::Command(pid_); 
}

// DONE: Return this process's memory utilization
string Process::Ram() { 
  return LinuxParser::Ram(pid_); 
}

// DONE: Return the user (name) that generated this process
string Process::User() { 
  int uid = std::stoi(LinuxParser::Uid(pid_));
  return LinuxParser::User(uid); 
}

// DONE: Return the age of this process (in seconds)
long int Process::UpTime() { 
  return LinuxParser::UpTime() - LinuxParser::UpTime(pid_); 
}

// DONE: Overload the "less than" comparison operator for Process objects

//bool Process::operator<(Process & a) {return CpuUtilization()  < a.CpuUtilization(); }
bool Process::operator<(Process & a) { return cpuUtil_ < a.cpuUtil_;}
