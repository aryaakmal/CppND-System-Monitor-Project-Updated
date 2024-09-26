#include "processor.h"
#include "linux_parser.h"

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
  std::vector<std::string> x;
  //std::vector<std::string> xp;
  //xp = LinuxParser::CpuUtilization();
  x  = LinuxParser::CpuUtilization();
  
//PrevIdle = previdle + previowait
//Idle = idle + iowait
  //float idlep = std::stof(xp[3]) + std::stof(xp[4]);
  float idle = std::stof(x[3]) + std::stof(x[4]);

//PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal
//NonIdle = user + nice + system + irq + softirq + steal
  //float nonidlep = std::stof(xp[0]) + std::stof(xp[1]) + std::stof(xp[2]) +
  //                 std::stof(xp[5]) + std::stof(xp[6]) + std::stof(xp[7]) ;     
  float nonidle = std::stof(x[0]) + std::stof(x[1]) + std::stof(x[2]) +
                  std::stof(x[5]) + std::stof(x[6]) + std::stof(x[7]) ;

//PrevTotal = PrevIdle + PrevNonIdle
//Total = Idle + NonIdle
  //float totalp = idlep + nonidlep;
  //float total = idle + nonidle;

//# differentiate: actual value minus the previous one
//totald = Total - PrevTotal
//idled = Idle - PrevIdle
  //float totald = total - totalp;
  //float idled = idle - idlep;
                                                                      
//CPU_Percentage = (totald - idled)/totald
  //float cpu_perc = (total - idle)/total;
  //return (totald - idled)/totald;
  return nonidle/(idle+nonidle);
  
}
