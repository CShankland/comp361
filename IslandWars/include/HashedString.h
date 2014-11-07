/*
 *  HashedString.h
 *  IslandWars
 *
 *  Created by Christopher Shankland on 2/13/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#pragma once
#ifndef __HASHED_STRING_H__
#define __HASHED_STRING_H__

#include <string>

namespace IslandWars
{
	
	class HashedString
	{
	public:
		
		explicit HashedString (const std::string pIdentStr) : m_ident( hash_name(pIdentStr) ), m_identStr(pIdentStr) { }
		
		unsigned long getIdent() const
		{
			return reinterpret_cast<unsigned long> (m_ident);
		}
		
		const std::string getStr() const { return m_identStr; }
		
		bool operator< ( HashedString const & o ) const
		{
			bool r = ( getIdent() < o.getIdent() );
			return r;
		}
		
		bool operator== ( HashedString const & o ) const
		{
			bool r = ( getIdent() == o.getIdent() );
			return r;
		}
		
		static void * hash_name( const std::string pIdentStr );
		
	private:
		
		// m_ident is stored as a void* not an int, so that the debugger
		// will show it as hex-values instead of integers.
		
		void *       m_ident;
		std::string m_identStr;
		
	};
}

#endif