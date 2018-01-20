const int selA = 2;
const int selB = 3;
const int selC = 4;
const int enable = 5;

void setup()
{
  // initialize the serial communication:
  Serial.begin(9600);
  
  // initialize the control outputs
  pinMode(selA, OUTPUT);
  pinMode(selB, OUTPUT);
  pinMode(selC, OUTPUT);
  pinMode(enable, OUTPUT);
  
  digitalWrite(selA, LOW);
  digitalWrite(selB, LOW);
  digitalWrite(selC, LOW);
  
  digitalWrite(enable, HIGH);
}

void writeSignal(int i)
{

  if (i == 0) {
    digitalWrite(enable, LOW);
    digitalWrite(selC, LOW);
    digitalWrite(selB, LOW);
    digitalWrite(selA, LOW);
    digitalWrite(enable, HIGH);

  }else if (i == 1) {
    digitalWrite(enable, LOW);
    digitalWrite(selC, LOW);
    digitalWrite(selB, LOW);
    digitalWrite(selA, HIGH);
    digitalWrite(enable, HIGH);
  
  }else if (i == 2) {
    digitalWrite(enable, LOW);
    digitalWrite(selC, LOW);
    digitalWrite(selB, HIGH);
    digitalWrite(selA, LOW);
    digitalWrite(enable, HIGH);
    
  }else if (i == 3) {
    digitalWrite(enable, LOW);
    digitalWrite(selC, LOW);
    digitalWrite(selB, HIGH);
    digitalWrite(selA, HIGH);
    digitalWrite(enable, HIGH);
    
  }else if (i == 4) {
    digitalWrite(enable, LOW);
    digitalWrite(selC, HIGH);
    digitalWrite(selB, LOW);
    digitalWrite(selA, LOW);
    digitalWrite(enable, HIGH);
    
  }else if (i == 5) {
    digitalWrite(enable, LOW);
    digitalWrite(selC, HIGH);
    digitalWrite(selB, LOW);
    digitalWrite(selA, HIGH);
    digitalWrite(enable, HIGH);
    
  }else if (i == 6) {
    digitalWrite(enable, LOW);
    digitalWrite(selC, HIGH);
    digitalWrite(selB, HIGH);
    digitalWrite(selA, LOW);
    digitalWrite(enable, HIGH);
    
  }else if (i == 7) {
    digitalWrite(enable, LOW);
    digitalWrite(selC, HIGH);
    digitalWrite(selB, HIGH);
    digitalWrite(selA, HIGH);
    digitalWrite(enable, HIGH);
    
  }
}

void loop() {
  writeSignal(random(0,8));
  delay(100);
}

