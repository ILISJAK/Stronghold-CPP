#include <iostream>
#include "StrongholdAPI.h"

int main(int argc, char* argv[]) {
	utility::string_t port = U("34568");
	utility::string_t address = U("http://localhost:");
	address.append(port);

	StrongholdApi api(address);
	api.open().wait();

	std::cout << "Press ENTER to exit." << std::endl;
	std::string line;
	std::getline(std::cin, line);

	api.close().wait();

	return 0;
}