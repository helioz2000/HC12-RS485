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
 * link HC12 RX/TX to MAX485 RX/TX and control the MAX485 driver/receive enable signals
 * this implementation is only suitable for slow baudrates 
 */
 

// I/O Pin definitions
#define MAX485_DE_PIN 10 // pin 2 (Digital)
#define HC12_RXD_PIN 9  // pin 3 (Digital)
#define HC12_TXD_PIN 8 // pin 5 (Digital)
#define HC12_SET_PIN 7 // pin 6 (Digital)
//#define x 6 // pin 7 (Digital)
#define LED_PIN 5          // pin 8 (Digital)
//#define x 4 // pin 19 (Digital)
#define TOGGLE_PIN 3 // pin 10 (Digital)
#define MAX485_DI_PIN 2 // pin 11 (Digital)
#define MAX485_RO_PIN 1 // pin 12 (Digital)
#define MAX485_RE_PIN 0 // pin 13 (Digital)

bool hc12_tx_state;
bool max485_tx_mode = false;
bool toggle_state;

// 25000/1200(HC12_BAUD) =  20.83ms; // 1T * 2.5 = T2.5
// add extra 15ms 
#define TX_MODE_OFF_DELAY_TIME 35   // ms
unsigned long tx_mode_off_time;

void setup() {
  // configure I/O pins
  pinMode(LED_PIN, OUTPUT);
  digitalWrite(LED_PIN, LOW);

  pinMode(TOGGLE_PIN, OUTPUT);

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
  
  hc12_tx_state = digitalRead(HC12_TXD_PIN);  // low on data tx 

  // is the HC12 sending data?
  if (!hc12_tx_state) {
    max485_tx_mode = HIGH;    // switch MAX485 into TX mode
    tx_mode_off_time = millis() + TX_MODE_OFF_DELAY_TIME;  // calculate timeout for MAX485 TX mode
  } else {
    if (millis() >= tx_mode_off_time) {   // has MAX485 TX timout expired?
      max485_tx_mode = LOW;
    }
  }
  // MAX485 RX/TX mode
  digitalWrite(MAX485_DE_PIN, max485_tx_mode); // enable MAX485 driver
  digitalWrite(MAX485_RE_PIN, max485_tx_mode); // disable MAX485 receive (active low)

  // echo RX/TX signal on relevant pins
  //digitalWrite(HC12_RXD_PIN, digitalRead(MAX485_RO_PIN));
  digitalWrite(MAX485_DI_PIN, hc12_tx_state);

  // visual indication of data on LED
  digitalWrite(LED_PIN, !hc12_tx_state);
  //digitalWrite(LED_PIN, !max485_tx_mode);
  
  // for cycle time measurements with scope
  toggle_state = !toggle_state;
  digitalWrite(TOGGLE_PIN, toggle_state);
  
}
