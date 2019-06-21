const byte LED = 11;  // Timer 2 "A" output: OC2A

void setup() {
  pinMode (LED, OUTPUT);

  // set up Timer 2
  TCCR2A = _BV (COM2A0) | _BV(WGM21);  // CTC, toggle OC2A on Compare Match
  TCCR2B = _BV (CS20);   // No prescaler
  OCR2A =  209;          // compare A register value (210 * clock speed)
  //  = 13.125 nS , so frequency is 1 / (2 * 13.125) = 38095
  Serial.begin(9600);
}  // end of setup

uint8_t mode = OUTPUT;
void loop() {
  if (Serial.available()) {
    if (Serial.read() == 'x') {
      mode = (mode == OUTPUT) ? INPUT: OUTPUT;
      pinMode(LED, mode);
    }
  }
}
