#include <iostream>
#include "grammar/Grammar.h"
#include "engine/Engine.h"

int main() {
    Engine eng(1920, 1080);
    Grammar g("A");
    g.add_rule('A', "[&FL!A]/////[&FL!A]///////[&FL!A]");
    g.add_rule('F', "S/////F");
    g.add_rule('S', "FL");
    g.add_rule('L', "[^^{-f+f+f-|-f+f+f}]");
    /*
    Grammar g("FX");
    g.add_rule('Y', "-FX-Y");
    g.add_rule('X',"X+YF+");
     */
    auto s = g.generate(5);
    std::cout << s << std::endl;
    Plant p = eng.draw(s, 22.5, 3, 0.5);
    for (const auto& c : p.cyls)
        std::cout << c << std::endl;
    Mesh m = p.to_mesh(12, 2);

    m.save_obj("cylinder.obj");
    return 0;
}
