/*
 *  ToDoList.h
 *  GE
 *
 *  Created by Seraphin on 10-03-04.
 *  Copyright 2010 __MyCompanyName__. All rights reserved.
 *
 */
#ifndef _TO_DO_LIST_H_
#define _TO_DO_LIST_H_

#include <vector>
enum OpType { CREATE, MOVE };


namespace IslandWars {
	class Op {
	public:
		virtual void doIt() = 0;
		virtual OpType getOpType() = 0;
		
	};
	class ToDo {
	private:
		Op *action;
		//type of 
		int objectId;
		int day;
	public:
		ToDo( Op *oaction, int oId, int _day ) {
			action = oaction;
			objectId = oId;
			day = _day;
		}
		~ToDo() {}
		
		Op* getAction() {
			return action;	
		}
		
		void doIt() {
			action->doIt();
		}
		
		int getDay() { return day; }
		int getObjectId() { return objectId; }
	};
	
	class ToDoList {
	private:
		std::vector< ToDo* > toDoList;
	public:
		void add(ToDo *item) {
			toDoList.push_back(item);
		}
		
		ToDo* get(int x) {
			return toDoList[x];
		}
		
		int size() {
			return toDoList.size();
		}
		
		
	};
	
}
#endif