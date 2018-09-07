#define MPXV A7			// pressure sensor output
#define L_R_SW 0		// left or right switch when solo mode is selected
#define SOLO_DUO_SW 1	// solo duo output mode selection
#define BUZZER A0
#define LEFT_RELAY A1
#define RIGHT_RELAY A2
#define LED A3

#define VSCALE 1023.0f
const int PUFF_THD = int(3.0 * VSCALE / 5.0);	//3v
const int SIP_THD = int(1.0 * VSCALE / 5.0);		//1v

// used for LED indication
#define PUFF_DELAY 50
#define SIP_DELAY 500
#define NOM_DELAY 250
int _delay = NOM_DELAY;

// for more elegantly relay control
int LEFT_RELAY_status = LOW;
int RIGHT_RELAY_status = LOW;

void setup(){
	Serial.begin(115200);

	pinMode(MPXV, INPUT);
	pinMode(L_R_SW, INPUT_PULLUP);
	pinMode(SOLO_DUO_SW, INPUT_PULLUP);
	pinMode(BUZZER, OUTPUT);
	pinMode(LEFT_RELAY, OUTPUT);
	pinMode(RIGHT_RELAY, OUTPUT);
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

	// if duo mode is selected
	if (digitalRead(SOLO_DUO_SW) == LOW){
		if (mpxv >= PUFF_THD){
		_delay = PUFF_DELAY;
		smartDigitalWrite(LEFT_RELAY, HIGH, &LEFT_RELAY_status);
		}
		else if (mpxv <= SIP_THD){
			_delay = SIP_DELAY;
			smartDigitalWrite(RIGHT_RELAY, HIGH, &RIGHT_RELAY_status);
		}
		else{
			_delay = NOM_DELAY;
			smartDigitalWrite(LEFT_RELAY, LOW, &LEFT_RELAY_status);
			smartDigitalWrite(RIGHT_RELAY, LOW, &RIGHT_RELAY_status);
		}
		int flickering_cnt = (_delay == NOM_DELAY)? 1:5;
	}
	else{
		// select target relay according to left/right switch
		int targetRelay = (digitalRead(L_R_SW) == LOW)? LEFT_RELAY:RIGHT_RELAY;
		if (mpxv >= PUFF_THD){
			_delay = PUFF_DELAY;
			digitalWrite(targetRelay, HIGH);
		}
		else if (mpxv <= SIP_THD){
			_delay = SIP_DELAY;
			digitalWrite(targetRelay, LOW);
		}
		else _delay = NOM_DELAY;

	}
	// LED feedback, flash quickly when puff, slowly when sip, and nomally when rest
	// since flickering rate when sip is slow,
	// to have more robust indicator, reduce flickering cycles when sip
	int flickering_cnt = (_delay == SIP_DELAY)? 2:5;
	for (int i = 0; i < flickering_cnt; i ++){
		digitalWrite(LED, HIGH);
		delay(_delay);
		digitalWrite(LED, LOW);
		delay(_delay);
	}
}

void smartDigitalWrite(int pin, int target, int* status){
	if (target != *status){
		digitalWrite(pin, target);
		*status = (*status == HIGH)? LOW:HIGH;
	}
}
