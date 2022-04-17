
const int D_1 = 2;
const int D_2 = 3;
const int D_3 = 4;
const int D_4 = 5;

const int ROW_SEL  = 6;
const int COL_SEL  = 7;
const int WRITE_EN = 8;
const int OUT_EN = 11;
  
const int A_0 = A0;
const int A_1 = A1;
const int A_2 = A2;
const int A_3 = A3;
const int A_4 = A4;
const int A_5 = A5;
const int A_6 = 9;
const int A_7 = 10;

const int BUTTON = A5;

void setDataIn() {
  pinMode(D_1, INPUT);
  pinMode(D_2, INPUT);
  pinMode(D_3, INPUT);
  pinMode(D_4, INPUT);
}

void setDataOut() {
  pinMode(D_1, OUTPUT);
  pinMode(D_2, OUTPUT);
  pinMode(D_3, OUTPUT);
  pinMode(D_4, OUTPUT);
}

byte readData(unsigned int address) {
  unsigned char high_byte = (unsigned char)address;
  unsigned char low_byte = (unsigned char)address >> 8;

  setDataIn();

  digitalWrite(WRITE_EN, HIGH);
  digitalWrite(ROW_SEL, HIGH);
  digitalWrite(COL_SEL, HIGH);
  delayMicroseconds(2);

  // read row
  digitalWrite(A_0, bitRead(high_byte,0));
  digitalWrite(A_1, bitRead(high_byte,1));
  digitalWrite(A_2, bitRead(high_byte,2));
  digitalWrite(A_3, bitRead(high_byte,3));
  digitalWrite(A_4, bitRead(high_byte,4));
  digitalWrite(A_5, bitRead(high_byte,5));
  digitalWrite(A_6, bitRead(high_byte,6));
  digitalWrite(A_7, bitRead(high_byte,7));

  digitalWrite(ROW_SEL, LOW);
  // delayMicroseconds(2);

  // read column
  digitalWrite(A_0, bitRead(low_byte,0));
  digitalWrite(A_1, bitRead(low_byte,1));
  digitalWrite(A_2, bitRead(low_byte,2));
  digitalWrite(A_3, bitRead(low_byte,3));
  digitalWrite(A_4, bitRead(low_byte,4));
  digitalWrite(A_5, bitRead(low_byte,5));
  digitalWrite(A_6, bitRead(low_byte,6));
  digitalWrite(A_7, bitRead(low_byte,7));
  // delayMicroseconds(2);

  digitalWrite(COL_SEL, LOW);
  // delayMicroseconds(2);

  byte data = 0;
  bitWrite(data, 0, digitalRead(D_1));
  bitWrite(data, 1, digitalRead(D_2));
  bitWrite(data, 2, digitalRead(D_3));
  bitWrite(data, 3, digitalRead(D_4));

  // reset
  digitalWrite(ROW_SEL, HIGH);
  digitalWrite(COL_SEL, HIGH);
  delayMicroseconds(2);
  digitalWrite(ROW_SEL, LOW);
  digitalWrite(COL_SEL, LOW);

  return data;
}

void writeData(unsigned int address, byte data) {
  unsigned char high_byte = (unsigned char)address;
  unsigned char low_byte = (unsigned char)address >> 8;

  setDataOut();

  // setup data
  digitalWrite(D_1, bitRead(data,0));
  digitalWrite(D_2, bitRead(data,1));
  digitalWrite(D_3, bitRead(data,2));
  digitalWrite(D_4, bitRead(data,3));
  delayMicroseconds(2);

  // initialize
  digitalWrite(ROW_SEL, HIGH);
  digitalWrite(COL_SEL, HIGH);
  digitalWrite(WRITE_EN, HIGH);
  digitalWrite(OUT_EN, LOW);
  delayMicroseconds(2);

  // write row
  digitalWrite(A_0, bitRead(high_byte,0));
  digitalWrite(A_1, bitRead(high_byte,1));
  digitalWrite(A_2, bitRead(high_byte,2));
  digitalWrite(A_3, bitRead(high_byte,3));
  digitalWrite(A_4, bitRead(high_byte,4));
  digitalWrite(A_5, bitRead(high_byte,5));
  digitalWrite(A_6, bitRead(high_byte,6));
  digitalWrite(A_7, bitRead(high_byte,7));

  digitalWrite(ROW_SEL, LOW);
  // delayMicroseconds(2);

  // write column
  digitalWrite(A_0, bitRead(low_byte,0));
  digitalWrite(A_1, bitRead(low_byte,1));
  digitalWrite(A_2, bitRead(low_byte,2));
  digitalWrite(A_3, bitRead(low_byte,3));
  digitalWrite(A_4, bitRead(low_byte,4));
  digitalWrite(A_5, bitRead(low_byte,5));
  digitalWrite(A_6, bitRead(low_byte,6));
  digitalWrite(A_7, bitRead(low_byte,7));
  // delayMicroseconds(2);
  
  digitalWrite(OUT_EN, HIGH);
  digitalWrite(COL_SEL, LOW);
  digitalWrite(WRITE_EN, LOW);
  // delayMicroseconds(2);

  // reset
  digitalWrite(ROW_SEL, HIGH);
  digitalWrite(COL_SEL, HIGH);
  digitalWrite(WRITE_EN, HIGH);
  digitalWrite(OUT_EN, LOW);
  delayMicroseconds(2);
  digitalWrite(ROW_SEL, LOW);
  digitalWrite(COL_SEL, LOW);
}


void setup() {
  Serial.begin(115200);

  setDataOut();
  pinMode(ROW_SEL, OUTPUT);
  pinMode(COL_SEL, OUTPUT);
  pinMode(WRITE_EN, OUTPUT);
  pinMode(OUT_EN, OUTPUT);

  pinMode(A_0, OUTPUT);
  pinMode(A_1, OUTPUT);
  pinMode(A_2, OUTPUT);
  pinMode(A_3, OUTPUT);
  pinMode(A_4, OUTPUT);
  pinMode(A_5, OUTPUT);
  pinMode(A_6, OUTPUT);
  pinMode(A_7, OUTPUT);

  Serial.println("Starting...");
  // for (int i = 0; i<8; i++) {
  //   digitalWrite(ROW_SEL, HIGH);
  //   delayMicroseconds(2);
  //   digitalWrite(ROW_SEL, LOW);
  //   delayMicroseconds(2);
  // }
  // for (int i = 0; i<8; i++) {
  //   digitalWrite(COL_SEL, HIGH);
  //   delayMicroseconds(2);
  //   digitalWrite(COL_SEL, LOW);
  //   delayMicroseconds(2);
  // }
  // delayMicroseconds(2);

  // writeData(1, 3);
  // writeData(2, 4);
  // for (unsigned int i = 0; i < 100; i++ ) {
  //   // read the first 10 addresses
  //   byte data = readData(i);
  //   Serial.print("i-"+String(i)+": ");
  //   Serial.println(data, BIN);
  // }
  // return;

  Serial.println("read first 10 addresses");
  for (unsigned int i = 0; i < 10; i++ ) {
    // read the first 10 addresses
    byte data = readData(i);
    Serial.print(String(i) + ":");
    Serial.println(data, BIN);
  }
  Serial.println("write all zeros");
  for (unsigned int i = 0; i < 10; i++ ) {
    // read the first 10 addresses
    writeData(i, 0);
  }
  Serial.println("read first 10 addresses");
  for (unsigned int i = 0; i < 10; i++ ) {
    // read the first 10 addresses
    byte data = readData(i);
    Serial.print(String(i) + ":");
    Serial.println(data, BIN);
  }
  Serial.println("write all #s");
  byte write_data = 0;
  // for (unsigned int i = 0; i < 10; i++ ) {
  //   // read the first 10 addresses
  //   write_data++;
  //   writeData(i, write_data);
  //   Serial.print(String(i) + ":");
  //   Serial.println(write_data, BIN);
  // }
  writeData(5, B1111);
  Serial.print("5:");
  Serial.println(B1111, BIN);
  
  Serial.println("read 10 addresses");
  for (unsigned int i = 0; i < 15; i++ ) {
    // read the first 10 addresses
    byte data = readData(i);
    Serial.print(String(i) + ":");
    Serial.println(data, BIN);
  }
}

void loop() {
}
