#include <iostream>
#include "grammar/Grammar.h"
#include "engine/Engine.h"

int main() {
    Engine eng(1920, 1080);

    Grammar g("A");
    g.add_rule('A', "B-F+CFC+F-D&F^D-F+&&CFC+F+B//");
    g.add_rule('B', "A&F^CFB^F^D^^-F-D^|F^B|FC^F^A//");
    g.add_rule('C', "|D^|F^B-F+C^F^A&&FA&F^C+F+B^F^D//");
    g.add_rule('D', "|CFB-F+B|FA&F^A&&FB-F+B|FC//");
    cylinders cyls = eng.draw(g.generate(2), 90, 3, 1);
    eng.save(cyls, "objs.yaml");
    for (const auto& c : cyls)
        std::cout << c << std::endl;
    Mesh m;
    for (auto const& c: cyls)
        m.merge_mesh(c.to_mesh(8, 8));
    m.save_obj("cylinder.obj");
    return 0;
}
