/*
 * Bit-Bang UART TX on D8  -> Loopback to D0 (HW RX)
 * HW RX(D0) received data is forwarded to USB Serial (TX)
 *
 * Wiring:
 *   D8  ---- jumper ---->  D0
 *   Scope CH1 -> D8 (optional)
 *
 * Baud: 9600 bps
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
  // Start bit
  TX_LOW();
  bitDelay();

  // Data bits (LSB first)
  for (int i = 0; i < 8; i++) {
    if (data & (1 << i)) TX_HIGH();
    else TX_LOW();
    bitDelay();
  }

  // Stop bit
  TX_HIGH();
  bitDelay();
}

void setup() {
  // D8 output
  DDRB |= (1 << TX_BIT);
  TX_HIGH(); // idle = HIGH

  // USB Serial (HW UART: RX=D0, TX=D1)
  Serial.begin(9600);
}

void loop() {
  // Bit-bang TX -> goes into D0 via jumper
  sendByte('a');
  delay(20);

  // HW RX 받은 걸 다시 USB 시리얼로 출력
  while (Serial.available()) {
    int c = Serial.read();
    Serial.write(c);   // 터미널에 그대로 출력
  }
}
