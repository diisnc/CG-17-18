#include <iostream>
#include <string>
#include <Windows.h>

int main(int argc, char** argv) {

    SetConsoleOutputCP(CP_UTF8); // UTF-8 mode for Windows console

    if(argv[1] != NULL) { // Checks if no argv[1] is not empty

        std::string form = argv[1];
        std::cout << form << " é argv1." << std::endl;

        if(form.compare("plane") == 0 && argc == 20) {
            std::cout << "Está a criar um plano." << std::endl;
            std::cout << "Triângulo 1:" << std::endl;
            std::cout << "A= (" << argv[2] << "," << argv[3] << "," << argv[4] << ")" << std::endl;
            std::cout << "B= (" << argv[5] << "," << argv[6] << "," << argv[7] << ")" << std::endl;
            std::cout << "C= (" << argv[8] << "," << argv[9] << "," << argv[10] << ")" << std::endl;
            std::cout << "Triângulo 2:" << std::endl;
            std::cout << "A= (" << argv[11] << "," << argv[12] << "," << argv[13] << ")" << std::endl;
            std::cout << "B= (" << argv[14] << "," << argv[15] << "," << argv[16] << ")" << std::endl;
            std::cout << "C= (" << argv[17] << "," << argv[18] << "," << argv[19] << ")" << std::endl;
        }
        else if(form.compare("box") == 0 && argc == 26) {
            std::cout << "Está a criar um quadrado." << std::endl;
            std::cout << "A= (" << argv[2] << "," << argv[3] << "," << argv[4] << ")" << std::endl;
            std::cout << "B= (" << argv[5] << "," << argv[6] << "," << argv[7] << ")" << std::endl;
            std::cout << "C= (" << argv[8] << "," << argv[9] << "," << argv[10] << ")" << std::endl;
            std::cout << "D= (" << argv[11] << "," << argv[12] << "," << argv[13] << ")" << std::endl;
            std::cout << "E= (" << argv[14] << "," << argv[15] << "," << argv[16] << ")" << std::endl;
            std::cout << "F= (" << argv[17] << "," << argv[18] << "," << argv[19] << ")" << std::endl;
            std::cout << "G= (" << argv[20] << "," << argv[21] << "," << argv[22] << ")" << std::endl;
            std::cout << "H= (" << argv[23] << "," << argv[24] << "," << argv[25] << ")" << std::endl;
        }
        else if(form.compare("sphere") == 0 && argc == 5) {
            std::cout << "Está a criar uma esfera." << std::endl;
            std::cout << "Raio= " << argv[2] << std::endl;
            std::cout << "Slices= " << argv[3] << std::endl;
            std::cout << "Stacks= " << argv[4] << std::endl;
        }
        else if(form.compare("cone") == 0 && argc == 6) {
            std::cout << "Está a criar um cone." << std::endl;
            std::cout << "Raio= " << argv[2] << std::endl;
            std::cout << "Altura= " << argv[3] << std::endl;
            std::cout << "Slices= " << argv[4] << std::endl;
            std::cout << "Stacks= " << argv[5] << std::endl;
        }
        else {
            std::cout << argv[1] << " não é válido." << std::endl;
        }
    }


    return 0;
    
}