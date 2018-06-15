#ifndef G11UTIL_H
#define G11UTIL_H

#include "Arduino.h"

class G11Util
{
public:
    void timed_delay(int);
    void virtual_delay(int);
    int get_total_delay(bool);

private:
    int delayed = 0;
};

#endif