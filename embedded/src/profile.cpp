#include "profile.hpp"
#include "constants.h"
#include <algorithm>
#include <math.h>
#include <string>

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
        this->goal == other->goal;
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
