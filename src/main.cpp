#include <iostream>
#include "grammar/Grammar.h"
#include "engine/Engine.h"

int main() {
    Engine eng(1920, 1080);
    Grammar g("A");
    g.add_rule('A', "[&FLA]/////[&FLA]///////[&FLA]");
    g.add_rule('F', "S/////F");
    g.add_rule('S', "FL");
    /*
    Grammar g("FX");
    g.add_rule('Y', "-FX-Y");
    g.add_rule('X',"X+YF+");
     */
    cylinders cyls = eng.draw(g.generate(6), 22.5, 3, 0.5);
    eng.save(cyls, "objs.yaml");
    for (const auto& c : cyls)
        std::cout << c << std::endl;
    Mesh m;
    for (auto const& c: cyls)
        m.merge_mesh(c.to_mesh(8, 8));
    m.save_obj("cylinder.obj");
    return 0;
}
