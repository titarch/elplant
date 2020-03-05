#include <SFML/Graphics.hpp>
#include <chrono>
#include "grammar/Grammar.h"
#include "engine/Engine.h"

int main()
{
    Engine eng(1920, 1080);

    Grammar g("FX");
    g.add_rule('Y', "-FX-Y");
    g.add_rule('X', "X+YF+");

    eng.render(g, 10, 90, 10);

    return 0;
}
