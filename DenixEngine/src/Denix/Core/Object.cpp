#include "Object.h"

#include <random>

unsigned int Denix::Object::GenID()
{
    // Create a random device to seed the random number generator
    std::random_device rd;

    // Use the Mersenne Twister engine
    std::mt19937 gen(rd());

    // Define the range for the random numbers (6 digits)
    std::uniform_int_distribution<> dis(100000, 999999);

    // Generate the random number
    return dis(gen);
}
