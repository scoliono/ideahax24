#include "disp.hpp"
#include "constants.h"
#include "beep.h"
#include "profile.hpp"

#include <Arduino.h>
#include <Adafruit_SSD1306.h>
#include <splash.h>

static Adafruit_SSD1306 display(128, 64, &Wire, -1);
static int commonInd = 0;
static int state = 0;

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
    display.println("MATCH\nFOUND!!");
    display.printf("%s\n", other->name);
    display.display();
    commonInd = -1;
    playTune();
}

void displayMatchDesc(const Profile* me, const Profile* other) {
    displayReset();
    display.setTextSize(1);
    display.setTextColor(SSD1306_WHITE);
    display.printf("%s\n", other->name);
    display.println("Conversation opener:");
    display.println();

    Profile::QAPair* out;
    int outLen;
    Profile::overlaps(me, other, &out, &outLen);

    int ind;
    do {
        ind = random(outLen);
    } while (ind == commonInd);

    int qIdx = out[ind].q;
    int aIdx = out[ind].a;

    display.printf("%s\n", questions[qIdx]);
    display.printf("%s\n", answers[qIdx][aIdx]);
    commonInd = ind;

    display.display();
    digitalWrite(DISP_BLUELED, HIGH);
    digitalWrite(DISP_REDLED, HIGH);
    digitalWrite(DISP_GREENLED, LOW);
}
