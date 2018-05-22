#include <iostream>
#include <string.h>
#include <stdio.h>
#include "HexHelper.h"
#include "Sha3.h"
using namespace std;


const int max_message_size = 2048;
void print_hash(unsigned long long* hash)
{
	for (int i = 0; i < keccak_c / 16; i++)
	{
		int low = ((unsigned char *)hash)[i] % 16;
		char low_c = HexHelper::get_hex_from_decimal(low);
		int high = (int)(((unsigned char *)hash)[i]);
		high = (high >> 4);
		char high_c = HexHelper::get_hex_from_decimal(high);
		cout << high_c << low_c;
	}
	cout << endl;
}


int main(int argc, char * argv[])
{
	if (argc < 2)
	{
		cout << "SHA3: Brak parametrow" << endl;
		cout << "Podaj wiadomosc w hexadecimal string" << endl;
		return 1;
	}
	string message(argv[1]);
	if (message.length() > max_message_size)
	{
		cout << "Maksymalna dlugosc wiadomosci to 1KB (2048 znakow HEX)" << endl;
		return 1;
	}
	Keccak keccak(message);

	unsigned long long* hash = keccak.calculate();
	print_hash(hash);
	delete[] hash;

	return 0;
}
