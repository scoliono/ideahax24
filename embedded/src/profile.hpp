#ifndef PROFILE_HPP
#define PROFILE_HPP

#include <stdint.h>
#include "constants.h"
#include <ArduinoJson.h>

struct Profile {
    const char* name;
    uint16_t bdayYear;
    uint8_t gender;
    uint8_t desiredGender;
    uint8_t goal;
    const char* similarity;

    float mag() const;

    /**
     * Returns cosine similarity value between -1 and 1, by taking the normalized dot product.
     */
    static float cosSimilarity(const struct Profile *p1, const struct Profile *p2);

    /**
     * Returns whether the two profiles are compatible.
     */
    bool isMatch(const struct Profile *other) const;

    /**
     * Serializes this profile to JSON.
     */
    void toJSON(StaticJsonDocument<JSON_DOC_LEN>& doc) const;

    /**
     * Deserializes a profile from JSON.
     */
    static Profile fromJSON(const JsonDocument& doc);
};

#endif
