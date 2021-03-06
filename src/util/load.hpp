/*
 * Copyright (C) 2011 Daniel Scharrer
 *
 * This software is provided 'as-is', without any express or implied
 * warranty.  In no event will the author(s) be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 */

#ifndef INNOEXTRACT_UTIL_LOAD_HPP
#define INNOEXTRACT_UTIL_LOAD_HPP

#include <stdint.h>
#include <cstring>
#include <iostream>
#include <string>

#include "util/endian.hpp"
#include "util/types.hpp"
#include "util/util.hpp"

struct binary_string {
	
	std::string & data;
	
	inline binary_string(std::string & target) : data(target) { }
	
	static void load(std::istream & is, std::string & target);
	
};

inline std::istream & operator>>(std::istream & is, const binary_string & str) {
	binary_string::load(is, str.data);
	return is;
}

void to_utf8(const std::string & from, std::string & to, uint32_t codepage = 1252);

struct encoded_string {
	
	std::string & data;
	uint32_t codepage;
	
	inline encoded_string(std::string & target, uint32_t _codepage)
		: data(target), codepage(_codepage) { }
	
	static void load(std::istream & is, std::string & target, uint32_t codepage);
	
};

inline std::istream & operator>>(std::istream & is, const encoded_string & str) {
	encoded_string::load(is, str.data, str.codepage);
	return is;
}

struct ansi_string : encoded_string {
	
	inline ansi_string(std::string & target) : encoded_string(target, 1252) { }
	
};

template <class T>
inline T load(std::istream & is) {
	T value;
	char buffer[sizeof(value)];
	is.read(buffer, sizeof(buffer));
	std::memcpy(&value, buffer, sizeof(value));
	return value;
}

template <class T, class Endianness>
inline T load_number(std::istream & is) {
	return Endianness::byteswap_if_alien(load<T>(is));
}

template <class T>
inline T load_number(std::istream & is) {
	return load_number<T, little_endian>(is);
}

template <class T, class Endianness>
T load_number(std::istream & is, size_t bits) {
	if(bits == 8) {
		return load_number<typename compatible_integer<T, 8>::type, Endianness>(is);
	} else if(bits == 16) {
		return load_number<typename compatible_integer<T, 16>::type, Endianness>(is);
	} else if(bits == 32) {
		return load_number<typename compatible_integer<T, 32>::type, Endianness>(is);
	} else {
		return load_number<typename compatible_integer<T, 64>::type, Endianness>(is);
	}
}

template <class T>
inline T load_number(std::istream & is, size_t bits) {
	return load_number<T, little_endian>(is, bits);
}

template <class T>
void discard(T & is, uint64_t bytes) {
	char buf[1024];
	while(bytes) {
		std::streamsize n = std::streamsize(std::min<uint64_t>(bytes, ARRAY_SIZE(buf)));
		is.read(buf, n);
		bytes -= uint64_t(n);
	}
}

#endif // INNOEXTRACT_UTIL_LOAD_HPP
