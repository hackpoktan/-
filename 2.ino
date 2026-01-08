const unsigned long BAUD = 9600;
const unsigned int BIT_US = 1000000UL / BAUD;

// D8 = PORTB bit 0
const uint8_t TX_MASK = (1 << 0);

unsigned long tStart, tEnd, count = 0;

// ---- Low level pin control ----
inline void txHigh() {
  PORTB |= TX_MASK;
}

inline void txLow() {
  PORTB &= ~TX_MASK;
}

inline void sendBit(bool level) {
  if (level) txHigh();
  else txLow();
  delayMicroseconds(BIT_US);
}

// ---- UART frame send ----
void sendUART(uint8_t data) {
  // start bit
  sendBit(false);

  // data bits (LSB first)
  uint8_t mask = 0x01;
  for (uint8_t i = 0; i < 8; i++) {
    sendBit(data & mask);
    mask <<= 1;
  }

  // stop bit
  sendBit(true);
}

void setup() {
  // set D8 output
  DDRB |= TX_MASK;
  txHigh(); // idle

  Serial.begin(9600);
  Serial.println("=== SP/02 ALT Bit-Bang UART ===");
  Serial.print("Baud: "); Serial.println(BAUD);
  Serial.print("Bit time(us): "); Serial.println(BIT_US);
  delay(2000);
}

void loop() {
  count++;
  Serial.print("Test ");
  Serial.print(count);
  Serial.print(": ");

  tStart = micros();
  sendUART('a');
  tEnd = micros();

  Serial.print("Tx time = ");
  Serial.print(tEnd - tStart);
  Serial.println(" us");

  delay(500);
}
