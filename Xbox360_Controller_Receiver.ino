/* Arduino code to communicate with xbox 360 RF module.
Original work by (yaywoop) / additional ideas from Alexander Martinez - modified by dilandou (www.dilandou.com, www.diru.org/wordpress) */

#define sync_pin 2 //power button repurposed for sync button (pin 5 on the module)
#define data_pin 3 //data line (pin 6 on the module)
#define clock_pin 4 //clock line (pin 7 on module) 

int led_cmd[10] =  {0, 0, 1, 0, 0, 0, 0, 1, 0, 0}; //Activates/initialises the LEDs, leaving the center LED lit.
int anim_cmd[10] = {0, 0, 1, 0, 0, 0, 0, 1, 0, 1}; //Makes the startup animation on the ring of light.
int sync_cmd[10] = {0, 0, 0, 0, 0, 0, 0, 1, 0, 0}; //Initiates the sync process.
volatile boolean sync_enable = 0;

void sendData(int cmd_do[]) {
  pinMode(data_pin, OUTPUT);
  digitalWrite(data_pin, LOW);    //start sending data.
  int prev = 1;
  for (int i = 0; i < 10; i++) {

    while (prev == digitalRead(clock_pin)) {} //detects change in clock
    prev = digitalRead(clock_pin);
    // should be after downward edge of clock, so send bit of data now
    digitalWrite(data_pin, cmd_do[i]);

    while (prev == digitalRead(clock_pin)) {} //detects upward edge of clock
    prev = digitalRead(clock_pin);
  }
  digitalWrite(data_pin, HIGH);
  pinMode(data_pin, INPUT);
}

void initLEDs() {
  sendData(led_cmd);
  delay(50);
  sendData(anim_cmd);
  delay(50);
}

void setup() {
  Serial.begin(9600);
  pinMode(sync_pin, INPUT);
  digitalWrite(sync_pin, HIGH);
  pinMode(data_pin, INPUT);
  pinMode(clock_pin, INPUT);
  delay(2000);

  initLEDs();
}

void loop() {
  Serial.println("Syncing.");
  sendData(sync_cmd);
  delay(10000);
}