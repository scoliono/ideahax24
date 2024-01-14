#include "profile.hpp"
#include "constants.h"
#include <algorithm>
#include <math.h>

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
    bool acceptableAgeGap = olderAge / 2 + 7 <= youngerAge;     // according to this formula james could date a 17 yo

    return cosSimilarity(this, other) > 0.5 &&
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
    struct Profile out;
    out.name = doc["name"];
    out.bdayYear = doc["bdayYear"];
    out.gender = doc["gender"];
    out.desiredGender = doc["desiredGender"];
    out.goal = doc["goal"];
    out.similarity = doc["similarity"];
    return out;
}
