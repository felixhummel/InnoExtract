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

#ifndef INNOEXTRACT_STREAM_BLOCK_HPP
#define INNOEXTRACT_STREAM_BLOCK_HPP

#include <ios>
#include <string>

#include <boost/shared_ptr.hpp>

namespace setup { struct version; }

namespace stream {

struct block_error : public std::ios_base::failure {
	
	inline block_error(std::string msg) : std::ios_base::failure(msg) { }
	
};

//! Reads a compressed and checksumed block of data used to store the setup headers.
class block_reader {
	
public:
	
	typedef std::istream type;
	typedef boost::shared_ptr<type> pointer;
	
	static pointer get(std::istream & base, const setup::version & version);
	
};

} // namespace stream

#endif // INNOEXTRACT_STREAM_BLOCK_HPP
