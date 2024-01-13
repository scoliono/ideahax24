#include <ArduinoJson.h>
#include <stdint.h>
#include "constants.h"
#include <math.h>

struct Profile {
    const char* name;
    uint16_t bdayYear;
    uint8_t gender;
    uint8_t desiredGender;
    uint8_t goal;
    const char* similarity;

    float mag() const
    {
        float mag = 0.0f;
        for (int i = 0; i < SIMILARITY_VEC_LEN; ++i)
        {
            if (this->similarity[i] == '1')
                mag += 1.0f;
        }
        return sqrtf(mag);
    }

    /**
     * Returns cosine similarity value between -1 and 1, by taking the normalized dot product.
     */
    static float cosSimilarity(const struct Profile *p1, const struct Profile *p2)
    {
        float normDotProduct = 0.0f;
        for (int i = 0; i < SIMILARITY_VEC_LEN; ++i)
        {
            normDotProduct += (p1->similarity[i] == '1') * (p2->similarity[i] == '1');
        }
        normDotProduct /= (p1->mag() * p2->mag());
        return normDotProduct;
    }
};

class PemdasSerial {
public:
    PemdasSerial();
    ~PemdasSerial();
    bool recvProfile(struct Profile* out);
private:
    void sendProfileAck();
    char json[MAX_PAYLOAD_LEN];
};
