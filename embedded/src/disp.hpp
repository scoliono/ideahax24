#ifndef DISP_HPP
#define DISP_HPP

#include "profile.hpp"

void displayReset();
void displayInit();
void displayMatchFound(const Profile* other);
void displayMatchDesc(const Profile* me, const Profile* other);

#endif
