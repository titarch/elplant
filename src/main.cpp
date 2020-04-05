#include <iostream>
#include "grammar/Grammar.h"
#include "engine/Engine.h"

int main() {
    Engine eng(1920, 1080);
    Grammar g("P");
    g.add_rule('P', "I+[P+O]--//[--L]I[++L]-[PO]++PO");
    g.add_rule('I', "FS[//&&L][//^^L]FS");
    g.add_rule('S', "SFS");
    g.add_rule('L', "[`{+f-ff-f+|+f-ff-f}]");
    g.add_rule('O', "[&&&E`/W////W////W////W////W]");
    g.add_rule('E', "FF");
    g.add_rule('W', "[`^F][{&&&&--f++f|--f++f}]");
    auto s = g.generate(5);
    std::cout << s << std::endl;
    materials mtls = {DefaultMtl::Green, DefaultMtl::White, DefaultMtl::Yellow};
    Plant p = eng.draw(s, 18, 3, 0.5);
    for (const auto& c : p.cyls)
        std::cout << c << std::endl;
    p.save_plant("plant.obj", "plant.mtl", mtls);
    return 0;
}
