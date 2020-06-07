#include <Adafruit_SSD1306.h>
#include <Fonts/FreeMono9pt7b.h>

Adafruit_SSD1306 display(4);

unsigned long prevMillis_DISPLAY = 0;
unsigned long prevMillis_BUTTON = 0;
unsigned long prevMillis_SWITCH = 0;
const unsigned long ONE_SECOND = 1*1000;
const unsigned long HALF_SECOND = 500;

int encoderPin1 = 7;
int encoderPin2 = 6;
int encoderSwitchPin = 5;
boolean encoderSwitchPressed = false;

volatile int lastEncoded = 0;
boolean fastEncoder = false;
boolean encoder_freq = true; // or PWM when false

// frequency (timer2)
volatile long encoderValue_freq = 128;

// pwm (timer1)
volatile long encoderValue_pwm = 128;

int timer1Divider = 1;
int timer2Divider = 1;

int lastMSB = 0;
int lastLSB = 0;

int switchPins[] = {A0,A1,A2,A3,4,8,12};
int lastSwitchValue = -1;

boolean updateDisplay = true;

int timer1ValueMap(int encoderValue) {
  if (encoderValue == 2) { // 0%
    return 0;
  } else if (encoderValue <= 25 ) { // 10%
    return 25;
  } else if (encoderValue <= 51 ) { // 20%
    return 51;
  } else if (encoderValue <= 64 ) { // 25%
    return 64;
  } else if (encoderValue <= 102 ) { // 40%
    return 102;
  } else if (encoderValue <= 128 ) { // 50%
    return 128;
  } else if (encoderValue <= 154 ) { // 60%
    return 154;
  } else if (encoderValue <= 192 ) { // 75%
    return 192;
  } else if (encoderValue <= 205 ) { // 80%
    return 205;
  } else if (encoderValue <= 230 ) { // 90%
    return 230;
  } else { // 100%
    return 256;
  }
}

void drawDisplay(String freq1, String freq2) {

  display.clearDisplay();

  display.setCursor(0,12);
  while (freq1.length() < 11) {
    freq1 = " " + freq1;
  }
  display.println(freq1);

  // line seperator
  display.drawLine(20, display.height()/2, display.width() - 20, display.height()/2, WHITE);

  display.setCursor(0,28);
  while (freq2.length() < 11) {
    freq2 = " " + freq2;
  }
  display.println(freq2);

  if (encoder_freq == true) {
    // setting frequency
    display.drawCircle(5, 7, 3, WHITE);
    if (fastEncoder) {
      display.fillCircle(5, 7, 3, WHITE);
    }
  } else {
    display.drawCircle(5, display.height()-10, 3, WHITE);
    if (fastEncoder) {
      display.fillCircle(5, display.height()-10, 3, WHITE);
    }
  }

  // right indicator (frequency)
  int lineHeight = map(encoderValue_freq, 0, 256, 0, display.height());
  display.drawLine(display.width() - 1,lineHeight,display.width() - 1, display.height() - 1, WHITE);
  display.drawLine(display.width() - 2,lineHeight,display.width() - 2, display.height() - 1, WHITE);
  display.drawLine(display.width() - 3,lineHeight,display.width() - 3, display.height() - 1, WHITE);

  // bottom indicator (pwm)
  int lineWidth = map(timer1ValueMap(encoderValue_pwm), 256, 2, 0, display.width()-8);
  display.drawLine(4,display.height()-1,lineWidth+4, display.height() - 1, WHITE);
  display.drawLine(4,display.height()-2,lineWidth+4, display.height() - 2, WHITE);
  display.display();
}

void setup() {
  Serial.begin (115200);

  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.setTextSize(1);
  display.setFont(&FreeMono9pt7b);
  display.setTextColor(SSD1306_WHITE);
  display.display();
  delay(250);
  
  // Timer1, pin 9 & 10
  // Fast PWM, 8-bit
  // Inverting signal on pins
  pinMode(9, OUTPUT);
  pinMode(10, OUTPUT);
  TCCR1A = _BV(COM1A0) | _BV(COM1A1) | _BV(COM1B1)| _BV(WGM10);
  TCCR1B = _BV(WGM12) | _BV(CS10);
  OCR1A = 128;
  OCR1B = 128;
  
  // Timer2, pin 3 & 11
  // Fast PWM
  pinMode(3, OUTPUT);
  pinMode(11, OUTPUT);
  TCCR2A = _BV(COM2A0) | _BV(COM2B1) | _BV(WGM20) | _BV(WGM21);
  TCCR2B = _BV(WGM22) | _BV(CS20);
  OCR2A = 128;
  OCR2B = 64;
  
  // encoder
  pinMode(encoderPin1, INPUT);
  pinMode(encoderPin2, INPUT);
  pinMode(encoderSwitchPin, INPUT);
  digitalWrite(encoderPin1, HIGH);
  digitalWrite(encoderPin2, HIGH);
  digitalWrite(encoderSwitchPin, HIGH);

  // dipswitch pins
  for (int i = 0; i < 7; i++) {
    pinMode(switchPins[i], INPUT);
    digitalWrite(switchPins[i], HIGH);
  }
}

boolean checkSwitch() {
  for (int i = 0; i < 7; i++ ) {
    if(digitalRead(switchPins[i]) == LOW){
      if (lastSwitchValue == i) {
        return false;
      }
      lastSwitchValue = i;
      return true;
    }
  }
  return false;
}


void loop() {
  
  if (updateEncoder() == true) {
    Serial.println(encoderValue_freq);
    if (encoder_freq == true) {
      // Timer 2 (frequency)
      OCR2A = encoderValue_freq;
      OCR2B = encoderValue_freq/2;
    } else {
      // Timer 1 (pwm)
      OCR1A = OCR1B = timer1ValueMap(encoderValue_pwm);
    }
    updateDisplay = true;
  }

  unsigned long currentMillis = millis();
 
  // encoder switch
  if (encoderSwitchPressed == false && digitalRead(encoderSwitchPin) == LOW) {
    prevMillis_BUTTON = currentMillis; // start timer
    encoderSwitchPressed = true;
  }

  if (encoderSwitchPressed == true && digitalRead(encoderSwitchPin) == HIGH) {
    // button released
    unsigned long duration = currentMillis - prevMillis_BUTTON;
    if (duration < 200) {
      // short press / change speed
      fastEncoder = !fastEncoder;
    } else {
      // long press / change modes
      encoder_freq = !encoder_freq;
    }
    encoderSwitchPressed = false;
    updateDisplay = true;
  }

  // update display
  if (currentMillis - prevMillis_DISPLAY >= HALF_SECOND) {
    prevMillis_DISPLAY = currentMillis;
    if (updateDisplay) { // need updating?
      // math
      // 16 MHz / 64 / (180+1) / 2 = 690.6Hz 
      float freq1 = 16000000 / timer2Divider / (encoderValue_freq + 1 );
      String suffix1;
      if (freq1 > 1000000) {
        freq1 = freq1 / 1000000;
        suffix1 = "mHz";
      }
      else if (freq1 > 1000) {
        freq1 = freq1 / 1000;
        suffix1 = "kHz";
      }
      else {
        suffix1 = "hz";
      }

      float freq2 = 16000000 / timer1Divider;
      String suffix2;
      if (freq2 > 1000000) {
        freq2 = freq2 / 1000000;
        suffix2 = "mHz";
      }
      else if (freq2 > 1000) {
        freq2 = freq2 / 1000;
        suffix2 = "kHz";
      }
      else {
        suffix2 = "hz";
      }
      
      String strFreq1 = String(freq1) + suffix1;
      String strFreq2 = String(freq2) + suffix2;
      drawDisplay(strFreq1, strFreq2);
      updateDisplay = false;
    }
  }

  // Check Switches
  if (currentMillis - prevMillis_SWITCH >= ONE_SECOND) {
    prevMillis_SWITCH = currentMillis;

    if(checkSwitch() == true){
      updateDisplay = true;
      switch (lastSwitchValue) {
        case 6:
        // Timer 1
        TCCR1B = _BV(WGM12) | _BV(CS10);
        timer1Divider = 1;
        // Timer 2
        TCCR2B = _BV(WGM22) | _BV(CS20);
        timer2Divider = 1;
        break;
  
        case 5:
        // Timer 1
        TCCR1B = _BV(WGM12) | _BV(CS11);
        timer1Divider = 8;
        // Timer 2
        TCCR2B = _BV(WGM22) | _BV(CS21);
        timer2Divider = 8;
        break;
  
        case 4:
        // Timer 1
        TCCR1B = _BV(WGM12) | _BV(CS11) | _BV(CS10);
        timer1Divider = 64;
        // Timer 2
        TCCR2B = _BV(WGM22) | _BV(CS20) | _BV(CS21);
        timer2Divider = 32;
        break;
  
        case 3:
        // Timer 1
        TCCR1B = _BV(WGM12) | _BV(CS12);
        timer1Divider = 256;
        // Timer 2
        TCCR2B = _BV(WGM22) | _BV(CS22);
        timer2Divider = 64;
        break;
  
        case 2:
        // Timer 1
        TCCR1B = _BV(WGM12) | _BV(CS12) | _BV(CS10);
        timer1Divider = 1024;
        // Timer 2
        TCCR2B = _BV(WGM22) | _BV(CS22) | _BV(CS20);
        timer2Divider = 128;
        break;
  
        case 1:
        // Timer 1
        TCCR1B = _BV(WGM12) | _BV(CS12) | _BV(CS10);
        timer1Divider = 1;
        // Timer 2
        TCCR2B = _BV(WGM22) | _BV(CS22) | _BV(CS21);
        timer2Divider = 256;
        break;
  
        case 0:
        // Timer 1
        TCCR1B = _BV(WGM12) | _BV(CS12) | _BV(CS10);
        timer1Divider = 1;
        // Timer 2
        TCCR2B = _BV(WGM22) | _BV(CS22) | _BV(CS21) | _BV(CS20);
        timer2Divider = 1024;
        break;
      }
    }
  }
}

boolean updateEncoder() {
  // https://www.best-microcontroller-projects.com/rotary-encoder.html
  // From bildr article: https://bildr.org/2012/08/rotary-encoder-arduino/
  boolean updated = false;
  boolean up = false;
  int LSB = digitalRead(encoderPin2);
  int MSB = digitalRead(encoderPin1);
  int encoded = (MSB << 1) | LSB;
  int sum = (lastEncoded << 2) | encoded;
  lastEncoded = encoded;
//  cc
//    11,01
//    01,00
//    00,10
//    10,11
  if (/*sum == 0b1101 || */sum == 0b0100 || sum == 0b0010 /*|| sum == 0b1011*/) {
//    Serial.println(sum, BIN);
    up = false;
    updated = true;
  }
//  cw
//    11,10
//    10,00
//    00,01
//    01,11
  else if (/*sum == 0b1110 || */sum == 0b1000 || sum == 0b0001/* || sum == 0b0111*/) {
//    Serial.println(sum, BIN);
    up = true;
    updated = true;
  }
  
  if (updated == true) {
    if (encoder_freq == true) {
      // frequency mode
      if (up) {
        if (fastEncoder) {
          encoderValue_freq = encoderValue_freq + 8;
        } else {
          encoderValue_freq++;
        }
        if (encoderValue_freq > 255) {
          encoderValue_freq = 255;
        }
      } else {
        if (fastEncoder) {
          encoderValue_freq = encoderValue_freq - 8;
        } else {
          encoderValue_freq--;
        }
        if (encoderValue_freq < 2) {
          encoderValue_freq = 2;
        }
      }
    } else {
      // pwm mode
      if (up) {
        encoderValue_pwm = encoderValue_pwm + 16;
        if (encoderValue_pwm > 256) {
          encoderValue_pwm = 256;
        }
      } else {
        encoderValue_pwm = encoderValue_pwm - 16;
        if (encoderValue_pwm < 1) {
          encoderValue_pwm = 1;
        }
      }
    }
  }
  return updated;
}

// pin 2  
// pin 3  / Timer2 / 1/2 frequency from pin 11
// pin 4  / DIP 5 / Clock Select / Timer1 /1024 / Timer2 /128
// pin 5  / Rotary Encoder / Button
// pin 6  / Rotary Encoder 
// pin 7  / Rotary Encoder
// pin 8  / DIP 6 / Clock Select / Timer1 /1024 / Timer2 /256
// pin 9  / Timer1 
// pin 10 / Timer1 / inverted signal from pin 9
// pin 11 / Timer2
// pin 12 / DIP 7 / Clock Select / Timer1 /1024 / Timer2 /1024
// pin 13 / LED

// A0 / DIP 1 / Clock Select / Timer1 /1    / Timer2 /1
// A1 / DIP 2 / Clock Select / Timer1 /8    / Timer2 /8
// A2 / DIP 3 / Clock Select / Timer1 /64   / Timer2 /32
// A3 / DIP 4 / Clock Select / Timer1 /256  / Timer2 /64
// A4 / SDA / oled
// A5 / SCL / oled
// A6 
// A7 
