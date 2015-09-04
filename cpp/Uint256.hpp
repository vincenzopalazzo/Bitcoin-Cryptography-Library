/* 
 * Bitcoin cryptography library, copyright (c) Project Nayuki
 * http://www.nayuki.io/page/bitcoin-cryptography-library
 * https://github.com/nayuki/Bitcoin-Cryptography-Library
 */

#pragma once

#include <cstdint>


/* 
 * An unsigned 256-bit integer, represented as eight unsigned 32-bit words in little endian.
 * All arithmetic operations are performed modulo 2^256 (the standard unsigned overflow behavior).
 * Instances of this class are mutable. All possible values are valid.
 * 
 * For example, the integer 0x0123456789abcdef000000001111111122222222333333334444444455555555 is represented by
 * the array {0x55555555, 0x44444444, 0x33333333, 0x22222222, 0x11111111, 0x00000000, 0x89abcdef, 0x01234567}.
 */
class Uint256 {
	
	#define NUM_WORDS 8
	
	/* Fields */
	
	// The words representing this number in little endian, conceptually like this:
	// actualValue = value[0] << 0 | value[1] << 32 | ... | value[7] << 224.
	// Remember, this field is public and mutable.
public:
	uint32_t value[NUM_WORDS];
	
	
	
	/* Constructors */
public:
	
	// Constructs a Uint256 initialized to zero. Constant-time.
	// For clarity, only use this constructor if the variable will be overwritten immediately
	// (pretend that this constructor leaves the value array uninitialized).
	// For actual zero values, please explicitly initialize them with: Uint256 num(Uint256::ZERO);
	Uint256();
	
	
	// Constructs a Uint256 from the given 64-character hexadecimal string. Not constant-time.
	// If the syntax of the string is invalid, then an assertion will fail.
	explicit Uint256(const char *str);
	
	
	// Constructs a Uint256 from the given 32 bytes encoded in big-endian.
	// Constant-time with respect to the input array values. All possible values are valid.
	explicit Uint256(const uint8_t b[NUM_WORDS * 4]);
	
	
	
	/* Arithmetic methods */
	
	// Adds the given number into this number, modulo 2^256. The other number must be a distinct object.
	// Enable must be 1 to perform the operation or 0 to do nothing. Returns the carry-out bit, which is 0 or 1.
	// Constant-time with respect to both values and the enable.
	uint32_t add(const Uint256 &other, uint32_t enable=1);
	
	
	// Subtracts the given number from this number, modulo 2^256. The other number must be a distinct object.
	// Enable must be 1 to perform the operation or 0 to do nothing. Returns the borrow-out bit, which is 0 or 1.
	// Constant-time with respect to both values and the enable.
	uint32_t subtract(const Uint256 &other, uint32_t enable=1);
	
	
	// Shifts this number left by 1 bit (same as multiplying by 2), modulo 2^256.
	// Returns the old leftmost bit, which is 0 or 1. Constant-time with respect to this value.
	uint32_t shiftLeft1();
	
	
	// Shifts this number right by 1 bit (same as dividing by 2 and flooring).
	// Constant-time with respect to this value.
	void shiftRight1();
	
	
	// Shifts this number right by 1 bit (same as dividing by 2 and flooring).
	// Enable must be 1 to perform the operation or 0 to do nothing.
	// Constant-time with respect to this value and the enable.
	void shiftRight1(uint32_t enable);
	
	
	// Computes the multiplicative inverse of this number with respect to the given modulus.
	// The modulus must be odd and coprime to this number. This number must be less than the modulus.
	// If this number is zero, the reciprocal is zero. Constant-time with respect to this value.
	void reciprocal(const Uint256 &modulus);
	
	
	/* Miscellaneous methods */
	
	// Copies the given number into this number if enable is 1, or does nothing if enable is 0.
	// Constant-time with respect to both values and the enable.
	void replace(const Uint256 &other, uint32_t enable);
	
	
	// Swaps the value of this number with the given number if enable is 1, or does nothing if enable is 0.
	// Constant-time with respect to both values and the enable.
	void swap(Uint256 &other, uint32_t enable);
	
	
	// Writes this 256-bit integer as 32 bytes encoded in big endian to the given array.
	// Constant-time with respect to this value.
	void getBigEndianBytes(uint8_t b[NUM_WORDS * 4]) const;
	
	
	/* Equality/inequality operators */
	
	// Tests whether this number is equal to the given number. Constant-time with respect to both values.
	bool operator==(const Uint256 &other) const;
	
	bool operator!=(const Uint256 &other) const;
	
	
	// Tests whether this number is less than the given number. Constant-time with respect to both values.
	bool operator<(const Uint256 &other) const;
	
	bool operator<=(const Uint256 &other) const;
	
	bool operator>(const Uint256 &other) const;
	
	bool operator>=(const Uint256 &other) const;
	
	
	#undef NUM_WORDS
	
	
	
	/* Class constants */
	
public:
	static const Uint256 ZERO;
	static const Uint256 ONE;
	
};