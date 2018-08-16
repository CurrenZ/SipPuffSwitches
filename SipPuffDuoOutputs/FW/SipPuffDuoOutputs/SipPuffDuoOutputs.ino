// for AT TINY 84
#define MPXV_IN A7
#define LED A3
#define BUZZER A0
#define RELAY_L A1
#define RELAY_R A2
#define SW_L_R 10
#define SW_2_1 9

const float VOLTAGE_SCALE = 1024.0 / 5.0;  
const float HIGH_THRSHLD = 3.0 * VOLTAGE_SCALE; //3v
const float LOW_THRSHLD = 1.0 * VOLTAGE_SCALE; //1v

const int NOM_DELAY = 250;
const int HIGH_DELAY = 50;
const int LOW_DELAY = 500;
const int RELAY_HOLD = 500;

int LED_DELAY = NOM_DELAY;

void setup(){
  pinMode(MPXV_IN, INPUT);
  pinMode(SW_L_R, INPUT);
  pinMode(SW_2_1, INPUT);
  pinMode(LED, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  pinMode(RELAY_L, OUTPUT);
  pinMode(RELAY_R, OUTPUT);

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
  int L_R = digitalRead(SW_L_R);
  int Duo_One = digitalRead(SW_2_1);
  int targetRelay = RELAY_L; // initialting the pin
  bool isActivated = false; // true if sip or puff

  // by defualt, the mode is set to duo outputs
  if (voltageRead >= HIGH_THRSHLD){
    // when puff
    isActivated = true;
    LED_DELAY = HIGH_DELAY;
    targetRelay = RELAY_L;
  }
  else if (voltageRead <= LOW_THRSHLD){
    // when sip
    isActivated = true;
    LED_DELAY = LOW_DELAY;
    targetRelay = RELAY_R;
  }
  else{
    // when rest
    isActivated = false;
    LED_DELAY = NOM_DELAY;
    targetRelay = RELAY_L;
  }

  // if single output mode is selected
  if (Duo_One == LOW) targetRelay = (L_R == HIGH)? RELAY_L:RELAY_R;

  relayActivation_noLatching(targetRelay, isActivated);
  indicator(LED_DELAY, isActivated);
}

void relayActivation_noLatching(int rrr, bool aaa){
  if (aaa){
    digitalWrite(rrr, HIGH);
    delay(RELAY_HOLD);
    digitalWrite(rrr, LOW);
  }
}

void indicator(int delayTime, bool aaa){
  int cnt = (LED_DELAY == NOM_DELAY)? 1
          : (LED_DELAY == LOW_DELAY)? 2
          : 5;
  for (int i = 0; i < cnt; i ++){
    digitalWrite(LED, HIGH);
    delay(LED_DELAY);
    digitalWrite(LED, LOW);
    delay(LED_DELAY);
  }
  if (aaa){
    digitalWrite(BUZZER, HIGH);
    delay(RELAY_HOLD);
    digitalWrite(BUZZER, LOW);
  }
}