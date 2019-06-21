class gun
{
    // MilesTag standard basic pulse duration of 600uS
    // 4*IRpulse for header bit,
    // 2*IRpulse for 1
    // 1*IRpulse for 0.
    private const int IRPulseDuration = 600;

    uint8_t playerCode;

    private int triggerPin = 4;
    private bool lastTriggerRead;

    gun(int team, int player) {
      this.playerCode = team << 5 + player;
    }

    void begin() {
      pinMode (LED, INPUT);

      // set up Timer 2
      TCCR2A = _BV (COM2A0) | _BV(WGM21);  // CTC, toggle OC2A on Compare Match
      TCCR2B = _BV (CS20);   // No prescaler
      OCR2A =  209;          // compare A register value (210 * clock speed)

      lastTriggerRead = false;
    }


    bool triggerPulled()
    {
      bool triggerRead = digitalRead(triggerPin) == LOW;

      bool triggered = triggerRead && triggerRead != lastTriggerRead;
      lastTriggerRead = triggerRead;

      return triggered;
    }


    void shoot() {
      // start bit
      sendPulses(4);
      delayMicroseconds(IRpulseDuration);

      int isOne;
      uint8_t xmitData = playerCode;
      for (int i = 0; i < 8; i++) {
        isOne = (xmitData & 0x80) == 0x80;
        xmitData <<= 1;
        sendPulses(1 + isOne);
        delayMicroseconds(IRpulseDuration);
      }

      // stop bit
      sendPulses(1);
    }


    void sendPulses(int numBlocks) {
      while (numBlocks) {
        int pulseCount = IRpulsesPer600uS;
        while (pulseCount) {
          digitalWrite(TRANSMITPIN, HIGH);
          delayMicroseconds(IRt);
          digitalWrite(TRANSMITPIN, LOW);
          delayMicroseconds(IRt);
          --pulseCount;
        }
        --numBlocks;
      }
    }
