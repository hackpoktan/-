/*
 * SP01 - UART Bit Waveform Observation
 * Board: Arduino Uno
 * TX Pin: D8
 * Baud: 9600 bps
 * Data: 'a'
 */

#define UART_TX 8
#define UART_BAUD 9600
#define BIT_US (1000000UL / UART_BAUD)

void uartSendChar(char data) {
  // Start bit
  digitalWrite(UART_TX, LOW);
  delayMicroseconds(BIT_US);

  // Data bits (LSB first)
  for (uint8_t bit = 0; bit < 8; bit++) {
    if (data & (1 << bit))
      digitalWrite(UART_TX, HIGH);
    else
      digitalWrite(UART_TX, LOW);

    delayMicroseconds(BIT_US);
  }

  // Stop bit
  digitalWrite(UART_TX, HIGH);
  delayMicroseconds(BIT_US);
}

void setup() {
  pinMode(UART_TX, OUTPUT);
  digitalWrite(UART_TX, HIGH);   // Idle state

  Serial.begin(9600);
  Serial.println("[SP01] UART waveform test started");
  Serial.print("Baud rate: ");
  Serial.println(UART_BAUD);
  Serial.print("Bit time (us): ");
  Serial.println(BIT_US);

  delay(1500); // scope 준비 시간
}

void loop() {
  uartSendChar('a');
  delay(400);  // 반복 간격
}
