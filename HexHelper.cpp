#include "HexHelper.h"

int HexHelper::get_decimal_from_hex(char hex)
{
	if (hex >= '0' && hex <= '9')
		return  hex - '0';
	if (hex >= 'A' && hex <= 'F')
		return  hex - 'A' + 10;
	if (hex >= 'a' && hex <= 'f')
		return  hex - 'a' + 10;
	throw std::exception("Podana wartosc nie jest HEX."); 

}

char HexHelper::get_hex_from_decimal(int decimal)
{
	if (decimal > 16)
		throw std::exception("Podana wartosc nie jest HEX.");
	if (decimal > 9)
		return 'a' + (decimal - 10);
	else
		return '0' + decimal;
}


unsigned char HexHelper::read_from_hex(char first_char, char second_char)
{
	int first_decimal = get_decimal_from_hex(first_char);
	int second_decimal = get_decimal_from_hex(second_char);

	return (first_decimal << 4) + second_decimal;
}


