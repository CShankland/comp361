/*
 *  EventManagerImpl.h
 *  IslandWars
 *
 *  Created by Christopher Shankland on 2/17/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#pragma once
#ifndef __EVENT_MANAGER_IMPL_H__
#define __EVENT_MANAGER_IMPL_H__

#include "GraphicsCommon.h"
#include "EventManager.h"

#include <vector>
#include <list>
#include <map>
#include <set>

namespace IslandWars
{
	
	typedef std::vector<EventListenerPtr> EventListenerList;
	
	typedef std::vector<EventType> EventTypeList;
	
	class EventManager : public IEventManager
	{
	public:
		
		explicit EventManager( char const * const pName, bool setAsGlobal );
		
		virtual ~EventManager();
		
		virtual bool VAddListener( EventListenerPtr const & inListener, EventType const & inType );
		virtual bool VDelListener( EventListenerPtr const & inListener, EventType const & inType );
		virtual bool VTrigger( IEventData & inEvent ) const;
		virtual bool VQueueEvent( IEventDataPtr & inEvent );
		virtual bool VAbortEvent( EventType const & inType, bool allOfType );
		virtual bool VTick( unsigned long maxMillis );
		
		virtual bool VValidateType( EventType const & inType ) const;
		EventListenerList GetListenerList( EventType const & eventType ) const;
		EventTypeList GetTypeList( void ) const;
		
	private:
		
		class IRegisteredEvent
		{
		public:
			enum eRegisteredEventMetaData
			{
				kREMD_ScriptDefined,
				kREMD_CodeEventOnly,
				kREMD_CodeEventScriptCallable,
			};
			
			IRegisteredEvent( const eRegisteredEventMetaData metaData ) : m_MetaData( metaData )
			{
			}
			
			virtual ~IRegisteredEvent()
			{
			}
			
		private:
			const eRegisteredEventMetaData m_MetaData;
		};
		
		class CodeOnlyDefinedEvent : public IRegisteredEvent
		{
		public:
			explicit CodeOnlyDefinedEvent( void ) : IRegisteredEvent( IRegisteredEvent::kREMD_CodeEventOnly )
			{
			}
		};
		
		typedef boost::shared_ptr< IRegisteredEvent > IRegisteredEventPtr;
		
		void AddRegisteredEventType( const EventType & eventType, IRegisteredEventPtr metaData );
		
		typedef std::map< EventType, IRegisteredEventPtr >    EventTypeSet;
		typedef std::pair< EventTypeSet::iterator, bool >     EventTypeSetIRes;
		typedef std::list< EventListenerPtr >                 EventListenerTable;
		typedef std::map< unsigned int, EventListenerTable >  EventListenerMap;
		typedef std::pair< unsigned int, EventListenerTable > EventListenerMapEnt;
		typedef std::pair< EventListenerMap::iterator, bool > EventListenerMapIRes;
		typedef std::list< IEventDataPtr >                    EventQueue;
		
		enum eConstants
		{
			kNumQueues = 2
		};
		
		EventTypeSet     m_typeList;
		EventListenerMap m_registry;
		EventQueue       m_queues[kNumQueues];
		int              m_activeQueue;
	};
	
}

#endif