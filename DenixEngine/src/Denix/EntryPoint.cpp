#include "Engine.h"

/**
 * @brief The main entry point for the Denix Engine application.
 *
 * This function initializes the engine and starts its execution.
 *
 * @param argc The number of command-line arguments.
 * @param argv The array of command-line arguments.
 * @return int Returns 0 upon successful execution.
 */
int main(int argc, char** argv)
{
    const auto engine = Denix::MakeEngine();
    engine->Run();

    return 0;
}