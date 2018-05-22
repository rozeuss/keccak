#include "Sha3.h"

namespace {
	unsigned long long mod5(unsigned long long x)
	{
		if (x < 5)
			return x;

		return x - 5;
	}
}

void Keccak::theta(unsigned long long* state, unsigned long long variable_a, unsigned long long* variable_b)
{
	for (int i = 0; i < 5; i++)
	{
		variable_b[i] = state[i] ^ state[i + 5] ^ state[i + 10] ^ state[i + 15] ^ state[i + 20];
	}
	for (int i = 0; i < 5; i++) {
		variable_a = variable_b[mod5(i + 4)] ^ (((variable_b[mod5(i + 1)]) << 1) | ((variable_b[mod5(i + 1)]) >> 63));
			state[i	    ] = state[i	    ] ^ variable_a;
			state[i +  5] = state[i  + 5] ^ variable_a;
			state[i + 10] = state[i + 10] ^ variable_a;
			state[i + 15] = state[i + 15] ^ variable_a;
			state[i + 20] = state[i + 20] ^ variable_a;
	}
}




void Keccak::rhoPi(unsigned long long* state, unsigned long long variable_a, unsigned long long* variable_b)
{
	variable_a = state[1];
	for (int i = 0; i < 24; i++) {
		int j = keccakf_piLane[i];
		const int offset = (((i + 1)*(i + 2)) / 2)%64;
		variable_b[0] = state[j];
		state[j] = (((variable_a) << (offset)) | ((variable_a) >> (64 - (offset))));
		variable_a = variable_b[0];
	}
}

void Keccak::chi(unsigned long long* state, unsigned long long* variable_b)
{
	for (int j = 0; j <= 24; j += 5) {
		variable_b[0] = state[j + 0];
		variable_b[1] = state[j + 1];
		variable_b[2] = state[j + 2];
		variable_b[3] = state[j + 3];
		variable_b[4] = state[j + 4];
		for (int i = 0; i < 5; i++)
			state[j + i] ^= (~variable_b[mod5(i + 1)]) & variable_b[mod5(i + 2)];
	}
}

void Keccak::iota(unsigned long long* state, int round)
{
	state[0] = state[0] ^ keccakf_round_constants[round];
}
void Keccak::keccakf(unsigned long long state[25], int rounds)
{
	unsigned long long variable_a = 0;
	unsigned long long variable_b[5];

	for (int round = 0; round < rounds; round++) {

		theta(state, variable_a, variable_b);
		rhoPi(state, variable_a, variable_b);
		chi(state, variable_b);
		iota(state, round);
	}
}

int Keccak::calculate_padding_size(int message_size)
{
	if (message_size % 8 != 0)
	{
		message_size += 4;
		if ((message_size % keccak_r) == 0)
			return 0;
	}

	return (keccak_r - ((message_size) % keccak_r)) / 8;
}
void Keccak::pad(unsigned char * padded_message, int message_size_in_bytes, int padded_message_size_in_bytes)
{
	for (int i = message_size_in_bytes + 1; i < padded_message_size_in_bytes; i++)
	{
		padded_message[i] = 0;
	}

	padded_message[padded_message_size_in_bytes - 1] |= 0x80; 
}


unsigned char* Keccak::prepare_message()
{
	message_size = message.length() * 4; 
	padding_size_in_bytes = calculate_padding_size(message_size); 
	is_message_in_bytes = (message.length() % 2 == 0); 
	message_size_in_bytes = (is_message_in_bytes) ? message.length() / 2 : (message.length() / 2) + 1;
	padded_message_size_in_bytes = message_size_in_bytes + padding_size_in_bytes;
	unsigned char * padded_message = new unsigned char[padded_message_size_in_bytes];

	for (int i = 0; i < message_size_in_bytes - 1; i++) 
	{
		padded_message[i] = HexHelper::read_from_hex(message[i * 2], message[(i * 2) + 1]);
	}
	if (is_message_in_bytes)
	{
		if (message_size_in_bytes > 0)
		{
			padded_message[message_size_in_bytes - 1] = HexHelper::read_from_hex(message[(message_size_in_bytes - 1) * 2], message[((message_size_in_bytes - 1) * 2) + 1]);
		}
		padded_message[message_size_in_bytes] = 0x01; 
		pad(padded_message, message_size_in_bytes, padded_message_size_in_bytes);
	}
	else
	{
		padded_message[message_size_in_bytes - 1] = HexHelper::read_from_hex('1', message[(message_size_in_bytes - 1) * 2]);
		pad(padded_message, message_size_in_bytes - 1, padded_message_size_in_bytes);
	}
	return padded_message;
}

unsigned long long* Keccak::calculate()
{
	unsigned long long* hash = new unsigned long long[25];
	memset(hash, 0, 200); 
	unsigned char* prepared_message = prepare_message();
 
	int bitrate_in_bytes = keccak_r / 8; 
	int n = (padded_message_size_in_bytes % bitrate_in_bytes > 0) ? padded_message_size_in_bytes / bitrate_in_bytes + 1 : padded_message_size_in_bytes / bitrate_in_bytes; //obliczanie ilosci blokow we wiadomosci
	for (int i = 0; i < n; i++)
	{
		for (int j = 0; j < (keccak_r / 64); j++)
		{
			hash[j] ^= ((unsigned long long *) prepared_message)[((i * bitrate_in_bytes)/8) + j];
		}
		Keccak::keccakf(hash, 24);  
	}
	delete[] prepared_message;
	return hash;
}

Keccak::Keccak(std::string message)
{
	this->message = message;
}


