#include "disp.hpp"
#include "constants.h"
#include "beep.h"

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>

static Adafruit_SSD1306 display(128, 64, &Wire, -1);
static const char* common[] = {"Socks", "Bachroooms", "CS 36L", "Inductors", "Scopes"};
static int state = 0;
static int commonInd = 0;

void displayReset()
{
    display.setCursor(0, 0);     // Start at top-left corner
    digitalWrite(DISP_BLUELED, HIGH);
    digitalWrite(DISP_REDLED, HIGH);
    digitalWrite(DISP_GREENLED, HIGH);
    display.clearDisplay();
}

void displayInit()
{
    //Address: 0x3C
    pinMode(DISP_BUTTON, INPUT_PULLUP);
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);

    pinMode(BUZZER_PIN, OUTPUT);
    pinMode(DISP_BLUELED, OUTPUT);
    pinMode(DISP_GREENLED, OUTPUT);
    pinMode(DISP_REDLED, OUTPUT);
    digitalWrite(DISP_BLUELED, HIGH);
    digitalWrite(DISP_REDLED, HIGH);
    digitalWrite(DISP_GREENLED, HIGH);

    displayReset();

    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.println("Looking...");
    display.display();
}

void displayMatchFound(const Profile* other) {
    displayReset();
    display.setTextSize(2);
    display.setTextColor(SSD1306_WHITE);
    display.println("MATCH FOUND!!");
    display.printf("%s\n", other->name);
    display.display();
    commonInd = -1;
    playTune();
}

void displayMatchDesc(const Profile* other) {
    displayReset();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.printf("%s\n", other->name);
    display.println("Conversation opener:");
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
