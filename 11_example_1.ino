#include <Servo.h>

// Arduino pin assignment
#define PIN_LED 9
#define PIN_SERVO 10
#define PIN_TRIG 12
#define PIN_ECHO 13

// configurable parameters
#define SND_VEL 346.0 // sound velocity at 24 celsius degree (unit: m/s)
#define INTERVAL 25 // sampling interval (unit: ms)
#define _DIST_MIN 180 // minimum distance to be measured (unit: mm)
#define _DIST_MAX 360 // maximum distance to be measured (unit: mm)

#define _DUTY_MIN 550 // servo full clockwise position (0 degree)
#define _DUTY_MAX 2400 // servo full counterclockwise position (180 degree)

#define _EMA_ALPHA 0.8

// global variables
float timeout; // unit: us
float dist_min, dist_max, dist_raw, dist_ema; // unit: mm
unsigned long last_sampling_time; // unit: ms
float scale; // used for pulse duration to distance conversion
float emaAlpha;
bool ema_start;
Servo myservo;

void setup() {
// initialize GPIO pins
  pinMode(PIN_LED,OUTPUT);
  pinMode(PIN_TRIG,OUTPUT);
  digitalWrite(PIN_TRIG, LOW); 
  pinMode(PIN_ECHO,INPUT);

  myservo.attach(PIN_SERVO); 
  myservo.writeMicroseconds((_DUTY_MIN + _DUTY_MAX) / 2);

// initialize USS related variables
  dist_min = _DIST_MIN; 
  dist_max = _DIST_MAX;
  emaAlpha = _EMA_ALPHA;

  timeout = (INTERVAL / 2) * 1000.0; // precalculate pulseIn() timeout value. (unit: us)
  dist_raw = dist_ema = 0.0; // raw distance output from USS (unit: mm)
  scale = 0.001 * 0.5 * SND_VEL;
  ema_start = true;

// initialize serial port
  Serial.begin(57600);

// initialize last sampling time
  last_sampling_time = 0;
}

void loop() {
// wait until next sampling time. 
// millis() returns the number of milliseconds since the program started. Will overflow after 50 days.
  if(millis() < last_sampling_time + INTERVAL) return;

// get a distance reading from the USS
  dist_raw = USS_measure(PIN_TRIG,PIN_ECHO);
  if(ema_start)
  {
    dist_ema = dist_raw;
    ema_start = false;
  }
  else
  {
    dist_ema = emaAlpha * dist_raw + (1 - emaAlpha) * dist_ema;
  }

// output the read value to the serial port
  Serial.print("Min:180, raw:");
  Serial.print(dist_raw);
  Serial.print("ema:");
  Serial.print(dist_ema);
  Serial.print(",servo:");
  Serial.print(myservo.read());
  Serial.print(",Max:360\n");

// adjust servo position according to the USS read value

  // add your code here!

if(dist_ema < _DIST_MIN) {
     myservo.writeMicroseconds(_DUTY_MIN);
     digitalWrite(PIN_LED, HIGH);
  }
  else if(dist_ema < _DIST_MAX){
     float f = (_DUTY_MAX - _DUTY_MIN) / (_DIST_MAX - _DIST_MIN);
     float amount = dist_ema * f - _DIST_MIN * f + _DUTY_MIN;
     myservo.writeMicroseconds(amount);
     digitalWrite(PIN_LED, LOW);  
  }
  else {
    myservo.writeMicroseconds(_DUTY_MAX);
    digitalWrite(PIN_LED, HIGH);
  }

// update last sampling time
  last_sampling_time += INTERVAL;
}

// get a distance reading from USS. return value is in millimeter. 
float USS_measure(int TRIG, int ECHO)
{
  float reading;
  digitalWrite(TRIG, HIGH);
  delayMicroseconds(10);
  digitalWrite(TRIG, LOW);

  reading = pulseIn(ECHO, HIGH, timeout) * scale; // unit: mm
  
  return reading;
}
