volatile int head = 0;
volatile int tail = 0;
uint16_t ringBuffer[64];

extern unsigned long ledOffTime;

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

void irIsr();

class Receiver
{
  public:
    static const int IRRECEIVERPIN = 3;

    Receiver() {
    }

    void begin() {
      attachInterrupt(digitalPinToInterrupt(IRRECEIVERPIN), irIsr, CHANGE);

      // initialise Timer1, prescaler 8
      TCCR1A = 0;
      TCCR1B = 0;
      TCCR1B |= (1 << CS11);
      TCNT1 = 0;
    }

    void update() {
    }

    bool detectedHit() {
      if (!dataPresent()) return false;

      uint16_t data = pop();
      uint16_t command = data >> 10;
      uint16_t payload = (data & 1023) >> 1;

      if (command == SHOT) {
        uint16_t team = payload >> 4;
        uint16_t player = payload & 15;
        Serial.print("shot by player ");
        Serial.print(player, DEC);
        Serial.print(" of team ");
        Serial.println(team, DEC);
        digitalWrite(5, HIGH);
        ledOffTime = millis() + 250;
      }

      return true;
    }
};

volatile uint16_t receivedValue;
volatile int receivedBits;
volatile int oneBits;

void irIsr() {
  if (digitalRead(Receiver::IRRECEIVERPIN) == LOW) {
    TCNT1 = 0;
    return;
  }

  uint16_t pulseLength = TCNT1;
  uint16_t rxBit = 0;

  if (pulseLength > 4000) {
    receivedValue = 0;
    receivedBits = 0;
    oneBits = 0;
    return;
  }
  else if (pulseLength > 2000) {
    rxBit = 1;
    ++oneBits;
  }

  receivedValue = (receivedValue << 1) | rxBit;
  ++receivedBits;
  if (receivedBits == 16) {
    if ((oneBits & 1) == 0) {
      ringBuffer[head] = receivedValue;
      head = (head + 1) & 63;
    }
  }
}
