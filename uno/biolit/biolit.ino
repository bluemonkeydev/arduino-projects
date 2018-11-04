
const int P_TOP_RED = 11;
const int P_TOP_GREEN = 10;
const int P_TOP_BLUE = 9;

const int P_TOP_WHITE = 8;

const int P_BOT_RED = 6;
const int P_BOT_GREEN = 5;
const int P_BOT_BLUE = 4;

const int P_BOT_WHITE = 7;

const int SWITCH_1 = A0;
const int SWITCH_2 = A1;
const int SWITCH_3 = A2;

boolean switch_1_state = false;
boolean switch_2_state = false;
boolean switch_3_state = false;

unsigned int state_1_time = 0;
unsigned int state_2_time = 0;
unsigned int state_3_time = 0;

unsigned int switch_debounce = 500;

boolean top_yellow_blink = false;
boolean top_white_blink = false;
boolean top_blue_blink = false;
boolean top_green_blink = false;
boolean top_red_blink = false;

boolean top_yellow_state = LOW;
boolean top_white_state = LOW;
boolean top_blue_state = LOW;
boolean top_green_state = LOW;
boolean top_red_state = LOW;

unsigned long top_yellow = 0;
unsigned long top_white = 0;
unsigned long top_blue = 0;
unsigned long top_green = 0;
unsigned long top_red = 0;

unsigned long top_yellow_time = 0;
unsigned long top_white_time = 0;
unsigned long top_blue_time = 0;
unsigned long top_green_time = 0;
unsigned long top_red_time = 0;

// bot
boolean bot_yellow_blink = false;
boolean bot_white_blink = false;
boolean bot_blue_blink = false;
boolean bot_green_blink = false;
boolean bot_red_blink = false;

boolean bot_yellow_state = LOW;
boolean bot_white_state = LOW;
boolean bot_blue_state = LOW;
boolean bot_green_state = LOW;
boolean bot_red_state = LOW;

unsigned long bot_yellow = 0;
unsigned long bot_white = 0;
unsigned long bot_blue = 0;
unsigned long bot_green = 0;
unsigned long bot_red = 0;

unsigned long bot_yellow_time = 0;
unsigned long bot_white_time = 0;
unsigned long bot_blue_time = 0;
unsigned long bot_green_time = 0;
unsigned long bot_red_time = 0;

unsigned long blinkDelay = 200;
unsigned long blinkTime = 2000; // 2 seconds
unsigned long solidTime = 4000; // 4 seconds (total)

boolean switch_1_mode = false;
unsigned long switch_1_time = 0;
boolean switch_2_mode = false;
unsigned long switch_2_time = 0;
boolean switch_3_mode = false;
unsigned long switch_3_time = 0;

boolean BOT_YELLOW_BLINK = false;
boolean BOT_WHILTE_BLINK = false;

void setup() {
  pinMode(P_TOP_RED, OUTPUT);
  pinMode(P_TOP_GREEN, OUTPUT);
  pinMode(P_TOP_BLUE, OUTPUT);
  pinMode(P_TOP_WHITE, OUTPUT);

  pinMode(P_BOT_RED, OUTPUT);
  pinMode(P_BOT_GREEN, OUTPUT);
  pinMode(P_BOT_BLUE, OUTPUT);
  pinMode(P_BOT_WHITE, OUTPUT);

  pinMode(SWITCH_1, INPUT);
  pinMode(SWITCH_2, INPUT);
  pinMode(SWITCH_3, INPUT);

  top_yellow_blink = true;
  top_white_blink = true;
  top_white_state = HIGH;

  bot_yellow_blink = true;
  bot_white_blink = true;
  bot_yellow_state = HIGH;

  // analogWrite(P_BOT_RED, 255);
  // analogWrite(P_BOT_GREEN, 35);
}

void loop() {
  unsigned long cMillis = millis();

  if ( top_yellow_blink == true) {
    if (cMillis - top_yellow >= blinkDelay) {
      top_yellow = cMillis;
      top_yellow_state = !top_yellow_state;
      if (top_yellow_state) {
        analogWrite(P_TOP_RED, 255);
        analogWrite(P_TOP_GREEN, 35);
      } else {
        analogWrite(P_TOP_RED, 0);
        analogWrite(P_TOP_GREEN, 0);
      }
    }
  }

  if ( top_white_blink == true) {
    if (cMillis - top_white >= blinkDelay) {
      top_white = cMillis;
      top_white_state = !top_white_state;
      if (top_white_state) {
        digitalWrite(P_TOP_WHITE, HIGH);
      } else {
        digitalWrite(P_TOP_WHITE, LOW);
      }
    }
  }

  if ( top_red_blink == true) {
    if (cMillis - top_red >= blinkDelay) {
      top_red = cMillis;
      top_red_state = !top_red_state;
      if (top_red_state) {
        digitalWrite(P_TOP_RED, HIGH);
      } else {
        digitalWrite(P_TOP_RED, LOW);
      }
    }
  }

  if ( top_green_blink == true) {
    if (cMillis - top_green >= blinkDelay) {
      top_green = cMillis;
      top_green_state = !top_green_state;
      if (top_green_state) {
        digitalWrite(P_TOP_GREEN, HIGH);
      } else {
        digitalWrite(P_TOP_GREEN, LOW);
      }
    }
  }

  if ( top_blue_blink == true) {
    if (cMillis - top_blue >= blinkDelay) {
      top_blue = cMillis;
      top_blue_state = !top_blue_state;
      if (top_blue_state) {
        digitalWrite(P_TOP_BLUE, HIGH);
      } else {
        digitalWrite(P_TOP_BLUE, LOW);
      }
    }
  }

// bot
  if ( bot_yellow_blink == true) {
    if (cMillis - bot_yellow >= blinkDelay) {
      bot_yellow = cMillis;
      bot_yellow_state = !bot_yellow_state;
      if (bot_yellow_state) {
        analogWrite(P_BOT_RED, 255);
        analogWrite(P_BOT_GREEN, 35);
      } else {
        analogWrite(P_BOT_RED, 0);
        analogWrite(P_BOT_GREEN, 0);
      }
    }
  }

  if ( bot_white_blink == true) {
    if (cMillis - bot_white >= blinkDelay) {
      bot_white = cMillis;
      bot_white_state = !bot_white_state;
      if (bot_white_state) {
        digitalWrite(P_BOT_WHITE, HIGH);
      } else {
        digitalWrite(P_BOT_WHITE, LOW);
      }
    }
  }

  if ( bot_red_blink == true) {
    if (cMillis - bot_red >= blinkDelay) {
      bot_red = cMillis;
      bot_red_state = !bot_red_state;
      if (bot_red_state) {
        digitalWrite(P_BOT_RED, HIGH);
      } else {
        digitalWrite(P_BOT_RED, LOW);
      }
    }
  }

  if ( bot_green_blink == true) {
    if (cMillis - bot_green >= blinkDelay) {
      bot_green = cMillis;
      bot_green_state = !bot_green_state;
      if (bot_green_state) {
        digitalWrite(P_BOT_GREEN, HIGH);
      } else {
        digitalWrite(P_BOT_GREEN, LOW);
      }
    }
  }

  if ( bot_blue_blink == true) {
    if (cMillis - bot_blue >= blinkDelay) {
      bot_blue = cMillis;
      bot_blue_state = !bot_blue_state;
      if (bot_blue_state) {
        digitalWrite(P_BOT_BLUE, HIGH);
      } else {
        digitalWrite(P_BOT_BLUE, LOW);
      }
    }
  }


  // all blue flash (2 seconds)
  // all blue on
  // all white on
  int reading = digitalRead(SWITCH_1);
  if( reading == HIGH && cMillis - switch_1_time > switch_debounce ) {
    switch_1_time = cMillis;

    allOff();
    switch_1_mode = true;
    switch_1_time = cMillis;

    top_blue_blink = true;
    bot_blue_blink = true;
  }

  if (switch_1_mode == true && cMillis - switch_1_time >= blinkTime) {
    digitalWrite(P_TOP_BLUE, HIGH);
    digitalWrite(P_TOP_WHITE, HIGH);
    digitalWrite(P_BOT_BLUE, HIGH);
    digitalWrite(P_BOT_WHITE, HIGH);
  }

  if (switch_1_mode == true && cMillis - switch_1_time >= solidTime) {
    allOff();
    top_yellow_blink = true;
    top_white_blink = true;
    bot_yellow_blink = true;
    bot_white_blink = true;
  }

  // top row green flash (2 seconds)
  // top row green on
  // top row white on
  // bot 'normal'
  reading = digitalRead(SWITCH_2);
  if( reading == HIGH && cMillis - switch_2_time > switch_debounce ) {
    switch_2_time = cMillis;

    allOff();
    switch_2_mode = true;
    switch_2_time = cMillis;

    top_green_blink = true;

    bot_yellow_blink = true;
    bot_white_blink = true;
  }

  if (switch_2_mode == true && cMillis - switch_2_time >= blinkTime) {
    digitalWrite(P_TOP_WHITE, HIGH);
    digitalWrite(P_TOP_GREEN, HIGH);
  }

  if (switch_2_mode == true && cMillis - switch_2_time >= solidTime) {
    allOff();
    top_yellow_blink = true;
    top_white_blink = true;
    bot_yellow_blink = true;
    bot_white_blink = true;
  }

  // bot row red flash (2 seconds)
  // bot row red on
  // top 'normal'
  reading = digitalRead(SWITCH_3);
  if( reading == HIGH && cMillis - switch_3_time > switch_debounce ) {
    switch_3_time = cMillis;
    
    allOff();
    switch_3_mode = true;
    switch_3_time = cMillis;

    bot_red_blink = true;
    top_yellow_blink = true;
    top_white_blink = true;
  }

  if (switch_3_mode == true && cMillis - switch_3_time >= blinkTime) {
    digitalWrite(P_BOT_RED, HIGH);
    digitalWrite(P_BOT_WHITE, HIGH);
  }

  if (switch_3_mode == true && cMillis - switch_3_time >= solidTime) {
    allOff();
    top_yellow_blink = true;
    top_white_blink = true;
    bot_yellow_blink = true;
    bot_white_blink = true;
  }
}

void allOff() {
  switch_1_mode = false;
  switch_2_mode = false;
  switch_3_mode = false;

  top_yellow_blink = false;
  top_white_blink = false;
  top_red_blink = false;
  top_blue_blink = false;
  top_green_blink = false;

  top_yellow_state = false;
  top_white_state = true;

  bot_yellow_blink = false;
  bot_white_blink = false;
  bot_red_blink = false;
  bot_blue_blink = false;
  bot_green_blink = false;

  bot_yellow_state = true;
  bot_white_state = false;

  digitalWrite(P_TOP_RED, LOW);
  digitalWrite(P_TOP_GREEN, LOW);
  digitalWrite(P_TOP_BLUE, LOW);
  digitalWrite(P_TOP_WHITE, LOW);

  digitalWrite(P_BOT_RED, LOW);
  digitalWrite(P_BOT_GREEN, LOW);
  digitalWrite(P_BOT_BLUE, LOW);
  digitalWrite(P_BOT_WHITE, LOW);
}
