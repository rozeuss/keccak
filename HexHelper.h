#ifndef HEXHELPER_H
#define HEXHELPER_H

#include <exception>

class HexHelper
{
public:
	static int get_decimal_from_hex(char hex);
	static char get_hex_from_decimal(int decimal);
	static unsigned char read_from_hex(char first_char, char second_char);
};

#endif