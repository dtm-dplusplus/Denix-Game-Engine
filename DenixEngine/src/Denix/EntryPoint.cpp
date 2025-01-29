#include "Engine.h"

int main(int argc, char** argv)
{
    const auto engine = Denix::MakeEngine();
    engine->Run();

    return 0;
}
