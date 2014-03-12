#include <iostream>

int main(int argc, char* argv[])
{
	std::cout << "Nothing to evaluate." << std::endl;
	for (unsigned int i = 0; (i < argc); i++)
		std::cout << "\"" << argv[i] << "\", " << std::endl;
	std::cout << std::endl;
	return 0;
}