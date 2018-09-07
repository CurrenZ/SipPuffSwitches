#define MPXV A3			// pressure sensor output
#define LED A4
#define RELAY A1

#define VSCALE 1023.0f
const int PUFF_THD = int(3.0 * VSCALE / 5.0);	//3v
const int SIP_THD = int(1.0 * VSCALE / 5.0);		//1v

// used for LED indication
#define PUFF_DELAY 50
#define SIP_DELAY 500
#define NOM_DELAY 250
int _delay = NOM_DELAY;

void setup(){
	Serial.begin(115200);

	pinMode(MPXV, INPUT);
	pinMode(RELAY, OUTPUT);
	pinMode(LED, OUTPUT);

	// flickering LED, nothing but a cool indicator
	for (int i = 0; i < 5; i ++){
	    digitalWrite(LED, HIGH);
	    delay(_delay);
	    digitalWrite(LED, LOW);
	    delay(_delay);
  	}
}

void loop(){
	int mpxv = analogRead(MPXV);

  if (mpxv >= PUFF_THD){
    _delay = PUFF_DELAY;
    digitalWrite(targetRelay, HIGH);
  }
  else if (mpxv <= SIP_THD){
    _delay = SIP_DELAY;
    digitalWrite(targetRelay, LOW);
  }
  else _delay = NOM_DELAY;

	for (int i = 0; i < flickering_cnt; i ++){
		digitalWrite(LED, HIGH);
		delay(_delay);
		digitalWrite(LED, LOW);
		delay(_delay);
	}
}
