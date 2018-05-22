#ifndef SHA3_H
#define SHA3_H

#include "HexHelper.h"
#include <string>




//via http://keccak.noekeon.org/specs_summary.html#roundConstants
const unsigned long long keccakf_round_constants[24] =
{
	0x0000000000000001, 0x0000000000008082, 0x800000000000808a, 0x8000000080008000, 
	0x000000000000808b, 0x0000000080000001, 0x8000000080008081, 0x8000000000008009, 
	0x000000000000008a, 0x0000000000000088, 0x0000000080008009, 0x000000008000000a,
	0x000000008000808b, 0x800000000000008b, 0x8000000000008089, 0x8000000000008003,
	0x8000000000008002, 0x8000000000000080, 0x000000000000800a, 0x800000008000000a, 
	0x8000000080008081, 0x8000000000008080, 0x0000000080000001, 0x8000000080008008
};
/*
const int keccak_rotation_offsets[24] =
{
	1, 3, 6, 10, 15, 21, 28, 36, 45, 55, 2, 14, 27, 41, 56, 8, 25, 43, 62, 18, 39, 61, 20, 44
};
*/
const int keccakf_piLane[24] =
{
	10, 7, 11, 17, 18, 3, 5, 16, 8, 21, 24, 4, 15, 23, 19, 13, 12, 2, 20, 14, 22, 9, 6, 1
};

const int keccak_r = 576;
const int keccak_c = 1024;

class Keccak
{
public:
	Keccak(std::string message);
	~Keccak() {};
	unsigned long long* calculate();
	void keccakf(unsigned long long* state, int);
private:

	void theta(unsigned long long* state, unsigned long long variable_a, unsigned long long* variable_b);
	void rhoPi(unsigned long long* state, unsigned long long variable_a, unsigned long long* variable_b);
	void chi(unsigned long long* state, unsigned long long* variable_b);
	void iota(unsigned long long* state, int round);
	int calculate_padding_size(int message_size);
	void pad(unsigned char * padded_message, int message_size_in_bytes, int padded_message_size_in_bytes);
	unsigned char* Keccak::prepare_message();

	int message_size;
	int padding_size_in_bytes;
	bool is_message_in_bytes;
	int message_size_in_bytes;
	int padded_message_size_in_bytes;
	std::string message;
};

#endif
