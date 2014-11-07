/*
 *  HashedString.cpp
 *  IslandWars
 *
 *  Created by Christopher Shankland on 2/10/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#include "HashedString.h"

namespace IslandWars
{
	
void * HashedString::hash_name( const std::string pIdentStr )
{
	char const * charIdentStr = pIdentStr.c_str();
	
	// largest prime smaller than 65536 = 2 ^ 16
	unsigned long BASE = 65521L;
	
	// NMAX is the largest n such that
	// 255n(n+1)/2 + (n+1)(BASE-1) <= 2^32 - 1
	unsigned long NMAX = 5552;
	
	// Macros to make the hash simpler to read
	#define DO1(buf, i) { s1 += tolower(buf[i]); s2 += s1; }
	#define DO2(buf, i) DO1(buf, i); DO1(buf, i + 1);
	#define DO4(buf, i) DO2(buf, i); DO2(buf, i + 2);
	#define DO8(buf, i) DO4(buf, i); DO4(buf, i + 4);
	#define DO16(buf) DO8(buf, 0); DO8(buf, 8);
	
	if (charIdentStr == NULL)
		return NULL;
	
	// Possbily have a check for a wildcard event here
	
	unsigned long s1 = 0;
	unsigned long s2 = 0;
	
	// Hashing algorithm is loosely based on the Adler-32 checksum
	for ( size_t len = strlen( charIdentStr ); len > 0; )
	{
		unsigned long k = len < NMAX ? len : NMAX;
		len -= k;
		while (k >= 16)
		{
			DO16(charIdentStr);
			charIdentStr += 16;
			k -= 16;
		}
		
		if (k != 0) do
		{
			s1 += *charIdentStr++;
			s2 += s1;
		} while (--k);
		
		s1 %= BASE;
		s2 %= BASE;
	}
	
	// Disable the cast warning before we do it
	#pragma warning(push)
	#pragma warning(disable : 4312)
	
	return reinterpret_cast<void *> ( (s2 << 16) | s1 );

	#pragma warning(pop)
	
	// Undefine the macros
	#undef DO1
	#undef DO2
	#undef DO4
	#undef DO8
	#undef DO16
	
}

}