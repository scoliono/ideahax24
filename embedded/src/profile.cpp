#include "profile.hpp"
#include "constants.h"
#include <algorithm>
#include <math.h>
#include <string>

const char* questions[NUM_QUESTIONS] = {
    "Favorite music", "Dietary", "Major", "Earliest class", "Fav color",
    "Blood Type","Room decor","Zombie apoc weapon","Elec component describing relationships","Preferred diode for stress",
    "Fav Boelter floor","Relationship waveform", "1st date loc", "Fun fact", "Of in the cold food","Chosen GE",
    "2 truths 1 lie","Red flag"
};
const int answersPerQuestion[NUM_QUESTIONS] = {4,5,8,4,9,4,4,2,2,3,4,3,4,4,2,4,3,4};
const char* answers[NUM_QUESTIONS][9] = {
    {"Rock","Classical","Pop","Jazz"},
    {"None","Pescatarian","Gluten free","Vegetarian","Vegan"},
    {"Arts & Arch","Engr","UCLA College","Educ & Info","Music","Nursing","Pub Aff","Film TV"},
    {"8 AM","9 AM","10 AM","2 PM"},
    {"Red","Orange","Yellow","Green","Blue","Purple","White","Black","Brown"},
    {"O+/-","A+/-","B+/-","AB+/-"},
    {"Posters","LED strips","Plants","Oscilloscopes"},
    {"Pool noodle","Baguette"},
    {"Resistor","Inductor"},
    {"Diode: negativity in one direction","Zener: stable breakdown voltage","LED: calm glow amidst chaos"},
    {"1", "4", "5", "9"},
    {"Sine wave: smooth and harmonious","Square wave: surprises + sharp transitions", "Triangle wave: steadily asc, w/ occasional peaks"},
    {"The 405 Freeway","St Monica Sock Museum","The Bachrooms","CS 36L discussion"},
    {"Never had COVID (but I have ligma)","Can taste a square wave","Family members in the mob","Just lost The Game"},
    {"Yes","No"},
    {"Oceanic Eel Car Battery Baptism","Study of Antarctican Cinema","Washing Machines in Impressionist Art","Business: Long Division"},
    {"Smelled bad in class","Victim of viral TikTok prank","Bird pooped on me twice in day"},
    {"Refers to self in 3rd person","Made out with oscilloscope","My \"homework\" folder is 2 terabytes","Room full of anime figurines"}
};


float Profile::mag() const
{
    float mag = 0.0f;
    for (int i = 0; i < SIMILARITY_VEC_LEN; ++i)
    {
        if (this->similarity[i] == '1')
            mag += 1.0f;
    }
    return sqrtf(mag);
}

float Profile::cosSimilarity(const struct Profile *p1, const struct Profile *p2)
{
    float normDotProduct = 0.0f;
    for (int i = 0; i < SIMILARITY_VEC_LEN; ++i)
    {
        normDotProduct += (p1->similarity[i] == '1') * (p2->similarity[i] == '1');
    }
    normDotProduct /= (p1->mag() * p2->mag());
    return normDotProduct;
}

bool Profile::isMatch(const struct Profile *other) const
{
    int older = other->bdayYear < this->bdayYear ? other->bdayYear : this->bdayYear;
    int olderAge = YEAR - older;
    int younger = other->bdayYear > this->bdayYear ? other->bdayYear : this->bdayYear;
    int youngerAge = YEAR - younger;
    bool acceptableAgeGap = olderAge / 2 + 7 <= youngerAge;

    float cossim = cosSimilarity(this, other);
    Serial.printf("Cosine similarity: %f\n", cossim);
    return cossim > 0.5 &&
        acceptableAgeGap &&
        (other->gender & this->desiredGender) == other->gender &&
        (this->gender & other->desiredGender) == this->gender &&
        this->goal == other->goal;
}

struct Profile::QAPair questionFromAnswer(int ansIdx)
{
    int tot = 0;
    for (int i = 0; i < NUM_QUESTIONS; i++) {
        tot += answersPerQuestion[i];
        if (tot > ansIdx) {
            return Profile::QAPair{
                .q = i,
                .a = ansIdx - (tot-answersPerQuestion[i])
            };
        }
    }
    Serial.println("QfromA: this should not happen!");
    return Profile::QAPair{
        .q = -1,
        .a = -1
    };
}

void Profile::overlaps(const struct Profile *p1, const struct Profile *p2, struct QAPair **out, int *outLen)
{
    *outLen = 0;
    *out = new QAPair[NUM_QUESTIONS];

    // janky bitwise AND done on two strings lmao
    int* answers = new int[SIMILARITY_VEC_LEN];
    for (int i = 0; i < SIMILARITY_VEC_LEN; ++i)
    {
        if (p1->similarity[i] == '1' && p2->similarity[i] == '1') {
            answers[i] = 1;
        } else {
            answers[i] = 0;
        }
    }
    for (int i = 0; i < SIMILARITY_VEC_LEN; ++i)
    {
        if (answers[i]) {
            (*out)[*outLen] = questionFromAnswer(i);
            *outLen = *outLen + 1;
        }
    }
}

void Profile::toJSON(StaticJsonDocument<JSON_DOC_LEN>& doc) const
{
    doc["name"] = this->name;
    doc["bdayYear"] = this->bdayYear;
    doc["gender"] = this->gender;
    doc["desiredGender"] = this->desiredGender;
    doc["goal"] = this->goal;
    doc["similarity"] = this->similarity;
}

Profile Profile::fromJSON(const JsonDocument& doc)
{
    char* name = new char[NAME_LEN+1];
    strncpy(name, doc["name"], NAME_LEN);
    char* similarity = new char[SIMILARITY_VEC_LEN+1];
    strncpy(similarity, doc["similarity"], SIMILARITY_VEC_LEN);

    Profile prof;
    prof.name = name;
    prof.bdayYear = doc["bdayYear"];
    prof.gender = doc["gender"];
    prof.desiredGender = doc["desiredGender"];
    prof.goal = doc["goal"];
    prof.similarity = similarity;
    return prof;
}
