#ifndef PERFOMANCETIMER_H
#define PERFOMANCETIMER_H

#include <chrono>
#include <string>
class PerfomanceTimer
{
public:
    PerfomanceTimer();
    PerfomanceTimer(std::string);
    ~PerfomanceTimer();
    double getTimeNS();
    std::string name;
    std::chrono::high_resolution_clock::time_point start,end;
};

#endif // PERFOMANCETIMER_H
