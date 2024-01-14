#include "disp.hpp"
#include "constants.h"
#include "beep.h"

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>

static Adafruit_SSD1306 display(128, 64, &Wire, -1);
static const char* common[] = {"Socks", "Bachroooms", "CS 36L", "Inductors", "Scopes"};
static int state = 0;
static int buttonState = 0;
static int lastButton = 0;
static int commonInd = 0;

Display::Display()
{
    //Address: 0x3C
    pinMode(DISP_BUTTON, INPUT_PULLUP);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

    pinMode(15, OUTPUT);
    pinMode(DISP_BLUELED, OUTPUT);
    pinMode(DISP_GREENLED, OUTPUT);
    pinMode(DISP_REDLED, OUTPUT);
    digitalWrite(DISP_BLUELED, HIGH);
    digitalWrite(DISP_REDLED, HIGH);
    digitalWrite(DISP_GREENLED, HIGH);

    stateChange();
}

Display::~Display()
{

}

void Display::stateChange() {
    display.setCursor(0, 0);     // Start at top-left corner
    digitalWrite(DISP_BLUELED, HIGH);
    digitalWrite(DISP_REDLED, HIGH);
    digitalWrite(DISP_GREENLED, HIGH);
    display.clearDisplay();
    if (state == 0) {
        display.setTextSize(2);
        display.setTextColor(SSD1306_WHITE);
        display.println("Looking...");
        display.display();
    }
    if (state == 1) {
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.println("MATCH FOUND");
        display.println("Name: Eggert");
        display.println("I love en passant");
        display.display();
        commonInd = -1;
        playTune();
    }
    if (state > 1) {
        display.setTextSize(1);
        display.setTextColor(SSD1306_WHITE);
        display.println("Name: Eggert");
        display.println("Random common thing:");
        display.println("You both love");
        int ind;
        do {
            ind = random(5);
        } while (ind == commonInd);
        commonInd = ind;
        display.println(common[ind]);
        display.display();
        digitalWrite(DISP_BLUELED, HIGH);
        digitalWrite(DISP_REDLED, HIGH);
        digitalWrite(DISP_GREENLED, LOW);
    }
}

void Display::loop() {
    if (digitalRead(DISP_BUTTON)== LOW && millis() - lastButton > 300) {
        int buttonStart = millis();
        while (digitalRead(DISP_BUTTON) == LOW);
        int buttonDir = millis() - buttonStart;
        lastButton = millis();
        if (buttonDir > 5000) {
            state = 0;
        } else {
            state++;
        }
        stateChange();
    }
}
