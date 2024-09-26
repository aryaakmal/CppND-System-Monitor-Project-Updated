#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>

#include "linux_parser.h"

using std::stof;
using std::string;
using std::to_string;
using std::vector;

// DONE: An example of how to read data from the filesystem
string LinuxParser::OperatingSystem() {
  string line;
  string key;
  string value;
  std::ifstream filestream(kOSPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ' ', '_');
      std::replace(line.begin(), line.end(), '=', ' ');
      std::replace(line.begin(), line.end(), '"', ' ');
      std::istringstream linestream(line);
      while (linestream >> key >> value) {
        if (key == "PRETTY_NAME") {
          std::replace(value.begin(), value.end(), '_', ' ');
          return value;
        }
      }
    }
  }
  return value;
}

// DONE: An example of how to read data from the filesystem
string LinuxParser::Kernel() {
  string os, version, kernel;
  string line;
  std::ifstream stream(kProcDirectory + kVersionFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> os >> version >> kernel;
  }
  return kernel;
}

// BONUS: Update this to use std::filesystem
vector<int> LinuxParser::Pids() {
  vector<int> pids;
  DIR* directory = opendir(kProcDirectory.c_str());
  struct dirent* file;
  while ((file = readdir(directory)) != nullptr) {
    // Is this a directory?
    if (file->d_type == DT_DIR) {
      // Is every character of the name a digit?
      string filename(file->d_name);
      if (std::all_of(filename.begin(), filename.end(), isdigit)) {
        int pid = stoi(filename);
        pids.push_back(pid);
      }
    }
  }
  closedir(directory);
  return pids;
}

// DONE: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  string line;
  string token_name;
  string value;
  std::ifstream filestream(kProcDirectory + kMeminfoFilename);
  float memtotal;
  float memfree;
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> token_name >> value) {
        if (token_name == "MemTotal:") {memtotal = std::stof(value);}
        if (token_name == "MemFree:") {memfree = std::stof(value);}
      }
    }
  }
  return (memtotal-memfree)/memtotal;
}

// DONE: Read and return the system uptime
long LinuxParser::UpTime() { 
  string suptime;
  string line;
  long uptime;
  std::ifstream stream(kProcDirectory + kUptimeFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
    std::istringstream linestream(line);
    linestream >> suptime;
  }
  uptime = stol(suptime);
  return uptime;
}

// DONE: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { 
  std::vector<std::string> cpuvalues;
  cpuvalues  = LinuxParser::CpuUtilization();
  long total_jiffies = 0;
  for( auto val : cpuvalues){
    total_jiffies += stol(val);
  }
  return total_jiffies; 
}

// DONE: Read and return the number of active jiffies for a PID
// Note: returns seconds, not cycles (jiffies)
long LinuxParser::ActiveJiffies(int pid) { 
  
  string line;
  string t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, utime, stime, 
         cutime, cstime;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
  }
  std::istringstream linestream(line);
  linestream >> t0 >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10
         >> t11 >> t12 >> utime >> stime >> cutime >> cstime ;
  return (stol(utime) + stol(stime) + stol(cutime) + stol(cstime))/sysconf(_SC_CLK_TCK) ; //convert ticks to seconds
 
  
}

// DONE: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { 
  std::vector<std::string> cpuvalues;
  cpuvalues  = LinuxParser::CpuUtilization();
  long active_jiffies = stol(cpuvalues[0]) + stol(cpuvalues[1]) + stol(cpuvalues[2]) +
                        stol(cpuvalues[5]) + stol(cpuvalues[6]) + stol(cpuvalues[7]);
  return active_jiffies; 
}

// DONE: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { 
  std::vector<std::string> cpuvalues;
  cpuvalues  = LinuxParser::CpuUtilization();
  long idle_jiffies = stol(cpuvalues[3]) + stol(cpuvalues[4]);
  return idle_jiffies;
}

// DONE: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line;
  string name, user, nice, system, idle, iowait, irq, softirq, steal, guest, guest_nice;
  vector<string> cpuinfo;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
     while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> name >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal) {
        if (name == "cpu") {
          cpuinfo = {user, nice, system, idle, iowait, irq, softirq, steal};
        }
      }
     }
  }
  
  //vector<string> v = { "0.25", "0.95" }; //test
  return {cpuinfo}; //test
}

// DONE: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  string line;
  string token_name;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> token_name >> value) {
        if (token_name == "processes") {return std::stoi(value);}
      }
    }
  }
}

// DONE: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  string line;
  string token_name;
  string value;
  std::ifstream filestream(kProcDirectory + kStatFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> token_name >> value) {
        if (token_name == "procs_running") {return std::stoi(value);}
      }
    }
  }
}

// DONE: Read and return the command associated with a process
string LinuxParser::Command(int pid) { 
  
  string line;
  std::ifstream stream(kProcDirectory + to_string(pid) + kCmdlineFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
  }
  
  return line; 
}

// DONE: Read and return the memory used by a process
string LinuxParser::Ram(int pid) { 
  
  string line;
  string name;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> name >> value) {
        if (name == "VmSize:") {return to_string(stol(value)/1024);} //convert kB to MB
      }
    }
  }
  
  //return string(); 
}

// DONE: Read and return the user ID associated with a process
string LinuxParser::Uid(int pid) { 
  
  string line;
  string name;
  string value;
  std::ifstream filestream(kProcDirectory + to_string(pid) + kStatusFilename);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::istringstream linestream(line);
      while (linestream >> name >> value) {
        if (name == "Uid:") {return value;}
      }
    }
  }
}

// DONE: Read and return the user associated with a UID
string LinuxParser::User(int uid) { 
  
  string line;
  string user, x, uidstring;
  std::ifstream filestream(kPasswordPath);
  if (filestream.is_open()) {
    while (std::getline(filestream, line)) {
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream linestream(line);
      while (linestream >> user >> x >> uidstring) {
        if (uidstring == to_string(uid) ) {
          return user;
        }
      }
    }
  }
}

// DONE: Read and return the uptime of a process
// note: value returned is starttime not uptime.
long LinuxParser::UpTime(int pid) { 
  
  string line;
  string t0, t1, t2, t3, t4, t5, t6, t7, t8, t9, t10, t11, t12, t13, t14, 
         t15, t16, t17, t18, t19, t20, t21;
  std::ifstream stream(kProcDirectory + to_string(pid) + kStatFilename);
  if (stream.is_open()) {
    std::getline(stream, line);
  }
  std::istringstream linestream(line);
  linestream >> t0 >> t1 >> t2 >> t3 >> t4 >> t5 >> t6 >> t7 >> t8 >> t9 >> t10
         >> t11 >> t12 >> t13 >> t14 >> t15 >> t16 >> t17 >> t18 >> t19 >> t20 >> t21 ;
    return stol(t21)/sysconf(_SC_CLK_TCK) ; //convert ticks to seconds

}

