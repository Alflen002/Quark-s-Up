#include <DS1307.h>
#include <TM1637Display.h>

const uint8_t  rs = 13, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
uint8_t aux = 0;
uint8_t sec, min, hour, day, month;
uint16_t year;
uint16_t glicemia;
const uint8_t LedG= 10 ,LedR = 8,LedY= 17,LedB = 9, LedW = 7,Buzzer = 6, button=12;
uint8_t remedioHora = 10;
uint8_t remedioMinuto = 00;
uint8_t medidaHora = 10;
uint8_t medidaMinuto = 20;
uint8_t consultaDia = 10;
uint8_t consultaMes = 11;
uint8_t consultaAno = 2023;
  /*
  Frequência das notas:
  Dó - 262 Hz
  Ré - 294 Hz
  Mi - 330 Hz
  Fá - 349 Hz
  Sol - 392 Hz
  Lá - 440 Hz
  Si - 494 Hz
  #Dó - 528 Hz
  */

#define CLK 2
#define DIO 3
#define Buzzer 5

TM1637Display display(CLK,DIO);

DS1307 rtc;
int analisadorFreq();
void ReadRTC();
void ReadSensor();
void ControlAlarm();
int analisador(int medica);
float avisoResponsavel();
float avisoUnimed();
int veririficaConsulta(uint8_t consultaMes, uint8_t month);


void setup() {

  Serial.begin(9600);
  while(!Serial); 
  Serial.println("Init RTC...");
  rtc.begin();
  rtc.start();
  pinMode(button,INPUT_PULLUP);
  pinMode(LedG, OUTPUT);
  pinMode(LedR, OUTPUT);
  pinMode(LedY, OUTPUT);
  pinMode(LedB, OUTPUT);
  pinMode(LedW, OUTPUT);
  pinMode(13, OUTPUT);
  digitalWrite(LedG,HIGH);
  digitalWrite(LedW,LOW);
  digitalWrite(LedR,LOW);
  digitalWrite(LedY,LOW);
  digitalWrite(LedB,LOW);
  display.setBrightness(0x0f);
  tone(Buzzer,425, 500);
  delay(1000);
  analogWrite(Buzzer, 0);

}

void loop() {
  // put your main code here, to run repeatedly:
  if(!digitalRead (button)){
    //Serial.print("aaa");
    ReadSensor();
    analisador(glicemia);  
  }
  delay(200);
}

void ReadRTC(){
  rtc.get(&sec, &min, &hour, &day, &month, &year);
}

void ReadSensor(){
  glicemia = analogRead(A0);
  glicemia = map(glicemia,0,1023,30,350);
}

void ControlAlarm(){
  /*
  if(hour<remedioHora && min<remedioMinuto){
     lcd.print(remedioHora);
     lcd.print(":");
     lcd.print(remedioMinuto);
     tone(Buzzer,440,500);
     
  }
  else{
    if(hour<medidaHora && min<medidaMinuto){
      lcd.print(medidaHora);
      lcd.print(":");
      lcd.print(medidaMinuto);
      tone(Buzzer,235,500);
    }else{
      lcd.print("Sem alarme");
    }
  }*/
}

int analisador(int medicao) {
  //Serial.print("Taxa: ");
  Serial.println(medicao);
  //Serial.println(" mg/dL");
  const int hipoglicemia = 71;
  const int normal = 99;
  const int diabetes = 125;
  const int hiperglicemia = 250;
  
  if (medicao >= 0 && medicao <= 70){ //hipo
    display.showNumberDec(medicao,false);
    musicaBuzzer();
    digitalWrite(LedR,LOW);
    digitalWrite(LedY,LOW);
    digitalWrite(LedB,HIGH);
    digitalWrite(LedW,LOW);
    
  }
    
  else if (medicao > 70 && medicao <= 249){ //normal
    display.showNumberDec(medicao,false);
    musicaBuzzer();
    digitalWrite(LedR,LOW);
    digitalWrite(LedY,LOW);
    digitalWrite(LedB,LOW);
    digitalWrite(LedW,HIGH);
  }

  else{ 
    display.showNumberDec(medicao,false);
    musicaBuzzer();
    digitalWrite(LedR,HIGH);
    digitalWrite(LedY,LOW);
    digitalWrite(LedB,LOW);
    digitalWrite(LedW,LOW);
    
      //analisadorFreq();
  }
  delay(1000);
  digitalWrite(LedR,LOW);
  digitalWrite(LedY,LOW);
  digitalWrite(LedB,LOW);
  digitalWrite(LedW,LOW);
}

void musicaBuzzer(){
  delay(1000);
  int tempo = 400;
  tone(Buzzer,440,tempo); //LA
  delay(tempo);
  tone(Buzzer,294,tempo); //RE
  delay(tempo);
  tone(Buzzer,349,tempo/2); //FA
  delay(tempo/2);
  tone(Buzzer,392,tempo/2); //SOL
  delay(tempo/2);
  tone(Buzzer,440,tempo); //LA
  delay(tempo);
  tone(Buzzer,294,tempo); //RE
  delay(tempo);
  tone(Buzzer,349,tempo/2); //FA
  delay(tempo/2);
  tone(Buzzer,392,tempo/2); //SOL
  delay(tempo/2);
  tone(Buzzer,330,tempo); //MI
  delay(tempo);
  /*
  Frequência das notas:
  Dó - 262 Hz
  Ré - 294 Hz
  Mi - 330 Hz
  Fá - 349 Hz
  Sol - 392 Hz
  Lá - 440 Hz
  Si - 494 Hz
  #Dó - 528 Hz
  */
}



/*
float avisoUnimed(){
  int i = 0;
  lcd.setCursor(0, 0);
  lcd.print("==Monitoramento==");
  lcd.setCursor(1, 0);
  lcd.print("Paciente em Alerta.");
  while(i<11){
    digitalWrite(LedG, HIGH);
    delay(100);
    digitalWrite(LedG, LOW);
    delay(100);
  }
}

float avisoResponsavel(){
  int i=0;
  lcd.setCursor(0, 0);
  lcd.print("==Resposável==");
  lcd.setCursor(1, 0);
  lcd.print("Paciente em Alerta.");
  while(i<11){
    digitalWrite(LedR, HIGH);
    delay(100);
    digitalWrite(LedR, LOW);
    delay(100);
  }
}

int veririficaConsulta(uint8_t consultaMes, uint8_t month){
  if (consultaMes > (month + 2)){
    lcd.setCursor(0, 0);
    lcd.print("Consulta Longe");
    lcd.setCursor(1, 0);
    lcd.print("Favor agendar nova consulta");
  }
  if (consultaMes < (month + 2)){
    lcd.setCursor(0, 0);
    lcd.print("Prox Consulta");
    lcd.setCursor(1,0);
    lcd.print(consultaDia);
    lcd.print(":");
    lcd.print(consultaMes);
    lcd.print(":");
    lcd.print(consultaAno);
  }
}
*/