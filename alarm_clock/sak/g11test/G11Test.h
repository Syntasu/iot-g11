//Prevent double header inclusion.
#ifndef G11TEST_H
#define G11TEST_H

#include "Arduino.h"

class G11Test
{
public:
    void do_test_1();
    int get_test_state();

private:
    int test_state = 0;
    void dont_touch_me();
};

#endif