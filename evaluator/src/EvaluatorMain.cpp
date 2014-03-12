#include <iostream>
#include "CommandLineArguments.h"

int main(int argc, char* argv[])
{
	// Parse command line arguments
	mdsearch::CommandLineArguments args(argc, argv);
	// If the arguments are not valid, exit program with failure code 
	if (!args.isValid())
	{
		std::cout << "Invalid command line arguments (use -h or --help for accepted arguments)." << std::endl;
		return 1;
	}

	std::cout << "Not implemented yet." << std::endl;

	return 0;
}