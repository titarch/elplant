#include <iostream>
#include "grammar/Grammar.h"
#include "engine/Engine.h"

#define DEFAULT_RENDER_MODE "3D"

int main(int argc, char *argv[]) {
    const std::string render_mode = argc == 2 ? argv[1] : DEFAULT_RENDER_MODE;

    Engine eng(1920, 1080);

    if (render_mode == "2D")
        eng.render("../grammars/2D.yaml");
    else if (render_mode == "3D")
        eng.render3D("../grammars/3D.yaml");
    else
        throw std::invalid_argument("Unrecognized render mode");
}
