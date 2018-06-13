#include <G11Test.h>

G11Test test;

void setup() {
  Serial.begin(9600);

  int state = test.get_test_state();

  Serial.print("State before is ");
  Serial.println(state);

  test.do_test_1();

  Serial.print("State after is ");
  Serial.println(state);

}

void loop() {

}
