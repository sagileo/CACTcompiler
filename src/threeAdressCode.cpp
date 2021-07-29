#include "./threeAdressCode.h"

threeAdressCode TAC;
std::vector< std::pair< std::string, int > > array_vector;
std::vector< std::string > array_vector_str;

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