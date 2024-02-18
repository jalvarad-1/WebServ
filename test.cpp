
#include <iostream>

int main() {
	std::string sample = "9\r\nasdfghjkl\r\n";
	size_t firstRN = sample.find("\r\n");
	std::cout << firstRN << std::endl;
	std::cout << sample.substr(0, firstRN) << std::endl;
	char *pEnd;
	double characterN = std::strtod(sample.substr(0, firstRN).c_str(), &pEnd);
	if (*pEnd == '\0')
		std::cout << *pEnd << std::endl;
	std::cout << characterN << std::endl;
}