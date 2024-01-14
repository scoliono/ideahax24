#ifndef CONSTANTS_H
#define CONSTANTS_H

#define BUZZER_PIN 15
// use ArduinoJSON calculator
#define JSON_DOC_LEN 384
#define NAME_LEN 32
#define SIMILARITY_VEC_LEN 73
#define YEAR 2024
#define DATE_SRV_UUID "DA73"
#define PROF_CHAR_UUID "47F1"

#define DISP_BUTTON 19
#define DISP_BLUELED 5
#define DISP_GREENLED 4
#define DISP_REDLED 2

#define NUM_QUESTIONS 18

extern const char* questions[NUM_QUESTIONS];
extern const int answersPerQuestion[NUM_QUESTIONS];
extern const char* answers[NUM_QUESTIONS][9];

#endif
