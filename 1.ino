/*
 * Bit-Bang UART TX on D8 (PORTB0)
 * Send character 'a' repeatedly at 9600 bps
 * Observe waveform on Oscilloscope CH1 -> D8
 */

#define BAUD_RATE 9600
#define BIT_DELAY_US (1000000UL / BAUD_RATE) // ~104 us

// D8 = PORTB bit 0
#define TX_BIT 0

#define TX_HIGH() (PORTB |= (1 << TX_BIT))
#define TX_LOW()  (PORTB &= ~(1 << TX_BIT))

void bitDelay() {
  delayMicroseconds(BIT_DELAY_US);
}

void sendByte(uint8_t data) {
  // Start bit (LOW)
  TX_LOW();
  bitDelay();

  // Data bits (LSB first)
  for (int i = 0; i < 8; i++) {
    if (data & (1 << i)) TX_HIGH();
    else TX_LOW();
    bitDelay();
  }

  // Stop bit (HIGH)
  TX_HIGH();
  bitDelay();
}

void setup() {
  // Set D8 (PORTB0) as output
  DDRB |= (1 << TX_BIT);

  // Idle state = HIGH
  TX_HIGH();
}

void loop() {
  sendByte('a');      // 0x61
  delay(10);          // 간격 주기 (파형 구분용)
}
