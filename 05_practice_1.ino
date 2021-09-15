#define PIN_LED 13
unsigned int toggle;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT);
  toggle = 0;
}

void loop() {
  toggle = toggle_state(toggle);
  digitalWrite(PIN_LED, toggle);
  Serial.println("Hello World!");
  delay(1000);
  // put your main code here, to run repeatedly:
}

int toggle_state(int toggle) {
  if (toggle == 0) {
    return 1;
  }
  else
  {
    return 0;
  }
}
