// for AT TINY 85
#define MPXV_IN A3
#define LED A2
#define RELAY A1

const float VOLTAGE_SCALE = 1024.0 / 5.0;  
const float HIGH_THRSHLD = 3.0 * VOLTAGE_SCALE; //3v
const float LOW_THRSHLD = 1.0 * VOLTAGE_SCALE; //1v

const int NOM_DELAY = 250;
const int HIGH_DELAY = 50;
const int LOW_DELAY = 500;

int DELAY = NOM_DELAY;

void setup(){
  pinMode(MPXV_IN, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(RELAY, OUTPUT);

  // initialting indicator
  for (int i = 0; i < 10; i ++){
    digitalWrite(LED, HIGH);
    delay(50);
    digitalWrite(LED, LOW);
    delay(50);
  }
}

void loop(){
  float voltageRead = analogRead(MPXV_IN);
  if (voltageRead >= HIGH_THRSHLD){
    DELAY = HIGH_DELAY;
    digitalWrite(RELAY, HIGH);
  }
  else if (voltageRead <= LOW_THRSHLD){
    DELAY = LOW_DELAY;
    digitalWrite(RELAY, LOW);
  } 
  else{
    DELAY = NOM_DELAY;
  }
  int cnt = (DELAY == NOM_DELAY)? 1:5;
  for (int i = 0; i < cnt; i ++){
    digitalWrite(LED, HIGH);
    delay(DELAY);
    digitalWrite(LED, LOW);
    delay(DELAY);
  }
}