#include <random>

#include "vt100.h"

int main(int argc, char const *argv[])
{
    for (int i = 0; i < argc; ++i)
    {
        //Pseudo-random color
        auto fg = vt100::Fg(static_cast<vt100::Color4Bit>(rand() % 15));
        auto bg = vt100::Bg(static_cast<vt100::Color4Bit>(rand() % 15));

        std::cout << fg << bg << argv[i] << vt100::Reset << std::endl;
    }

    return 0;
}
