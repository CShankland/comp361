/*
 *  EventManager.h
 *  IslandWars
 *
 *  Created by Christopher Shankland on 2/13/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#pragma once
#ifndef __EVENT_MANAGER_H__
#define __EVENT_MANAGER_H__

#include <boost/shared_ptr.hpp>
#include "HashedString.h"

namespace IslandWars
{
	
	typedef HashedString EventType;
	
	class IEventData;
	typedef boost::shared_ptr<IEventData> IEventDataPtr;
	
	class IEventData
	{
	public:
		virtual const EventType & VGetEventType( void ) const = 0;
		virtual float VGetTimeStamp() const = 0;
		virtual std::string VSerialize() = 0;
        int net_send;
	};
	
	class BaseEventData : public IEventData
	{
	public:
		explicit BaseEventData( const float timeStamp = 0.0f )
		: m_TimeStamp( timeStamp )	
		{
			net_send = 1;
		}
		
		virtual ~BaseEventData() { }
		
		virtual const EventType & VGetEventType( void ) const = 0;
		
		float VGetTimeStamp( void ) const
		{
			return m_TimeStamp;
		}
		
		virtual std::string VSerialize() = 0;
		
	protected:
		const float m_TimeStamp;
	};
	
	// Helper class for events that require no data
	class EmptyEventData : public BaseEventData
	{
	public:
		explicit EmptyEventData( const float timeStamp = 0.0f )
		: BaseEventData( timeStamp ), m_EventType("") { }
		virtual ~EmptyEventData() { }
		
		virtual const EventType & VGetEventType( void )
		{
			return m_EventType;
		}
		
	private:
		EventType m_EventType;
	};
	
	class IEventListener
	{
	public:

		virtual char const * GetName( void ) = 0;
		
		virtual bool HandleEvent( IEventData & event ) = 0;
	};
	
	typedef boost::shared_ptr<IEventListener> EventListenerPtr;
	
	class IEventManager
	{
	public:
		enum eConstants
		{
			kINFINITE = 0xffffffff
		};
		
		explicit IEventManager( char const * const pName, bool setAsGlobal );
		
		virtual ~IEventManager();
		
		virtual bool VAddListener( EventListenerPtr const & inHandler, EventType const & inType ) = 0;
		
		virtual bool VDelListener( EventListenerPtr const & inHandler, EventType const & inType ) = 0;
		
		virtual bool VTrigger( IEventData & inEvent ) const = 0;
		
		virtual bool VQueueEvent( IEventDataPtr & inEvent ) = 0;
		
		virtual bool VAbortEvent( EventType const & inType, bool allOfType = false ) = 0;
		
		virtual bool VTick( unsigned long maxMillis = kINFINITE ) = 0;
		
		virtual bool VValidateType( EventType const & inType ) const = 0;
		
	private:
		
		static IEventManager * Get();
		
		friend bool safeAddListener( EventListenerPtr const & inHandler, EventType const & inType );
		friend bool safeDelListener( EventListenerPtr const & inHandler, EventType const & inType );
		friend bool safeTriggerEvent( IEventData & inEvent );
		friend bool safeQueueEvent( IEventDataPtr & inEvent );
		friend bool safeAbortEvent( EventType const & inType, bool allOfType = false );
		friend bool safeTickEventManager( unsigned long maxMillis = IEventManager::kINFINITE );
		friend bool safeValidateEventType( EventType const & inType );
	};
	
	// Access methods
	
	bool safeAddListener( EventListenerPtr const & inHandler, EventType const & inType );
	bool safeDelListener( EventListenerPtr const & inHandler, EventType const & inType );
	bool safeTriggerEvent( IEventData & inEvent );
	bool safeQueueEvent( IEventDataPtr & inEvent );
	bool safeAbortEvent( EventType const & inType, bool allOfType );
	bool safeTickEventManager( unsigned long maxMillis );
	bool safeValidateEventType( EventType const & inType );
	
}

#endif