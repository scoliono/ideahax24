 #include <Adafruit_SSD1306.h>
#include <splash.h>
#define BUTTON 19
#define BLUELED 4
#define GREENLED 18
#define REDLED 5
Adafruit_SSD1306 display(128, 64, &Wire, -1);
int state = 0;
int buttonState=0;
int lastButton = 0;
int commonInd = 0;
String common[5]={"Socks", "Bachroooms", "CS 36L", "Inductors", "Scopes"}; 
void setup() {
  // put your setup code here, to run once:
  
  //Address: 0x3C
  pinMode(BUTTON, INPUT_PULLUP);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

pinMode(15, OUTPUT);
pinMode(BLUELED, OUTPUT);
pinMode(GREENLED, OUTPUT);
pinMode(REDLED, OUTPUT);
digitalWrite(BLUELED, HIGH);
digitalWrite(REDLED, HIGH);
digitalWrite(GREENLED, HIGH);

stateChange();
//  tone(15, 880, 250);
//  tone(15, 988, 250);
//  tone(15, 1109, 250);
//  tone(15, 988, 250);
//  tone(15, 880, 250);
//  tone(15, 831, 250);
//  tone(15, 740, 250);
//  tone(15, 988, 250);
//  tone(15, 831, 250);
//  tone(15, 659, 250);
//  tone(15, 880, 1000);
//tone(15, 622, 250);
//tone(15, 698, 250);
//tone(15, 740, 250);
//tone(15, 831, 250);
//tone(15, 932, 500);
//tone(15, 1245, 250);
//tone(15, 1109, 250);
//tone(15, 932, 500);
//tone(15, 622, 500);
//tone(15, 932, 250);
//tone(15, 831, 250);
//tone(15, 740, 250);
//tone(15, 698, 250);
//
//tone(15, 622, 250);
//tone(15, 698, 250);
//tone(15, 740, 250);
//tone(15, 831, 250);
//tone(15, 932, 500);
//tone(15, 831, 250);
//tone(15, 740, 250);
//tone(15, 698, 250);
//tone(15, 622, 250);
//tone(15, 698, 250);
//tone(15, 740, 250);
//tone(15, 698, 250);
//tone(15, 622, 250);
//tone(15, 587, 250);
//tone(15, 698, 250);
//
//tone(15, 622, 250);
//tone(15, 698, 250);
//tone(15, 740, 250);
//tone(15, 831, 250);
//tone(15, 932, 500);
//tone(15, 1245, 250);
//tone(15, 1109, 250);
//tone(15, 932, 500);
//tone(15, 622, 500);
//tone(15, 932, 250);
//tone(15, 831, 250);
//tone(15, 740, 250);
//tone(15, 698, 250);
//
//tone(15, 622, 250); 
//tone(15, 698, 250); //F
//tone(15, 740, 250);
//tone(15, 831, 250);
//tone(15, 932, 500);
//tone(15, 831, 250);
//tone(15, 740, 250);
//tone(15, 698, 500);
//tone(15, 740, 500);
//tone(15, 831, 500);
//tone(15, 932, 500);

}
void playTune(){
    tone(15, 880, 250);
  tone(15, 988, 250);
  tone(15, 1109, 250);
  tone(15, 988, 250);
  tone(15, 880, 250);
  tone(15, 831, 250);
  tone(15, 740, 250);
  tone(15, 988, 250);
  tone(15, 831, 250);
  tone(15, 659, 250);
  tone(15, 880, 1000);
  
}



void stateChange(){
    display.setCursor(0, 0);     // Start at top-left corner
digitalWrite(BLUELED, HIGH);
digitalWrite(REDLED, HIGH);
digitalWrite(GREENLED, HIGH);
      display.clearDisplay();
    if (state == 0){
    display.setTextSize(2);  
      display.setTextColor(SSD1306_WHITE);
    display.println("Looking...");
    display.display();
    
  }
  if (state == 1){
    display.setTextSize(1);  
      display.setTextColor(SSD1306_WHITE);
    display.println("MATCH FOUND");
    display.println("Name: Eggert");
    display.println("I love en passant");
    display.display();
    commonInd = -1;
    playTune();
  }
  if (state > 1){
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.println("Name: Eggert");
    display.println("Random common thing:");
    display.println("You both love");
    int ind;
    do{
      ind = random(5);
    }
    while (ind == commonInd);
    commonInd = ind;
    display.println(common[ind]);
    display.display();
digitalWrite(BLUELED, HIGH);
digitalWrite(REDLED, HIGH);
digitalWrite(GREENLED, LOW);
  }
}



void loop() {
  // put your main code here, to run repeatedly:
  if (digitalRead(BUTTON)== LOW and millis() - lastButton > 300){
    int buttonStart = millis();
    while (digitalRead(BUTTON) == LOW){}
    int buttonDir = millis() - buttonStart;
    lastButton = millis();
    if (buttonDir > 5000){
      state = 0;
    }
    else{
      state++;
    }
    stateChange();
  }

}
