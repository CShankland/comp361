/*
 *  EventManagerImpl.cpp
 *  IslandWars
 *
 *  Created by Christopher Shankland on 2/17/10.
 *  Copyright 2010 Comp361. All rights reserved.
 *
 */

#include "EventManagerImpl.h"
#include "Events.h"
#include <iostream.h>

namespace IslandWars
{
	
	EventManager::EventManager( char const * const pName, bool setAsGlobal ) : IEventManager( pName, setAsGlobal ), m_activeQueue(0)
	{
		// Only need to do something here if scripting is going to be implemented
	}
	
	EventManager::~EventManager()
	{
		m_activeQueue = 0;
	}
	
	bool EventManager::VAddListener(EventListenerPtr const & inListener, EventType const & inType)
	{	
		if ( ! VValidateType(inType))
			return false;
		
		// check / update type list
		EventTypeSet::iterator evIt = m_typeList.find(inType);
		
		// find listener map entry, create one if no table already exists for this event type
		EventListenerMap::iterator elmIt = m_registry.find(inType.getIdent());
		
		if (elmIt == m_registry.end())
		{
			EventListenerMapIRes elmIRes = m_registry.insert(EventListenerMapEnt(inType.getIdent(), EventListenerTable()));
			
			// Error inserting into map
			if (elmIRes.second == false)
			{
				return false;
			}
			
			// Inserted and created an empty table.  Shouldn't be possible.
			if (elmIRes.first == m_registry.end())
			{
				return false;
			}
			
			// Store the new table, so we can update it
			elmIt = elmIRes.first;
		}
		
		// Update the mapped list of listeners, walk the exisiting list (if there are any) to prevent duplicate
		// entries.  This is a bit of a cost at registry time, but it helps to prevent listeners from listening to 
		// the same event twice.
		
		// note: use reference to make the following code more simple
		EventListenerTable & evlTable = (*elmIt).second;
		
		for (EventListenerTable::iterator it = evlTable.begin(), itEnd = evlTable.end(); it != itEnd; it++)
		{
			bool bListenerMatch = (*it == inListener);
			
			if (bListenerMatch)
				return false;
		}
		
		// Everything (event type, event listener) has been validated, so add it to the map
		evlTable.push_back(inListener);
		
		return true;
	}
	
	bool EventManager::VDelListener(EventListenerPtr const & inListener, EventType const & inType)
	{
		if (!VValidateType(inType))
			return false;
		
		bool rc = false;
		
		// Just simple brute force search for the mapping
		for (EventListenerMap::iterator it = m_registry.begin(), itEnd = m_registry.end(); it != itEnd; it++)
		{
			unsigned int const kEventId = it -> first;
			EventListenerTable & table = it -> second;
			
			for (EventListenerTable::iterator it2 = table.begin(), it2End = table.end(); it2 != it2End; it2++)
			{
				if (*it2 == inListener)
				{
					// Found the pairing, so remove it
					table.erase(it2);
					
					// Set the return value
					rc = true;
					
					// And exit, since each listener / type pairing is guaranteed unique by the add method
					break;
				}
			}
		}
		
		return rc;
	}
	
	bool EventManager::VTrigger(IEventData & inEvent) const
	{
		if (!VValidateType(inEvent.VGetEventType()))
			return false;
		
		EventListenerMap::const_iterator itWC = m_registry.find(0);
		
		if (itWC != m_registry.end())
		{
			EventListenerTable const & table = itWC -> second;
			
			bool processed = false;
			
			for (EventListenerTable::const_iterator it2 = table.begin(), it2End = table.end(); it2 != it2End; it2++)
			{
				(*it2) -> HandleEvent(inEvent);
			}
		}
		
		EventListenerMap::const_iterator it = m_registry.find(inEvent.VGetEventType().getIdent());
		
		if (it == m_registry.end())
			return false;
		
		EventListenerTable const & table = it -> second;
		
		bool processed = false;
		
		for (EventListenerTable::const_iterator it2 = table.begin(), it2End = table.end(); it2 != it2End; it2++)
		{
			EventListenerPtr listener = *it2;
						
			if (listener -> HandleEvent(inEvent))
			{
				// Only set to true if the listener stopped the propogation
				processed = true;
			}
		}
		
		return processed;
	}
	
	bool EventManager::VQueueEvent(IEventDataPtr & inEvent)
	{	
		assert(m_activeQueue >= 0);
		assert(m_activeQueue < kNumQueues);
		
		if (!VValidateType(inEvent -> VGetEventType()))
		{
			return false;
		}
		
		EventListenerMap::const_iterator it = m_registry.find(inEvent -> VGetEventType().getIdent());
		
		if (it == m_registry.end())
		{
			// If there is no global listener, then abort the queue add
			EventListenerMap::const_iterator itWC = m_registry.find(0);
			
			if (itWC == m_registry.end())
			{
				// No listeners for the event, no need to add it
				return false;
			}
		}
		
		
		m_queues[m_activeQueue].push_back(inEvent);
		
		return true;
	}
	
	bool EventManager::VAbortEvent(EventType const & inType, bool allOfType)
	{
		assert(m_activeQueue >= 0);
		assert(m_activeQueue < kNumQueues);
		
		if (!VValidateType(inType))
			return false;
		
		EventListenerMap::iterator it = m_registry.find(inType.getIdent());
		
		// If there are no listeners for the event type, skip this request
		if (it == m_registry.end())
			return false;
		
		bool rc = false;
		
		EventQueue &evtQueue = m_queues[m_activeQueue];
		
		for (EventQueue::iterator it = evtQueue.begin(), itEnd = evtQueue.end(); it != itEnd; it++)
		{
			if ((*it) -> VGetEventType() == inType)
			{
				it = evtQueue.erase(it);
				rc = true;
				if (!allOfType)
					break;
			}
			else
			{
				++it;
			}
		}
		
		return rc;
	}
	
	bool EventManager::VTick(unsigned long maxMillis)
	{
		// TODO: Add some way to check when this method is called (like... getTime() or something)
		unsigned long curMs = IEventManager::kINFINITE;//GetTickCount();
		unsigned long maxMs = (maxMillis == IEventManager::kINFINITE) ? IEventManager::kINFINITE : (curMs + maxMillis);
		
		EventListenerMap::const_iterator itWC = m_registry.find(0);
		
		// Swap active queues, and make sure the new queue is empty after the swap
		int queueToProcess = m_activeQueue;
		m_activeQueue = (m_activeQueue + 1) % kNumQueues;
		m_queues[m_activeQueue].clear();
		
		// Now start processing until either the time limit is up, or all the events are consumed
		
		while (m_queues[queueToProcess].size() > 0)
		{
			IEventDataPtr event = m_queues[queueToProcess].front();
						
			m_queues[queueToProcess].pop_front();
			
			EventType const & eventType = event -> VGetEventType();
			
			EventListenerMap::const_iterator itListeners = m_registry.find(eventType.getIdent());
			
			
			if (itWC != m_registry.end())
			{
				EventListenerTable const & table = itWC -> second;
				bool processed = false;
				
				
				for (EventListenerTable::const_iterator it2 = table.begin(), it2End = table.end(); it2 != it2End; it2++)
				{
					(*it2) -> HandleEvent(*event);
				}
			}
			
			// No listeners for the event, so skip it
			if (itListeners == m_registry.end())
				continue;
			
			unsigned int const kEventId = itListeners -> first;
			EventListenerTable const & table = itListeners -> second;
			
			for (EventListenerTable::const_iterator it = table.begin(), itEnd = table.end(); it != itEnd; it++)
			{
				 
				if ((*it)->HandleEvent(*event))
				{
					break;
				}
				
			}
			
			// TODO:  Update the time here....
			//curMs = GetTickCount();
			
			if (maxMillis != IEventManager::kINFINITE)
			{
				if (curMs >= maxMs)
				{
					// Time is up, stop processing
					break;
				}
			}
		}
		
		// Move any remaining events to the new current queue
		//
		// And to preserve the timing of their addition, they are added to the head in reverse order
		
		bool queueFlushed = (m_queues[queueToProcess].size() == 0);
		
		if (!queueFlushed)
		{
			while (m_queues[queueToProcess].size() > 0)
			{
				IEventDataPtr event = m_queues[queueToProcess].back();
				m_queues[queueToProcess].pop_back();
				m_queues[m_activeQueue].push_front(event);
			}
		}
		
		// Processing finished
		return queueFlushed;
	}
	
	bool EventManager::VValidateType(EventType const & inType) const
	{
		if (0 == inType.getStr().length())
			return false;
		
		// If there is a wildcard event type, add a check that it is not equal to that here...
		if (inType.getIdent() == 0)
			return false;
		
		EventTypeSet::const_iterator evIt = m_typeList.find(inType);
		
		if (evIt != m_typeList.end())
		{
			return false;
		}
		
		return true;
	}
	
	EventListenerList EventManager::GetListenerList(EventType const & eventType) const
	{
		// No listeners to invalid event types
		if (!VValidateType(eventType))
			return EventListenerList();
		
		EventListenerMap::const_iterator itListeners = m_registry.find(eventType.getIdent());
		
		// No listeners for this event
		if (itListeners == m_registry.end())
			return EventListenerList();
		
		EventListenerTable const & table = itListeners -> second;
		
		// There used to be listeners, but there aren't anymore
		if (table.size() == 0)
			return EventListenerList();
		
		EventListenerList result;
		result.reserve(table.size());
		
		for (EventListenerTable::const_iterator it = table.begin(), itEnd = table.end(); it != itEnd; it++)
		{
			result.push_back(*it);
		}
		
		return result;
	}
	
	EventTypeList EventManager::GetTypeList(void) const
	{
		// No entries, nothing to report :(
		if (m_typeList.size() == 0)
			return EventTypeList();
		
		EventTypeList result;
		result.reserve(m_typeList.size());
		
		for (EventTypeSet::const_iterator it = m_typeList.begin(), itEnd = m_typeList.end(); it != itEnd; it++)
		{
			result.push_back(it -> first);
		}
		
		return result;
	}
	
	void EventManager::AddRegisteredEventType( const EventType & eventType, IRegisteredEventPtr metaData )
	{
		const EventTypeSet::const_iterator iter = m_typeList.find( eventType );
		if ( iter != m_typeList.end() )
		{
			// Error!
		} 
		else
		{
			m_typeList.insert( std::make_pair( eventType, metaData ) );
		}
	}
}