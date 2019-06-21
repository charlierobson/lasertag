

void initialiseReceiver() {
  cli();

  attachInterrupt(digitalPinToInterrupt(DETECTORPIN), irIsr, CHANGE);

  // initialise Timer1, prescaler 8
  TCCR1A = 0;
  TCCR1B = 0;
  TCCR1B |= (1 << CS11);
  TCNT1 = 0;

  sei();
}



volatile int head = 0;
volatile int tail = 0;
uint16_t ringBuffer[64];

void irIsr() {
  digitalWrite(LEDPIN, HIGH);

  if (digitalRead(DETECTORPIN) == LOW) {
    TCNT1 = 0;
    return;
  }

  ringBuffer[head] = TCNT1;
  head = (head + 1) & 63;
}

bool dataPresent() {
  return head != tail;
}

int dataCount() {
  int entries = head - tail;
  if (entries < 0) entries += 64;
  return entries;
}

uint16_t pop() {
  uint16_t val = ringBuffer[tail];
  tail = (tail + 1) & 63;
  return val;
}


bool playerWasShot()
{
  if(dataCount() < 10) return false;

  Serial.println(pop(), DEC);
  uint16_t data = 0;

  for (int i = 0; i < 8; ++i) {
    data <<= 1;
    int x = pop();
  Serial.println(x, DEC);
    if (x > 2000) {
      data |= 1;
    }
  }

  Serial.println(pop(), DEC);

  return data == B00100001;
}
