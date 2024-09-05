#include <iostream>
#include <string>

int interactive_main();
int simulation_main(int argc, char** argv);

int main(int argc, char** argv) {
    std::string mode;
    std::cout << "Select mode (interactive/simulation): ";
    std::cin >> mode;

    if (mode == "interactive") {
        return interactive_main();
    } else if (mode == "simulation") {
        return simulation_main(argc, argv);
    } else {
        std::cout << "Invalid mode selected." << std::endl;
        return 1;
    }
}