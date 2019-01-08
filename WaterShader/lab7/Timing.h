#pragma once


#include <iostream>

/**
 * \brief A simple class for timing things.
 */
class  Clock {
public:
    Clock(char *n = "UnNamed");
#ifndef SWIG
	/** Print the results. */
    void print();
#endif
    /** Start the timer. */
    void start();
    /** Stop the timer. */
    void stop();
    /** Reset timer to 0. */
    void reset();
    /** # of starts. */
    int count;
    /** get elapsed time. */
    double get();

    // use 64 bit ints to support the high res timer
    _int64 total;		// time counts 
    _int64 btime;		// for times command
    _int64 etime;
    _int64 freq;	    // 0 if the high res timer isn't available
    char *name;
    _int64 max,min;		// keep track of these too!

    std::string toString();


    friend std::ostream& operator<<(std::ostream&, const Clock&);

};

// unfortunately, AutoTimer no longer works - compilers destroy things too quickly!
