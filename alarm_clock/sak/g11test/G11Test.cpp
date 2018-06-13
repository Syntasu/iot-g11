#include "Arduino.h"
#include "G11Test.h"

void G11Test::do_test_1()
{
    test_state = 1337;
}

int G11Test::get_test_state()
{
    return test_state;
}

void G11Test::dont_touch_me()
{
    Serial.println("I said don't touch me!");
}