#define PIN_LED 7
unsigned int toggle, cnt, flag;

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  pinMode(PIN_LED, OUTPUT);
  toggle = 0;
  cnt = 0;
  flag = 1;
}

void loop() {
  if(flag){
    digitalWrite(PIN_LED, 0);
    delay(1000);
    flag = 0;
  }
  
  while(cnt <= 10)
  {
    toggle = toggle_state(toggle);
    digitalWrite(PIN_LED, toggle);
    delay(100);
    cnt = cnt + 1;
  }
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
