
#include <Windows.h>
#include <iostream>
#include <sstream>      // std::ostringstream

#include "Timing.h"

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN		// Exclude rarely-used stuff from Windows headers
#endif

#include <time.h>		// for clock support
#include <iomanip>

using namespace std;

#ifndef CLOCKS_PER_SEC
#define CLOCKS_PER_SEC 60
#define CLOCKS_PER_SEC 60
/* the second one is there to generate a warning message */
#endif

Clock::Clock(char *n) {
    name = n;
    count = 0;
    total = 0;
    min = 0;
    max = 0;
    freq = 0;
    etime = 0;
    btime = 0;
}

void Clock::start() {
    if (!QueryPerformanceFrequency(reinterpret_cast<LARGE_INTEGER *>(&freq))) freq=0;

    if (freq)
        QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&btime));
    else
        btime = clock();
}

void Clock::stop() {
    if (freq)
        QueryPerformanceCounter(reinterpret_cast<LARGE_INTEGER *>(&etime));
    else
        etime = clock();

    _int64 tdiff = etime-btime;
    count++;
    total += tdiff;
    if (!min || (tdiff < min)) min = tdiff;
    if (tdiff > max) max=tdiff;
}

void Clock::reset() {
    count = 0;
    total = 0;
    btime = 0;
    max = 0;
    min = 0;
}

void Clock::print() {
    std::cout << (toString()) << std::endl;
}

double Clock::get() {
	if (total == 0) return 0;

    if (freq) return ((double) total)/((double) (freq*count));
    else return ((double) total)/((double) (CLOCKS_PER_SEC*count));
}

std::string Clock::toString() 
{
	std::ostringstream stringStream;
    double aveTime, bestTime, worstTime;
    if (freq) {
        aveTime = (count>0) ? ((double) total)/((double) (freq*count)) : 0;
        bestTime = (count>0) ? ((double) min)/((double) (freq)) : 0;
        worstTime = (count>0) ? ((double) max)/((double) (freq)) : 0;
    } else {
        aveTime = (count>0) ? ((double) total)/((double) (CLOCKS_PER_SEC*count)) : 0;
        bestTime = (count>0) ? ((double) min)/((double) (CLOCKS_PER_SEC)) : 0;
        worstTime = (count>0) ? ((double) max)/((double) (CLOCKS_PER_SEC)) : 0;
    }

	if (count > 1) 
		stringStream << "Clock <" << name << "> " << total << " ticks / " << count << " times = " << aveTime <<
			" sec per (" << setprecision(3) << bestTime << " - " << worstTime << ")";
	else
		stringStream << "Clock <" << name << "> " << total << " ticks / " << count << " times = " << aveTime << " ";
    return stringStream.str();
}

std::ostream& operator<<(std::ostream& os, const Clock& clock) {

    if(clock.count > 0) {
        float freq = (float) (clock.freq ? clock.freq : CLOCKS_PER_SEC);
        os << ((float)clock.total/(freq*(float)clock.count)) << "s";
    } else {
        os << "0s";
    }
	
    return os;
}
