#include <iostream>
#include "grammar/Grammar.h"
#include "engine/Engine.h"

int main()
{
    Engine eng(1920, 1080);

    Grammar g("A");
    g.add_rule('A', "B-F+CFC+F-D&F^D-F+&&CFC+F+B//");
    g.add_rule('B', "A&F^CFB^F^D^^-F-D^|F^B|FC^F^A//");
    g.add_rule('C', "|D^|F^B-F+C^F^A&&FA&F^C+F+B^F^D//");
    g.add_rule('D', "|CFB-F+B|FA&F^A&&FB-F+B|FC//");
    cylinders cyls = eng.draw(g.generate(2), 90, 1, 1);
    for (const auto& c : cyls)
        std::cout << c << std::endl;
    return 0;
}
