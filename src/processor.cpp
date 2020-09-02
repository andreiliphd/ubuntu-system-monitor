#include "processor.h"
#include "linux_parser.h"
#include <iostream>
#include <iterator>

// TODO: Return the aggregate CPU utilization
float Processor::Utilization() { 
    std::vector<std::string> utilizationString = LinuxParser::CpuUtilization();
    std::vector<float> utilization{0.0};
     
/*

PrevIdle = previdle + previowait
Idle = idle + iowait

PrevNonIdle = prevuser + prevnice + prevsystem + previrq + prevsoftirq + prevsteal
NonIdle = user + nice + system + irq + softirq + steal

PrevTotal = PrevIdle + PrevNonIdle
Total = Idle + NonIdle

# differentiate: actual value minus the previous one
totald = Total - PrevTotal
idled = Idle - PrevIdle

CPU_Percentage = (totald - idled)/totald */

    std::transform(utilizationString.begin(), utilizationString.end(), std::back_inserter(utilization), [](const std::string& val)
                 {
                     return std::stof(val);
                 });

    float idle = utilization[3] + utilization[4];
    float nonIdle = utilization[0] + utilization[1] + utilization[2] + utilization[5] + utilization[6] + utilization[7];
    float total = idle + nonIdle;
    return (total - idle) / total;           
    }