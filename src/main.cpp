#include <iostream>
#include "grammar/Grammar.h"
#include "engine/Engine.h"
#include <boost/program_options.hpp>

namespace po = boost::program_options;

int main(int argc, char* argv[]) {
    unsigned width, height;
    std::string render_mode, yaml_file;
    po::options_description desc("Options: ");
    desc.add_options()
            ("help", "Display this information")
            ("mode,m", po::value<std::string>(&render_mode)->default_value("3D"), R"("2D" or "3D" ("3D" by default))")
            ("load,l", po::value<std::string>(&yaml_file)->default_value(""),
             "Specify YAML to load, defaults are ../grammars/[2 or 3]D.yaml depending on render mode")
            ("width,w", po::value<unsigned>(&width)->default_value(1920), "Width of the display for 2D render mode")
            ("height,h", po::value<unsigned>(&height)->default_value(1080), "Height of display for 2D render mode");

    po::variables_map vm;
    try {
        po::store(po::parse_command_line(argc, argv, desc), vm);
        po::notify(vm);
    } catch (po::error const& e) {
        std::cerr << "Option error: " << e.what() << std::endl;
        return 2;
    }

    Engine eng(width, height);
    if (render_mode == "2D") {
        if (yaml_file.empty()) yaml_file = "../grammars/2D.yaml";
        eng.render(yaml_file);
    } else if (render_mode == "3D") {
        if (yaml_file.empty()) yaml_file = "../grammars/3D.yaml";
        eng.render3D(yaml_file);
    } else {
        std::cerr << render_mode + ": unrecognized render mode use 2D or 3D" << std::endl;
        return 3;
    }

    return 0;
}
