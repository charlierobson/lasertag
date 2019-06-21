
volatile int head = 0;
volatile int tail = 0;
uint16_t ringBuffer[64];


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
      if (dataCount() < 16) return false;

      // pop the header - should be > 4000 
      pop();

      int oneBits = 0;
      uint16_t data = 0;

      for (int i = 0; i < 15; ++i) {
        data <<= 1;
        if (pop() > 2000) {
          data |= 1;
          ++oneBits;
        }
      }

      int parity = pop() > 2000 ? 1 : 0;
      
      if ((oneBits & 1) ^ parity == 1) return false;
      
      uint16_t command = data >> 9;
      uint16_t payload = data & 511;

      if (command == SHOT) {
        uint16_t team = payload >> 4;
        uint16_t player = payload & 15;
        Serial.print("shot by player ");
        Serial.print(player, DEC);
        Serial.print(" of team ");
        Serial.println(team, DEC);
      }

      return true;
    }
};

void irIsr() {
  if (digitalRead(Receiver::IRRECEIVERPIN) == LOW) {
    TCNT1 = 0;
    return;
  }

  ringBuffer[head] = TCNT1;
  head = (head + 1) & 63;
}
