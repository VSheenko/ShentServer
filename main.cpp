#include <iostream>

#include "FrontController/FrontController.h"

int main(int argc, char* argv[]) {
	FrontController controller(std::stoi(argv[1]));

	controller.run();
}
