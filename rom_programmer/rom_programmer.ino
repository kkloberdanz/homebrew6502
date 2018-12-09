/*
 * Author: Kyle Kloberdanz
 * Date: 08 December 2018
 * License: GNU GPLv3
 */


#include <stdint.h>


/*
 * Control pins from the 6502
 */
#define CLOCK 13
#define RESET 12


/*
 * Address lines (lower byte)
 */
#define A0    10
#define A1     9
#define A2     8
#define A3     7
#define A4     6
#define A5     5
#define A6     4
#define A7     3


uint8_t get_data_byte(void) {
  uint8_t bit_0 = digitalRead(A0);
  uint8_t bit_1 = digitalRead(A1);
  uint8_t bit_2 = digitalRead(A2);
  uint8_t bit_3 = digitalRead(A3);
  uint8_t bit_4 = digitalRead(A4);
  uint8_t bit_5 = digitalRead(A5);
  uint8_t bit_6 = digitalRead(A6);
  uint8_t bit_7 = digitalRead(A7);

  uint8_t data =  bit_0       |
                 (bit_1 << 1) |
                 (bit_2 << 2) |
                 (bit_3 << 3) |
                 (bit_4 << 4) |
                 (bit_5 << 5) |
                 (bit_6 << 6) |
                 (bit_7 << 7) ;
  return data;
}


void pulse_clock() {
  digitalWrite(CLOCK, HIGH);
  delay(1);
  digitalWrite(CLOCK, LOW);
}


void set_ready() {
  digitalWrite(RESET, LOW);
  pinMode(RESET, OUTPUT);
  digitalWrite(RESET, LOW);
  for (uint8_t i = 0; i < 7; i++) {
    pulse_clock();
  }
  digitalWrite(RESET, HIGH);
}


void setup() {
  delay(1000);
  Serial.begin(9600);
  Serial.println("*** Initializing ***");
  for (uint8_t i = 3; i <= 10; i++) {
    pinMode(i, INPUT);
  }
  digitalWrite(CLOCK, LOW);
  pinMode(CLOCK, OUTPUT);
  set_ready();
}


void print_lower_address() {
  uint8_t data;
  data = get_data_byte();
  char message[4];
  sprintf(message, "%02x\n", data);
  Serial.print(message);
}


void loop() {

  /* Loop until 6502 address lines are 0000 */
  uint16_t num_loops = 5402;
  while (num_loops > 0) {
    pulse_clock();
    pulse_clock();
    print_lower_address();
    num_loops--;
  }

  /* address lines now point to 0000 */
  Serial.println("*** Back to zero ***");
  delay(10000);

  num_loops = 0xffff;
  while (num_loops) {
    num_loops--;
    pulse_clock();
    pulse_clock();
    print_lower_address();
  }
}
