/*
 * HC12-RS485.ino
 * 
 * 
 * Compile settings:
 * Board: ATtiny 24/44/84
 * Processor: ATtiny84
 * Clock: Internal 8MHz
 * Programmer: USBtinyISP
 * Port: leave clear
 * 
 * Hardware: Atmel ATTiny84
 * Connections:
 * 1 - VCC
 * 2 - Digital Output - MAX485 DE (Driver enable - active high)
 * 3 - Digital Output - HC12 RXD (transmit data)
 * 4 - not connected (RESET)
 * 5 - Digital Input - HC12 TXD (received data)
 * 6 - Digital Output - HC12 SET (active low = HC12 config mode)
 * 7 - I2C SDA
 * 8 - Digital Output - LED (active low)
 * 9 - I2C SCL
 * 10- Analog Input - Supply voltage, via divider 22k/2k7
 * 11 - Digital - MAX485 DI (Driver input - TX)
 * 12 - Digital Input - MAX485 RO (Receive output - RX)
 * 13 - Digital Output - MAX485 RE (Receive enable - active low)
 * 14 - GND
 * 
 * Functional description:
 * 
 * 
 */

// I/O Pin definitions
#define MAX485_DE_PIN 1 // pin 2 (Digital)
#define HC12_RXD_PIN 9  // pin 3 (Digital)
#define HC12_TXD_PIN 8 // pin 5 (Digital)
#define HC12_SET_PIN 7 // pin 6 (Digital)
//#define x 6 // pin 7 (Digital)
#define LED_PIN 5          // pin 8 (Digital)
//#define x 4 // pin 19 (Digital)
//#define x 3 // pin 10 (Digital)
#define MAX485_DI_PIN 2 // pin 11 (Digital)
#define MAX485_RO_PIN 1 // pin 12 (Digital)
#define MAX485_RE_PIN 0 // pin 13 (Digital)

void setup() {
  // configure I/O pins
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(MAX485_DE_PIN, OUTPUT);
  pinMode(MAX485_DI_PIN, OUTPUT);
  pinMode(MAX485_RE_PIN, OUTPUT);
  pinMode(MAX485_RO_PIN, INPUT);

  digitalWrite(MAX485_DE_PIN, LOW); // disable driver
  digitalWrite(MAX485_RE_PIN, LOW); // enable receive (active low)
  
  pinMode(HC12_SET_PIN, OUTPUT);
  pinMode(HC12_RXD_PIN, OUTPUT);
  pinMode(HC12_TXD_PIN, INPUT);

  digitalWrite(HC12_RXD_PIN, LOW);
  digitalWrite(HC12_SET_PIN, HIGH); // active low
  
  startup_flash();
}

void startup_flash() {
  for (int i = 20; i > 0; i--) {
    delay(100);
    digitalWrite(LED_PIN, LOW);
    delay(100);
    digitalWrite(LED_PIN, HIGH);
  }
}

void loop() {
  digitalWrite(HC12_RXD_PIN, digitalRead(MAX485_RO_PIN));
  digitalWrite(MAX485_DI_PIN , digitalRead(HC12_TXD_PIN));
  // debug
  digitalWrite(LED_PIN, digitalRead(HC12_TXD_PIN));
}
