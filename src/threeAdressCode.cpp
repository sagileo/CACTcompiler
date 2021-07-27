#include "./threeAdressCode.h"

threeAdressCode TAC;

std::string mynewlabel()
{
	static int number = 0;
	return std::string("L") + std::to_string(number++);
}

std::string mynewtemp()
{
	static int number = 0;
	return std::string("t") + std::to_string(number++);
}