#include <dirent.h>
#include <unistd.h>
#include <string>
#include <vector>
#include <unistd.h>


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
  string os, kernel, line, version;
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

// TODO: Read and return the system memory utilization
float LinuxParser::MemoryUtilization() { 
  std::string key, value;
  float memTotal, memFree;
  std::ifstream file(kProcDirectory + kMeminfoFilename);
  if (file.is_open()) {
    for (std::string line; std::getline(file, line); ){
      std::istringstream stream(line);
      stream >> key >> value;
      if (key == "MemTotal:") {
        memTotal = std::stof(value);
      }
      if (key == "MemFree:") {
        memFree = std::stof(value);
      }      
    }
    return (memTotal - memFree) / memTotal;

  }
  
  return 0.0; 
  }

// TODO: Read and return the system uptime
long LinuxParser::UpTime() { 
  std::string line, uptime, idle;
  std::ifstream file(kProcDirectory + kUptimeFilename);
  if (file.is_open()) {
      std::getline(file, line);
      std::istringstream stream(line);
      stream >> uptime >> idle;   
    return std::stol(uptime);
  }

  return 0; 
}

// TODO: Read and return the number of jiffies for the system
long LinuxParser::Jiffies() { return 0; }

// TODO: Read and return the number of active jiffies for a PID
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::ActiveJiffies(int pid[[maybe_unused]]) { return 0; }

// TODO: Read and return the number of active jiffies for the system
long LinuxParser::ActiveJiffies() { return 0; }

// TODO: Read and return the number of idle jiffies for the system
long LinuxParser::IdleJiffies() { return 0; }

// TODO: Read and return CPU utilization
vector<string> LinuxParser::CpuUtilization() { 
  string line, cpu, user, nice, system, idle, iowait, irq, softirq, steal, guest, guestnice;
  std::ifstream file(kProcDirectory + kStatFilename);
  if (file.is_open()) {
    std::getline(file, line);
    std::istringstream stream(line);
    stream >> cpu >> user >> nice >> system >> idle >> iowait >> irq >> softirq >> steal >> guest >> guestnice;
    return std::vector<string>{user, nice, system, idle, iowait, irq, softirq, steal, guest, guestnice};
  }
  return {}; 
  }

// TODO: Read and return the total number of processes
int LinuxParser::TotalProcesses() { 
  std::string key, value;
  std::ifstream file(kProcDirectory + kStatFilename);
  if (file.is_open()) {
    for (std::string line; std::getline(file, line); ){
      std::istringstream stream(line);
      stream >> key >> value;
      if (key == "processes") {
        return std::stoi(value);
      }
    }
  }
  return 0; 
}

// TODO: Read and return the number of running processes
int LinuxParser::RunningProcesses() { 
  std::string key, value;
  std::ifstream file(kProcDirectory + kStatFilename);
  if (file.is_open()) {
    for (std::string line; std::getline(file, line); ){
      std::istringstream stream(line);
      stream >> key >> value;
      if (key == "procs_running") {
        return std::stoi(value);
      }
    }
  }
  return 0; 
  }

// TODO: Read and return the command associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Command(int pid[[maybe_unused]]) { 
  std::string line, value;
  std::ifstream file(kProcDirectory + std::to_string(pid) + kCmdlineFilename);
  if (file.is_open()) {
    std::getline(file, line);
    std::istringstream stream(line);
    stream >> value;
    return value;
  }  
  return string(); 
  }

// TODO: Read and return the memory used by a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Ram(int pid[[maybe_unused]]) { 
  std::string key, value;
  std::ifstream file(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (file.is_open()) {
    for (std::string line; std::getline(file, line); ){
      std::istringstream stream(line);
      stream >> key >> value;
      if (key == "VmSize:") {
        return value;
      }
    }
  }

  return string(); 
  }

// TODO: Read and return the user ID associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::Uid(int pid[[maybe_unused]]) { 
  std::string key, value;
  std::ifstream file(kProcDirectory + std::to_string(pid) + kStatusFilename);
  if (file.is_open()) {
    for (std::string line; std::getline(file, line); ){
      std::istringstream stream(line);
      stream >> key >> value;
      if (key == "Uid:") {
        return value;
      }
    }
  }
  return string(); 
  }

// TODO: Read and return the user associated with a process
// REMOVE: [[maybe_unused]] once you define the function
string LinuxParser::User(int pid[[maybe_unused]]) { 
  std::string uid = Uid(pid);
  std::string user, x, uidPWD;
  std::ifstream file(kPasswordPath);
  if (file.is_open()) {
    for (std::string line; std::getline(file, line); ){
      std::replace(line.begin(), line.end(), ':', ' ');
      std::istringstream stream(line);
      stream >> user >> x >> uidPWD;
      if (uidPWD == uid) {
        return user;
      }
    }
  }

  return string();
  }

// TODO: Read and return the uptime of a process
// REMOVE: [[maybe_unused]] once you define the function
long LinuxParser::UpTime(int pid[[maybe_unused]]) {
  std::string value, line;
  std::ifstream file(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (file.is_open()) {
    std::getline(file, line);
    std::istringstream stream(line);
    for (int i = 0; i < 22; i++)
    stream >> value;
    return UpTime() - std::stol(value) / sysconf(_SC_CLK_TCK);
    }

  return 0; 
  }

  float LinuxParser::CpuUtil(int pid) {
  /*
  #14 utime - CPU time spent in user code, measured in clock ticks
  #15 stime - CPU time spent in kernel code, measured in clock ticks
  #16 cutime - Waited-for children's CPU time spent in user code (in clock ticks)
  #17 cstime - Waited-for children's CPU time spent in kernel code (in clock ticks)
  #22 starttime - Time when the process started, measured in clock ticks
 */
  std::string line, value, utime, stime, cutime, cstime, starttime;
  std::ifstream file(kProcDirectory + std::to_string(pid) + kStatFilename);
  if (file.is_open()) {
    std::getline(file, line);
    std::istringstream stream(line);
    for (int i = 0; i < 23; i++) {
      stream >> value;
      if (i==13) {
        utime = value;
      }
      if (i==14) {
        stime = value;
      }
      if (i==15) {
        cutime = value;
      }
      if (i==16) {
        cstime = value;
      }
      if (i==21) {
        starttime = value;
      }
    }
  /*
  Calculation
First we determine the total time spent for the process:

total_time = utime + stime
We also have to decide whether we want to include the time from children processes. If we do, then we add those values to total_time:

total_time = total_time + cutime + cstime
Next we get the total elapsed time in seconds since the process started:

seconds = uptime - (starttime / Hertz)
Finally we calculate the CPU usage percentage:

cpu_usage = 100 * ((total_time / Hertz) / seconds)
  */  
    float cpuUsage = ((std::stof(utime) + std::stof(stime) + std::stof(cutime) + std::stof(cstime)) / sysconf(_SC_CLK_TCK)) / (UpTime() - std::stof(starttime) / sysconf(_SC_CLK_TCK));
    return cpuUsage;
    }

  return 0; 
  }
