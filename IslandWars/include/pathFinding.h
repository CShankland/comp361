/*
 *  pathFinding.h
 *  
 *
 *  Created by Simon Hlywa on 10-03-29.
 *  Copyright 2010 Webber Academy. All rights reserved.
 *
 */
#include <list>
#include "Unit.h"
#include "Hexagon.h"
#include "GameEngine.h"

#ifndef DEFAULT_MAP_SIZE
#define DEFAULT_MAP_SIZE 1600
#endif

#ifndef ROW_SIZE
#define ROW_SIZE 40
#endif

#ifndef COL_SIZE
#define COL_SIZE 40
#endif

namespace IslandWars
{
    class PathFinding {
    private:
        int map[ROW_SIZE][COL_SIZE];
        Vector2 curr_adjcnt[6];
        Vector2 expansion (Vector2 start_point, Vector2 end_point, UnitClass unit);
        void getAdjacent (Vector2 target);
        bool find (std::list <Vector2> search_list, Vector2 target);
        bool isTraversable(UnitClass unit, Vector2 tile);
        int findMax(Vector2 choord);
        int findMin(Vector2 choord);
        std::list <Vector2> pathRecovery (Vector2 start, Vector2 dest);
    public:
        std::list <Vector2> findPath (Vector2 start, Vector2 dest, UnitClass unitClass);
		void PrintPath (std::list <Vector2> path);
    };
    
}