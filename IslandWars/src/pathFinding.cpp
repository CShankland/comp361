/*
 *  pathFinding.cpp
 *  
 *
 *  Created by Simon Hlywa on 10-03-29.
 *
 */

#include "pathFinding.h"
#include <iostream>
#include <cmath>

namespace IslandWars
{    
    //////////////////////////////FIND PATH//////////////////////////////////////////////////////////////
    //                                                                                                 //
    //      Description: This is the function you want to call. It will return the path. It calls      //
    //                  expansion, which does most of the grunt work. What it gets back is a coord     //
    //                  describing where the two expansion horizons meet, or in other words, the       //
    //                  midpoint of the path. It also populates Map with weights given to each         //
    //                  coordinate. This function greedily traverses from the midpoint to the start    //
    //                  using the weights given, then traverses from the midpoint to the end,          //
    //                  pushing the coordinates used onto the front and back of the list,              //
    //                  respectively.                                                                  //
    //                                                                                                 //
    //      Arguments: Vector2 starting coordinate, Vector2 destination coordinate, int UNIT_ID        //
    //                                                                                                 //
    //      Return Value: A list of Vector2s that describes the shortest path from start to dest       //
    //                    If the list is empty, it means that no path could be found.                  //
    //                                                                                                 //
    /////////////////////////////////////////////////////////////////////////////////////////////////////
	std::list<Vector2> PathFinding::findPath (Vector2 start, Vector2 dest, UnitClass unitClass)
    {
		//std::cout << "Going from: " << start.x <<","<<start.y<<" to "<<dest.x<<","<<dest.y<<"\n";
        int i, j;
		std::list <Vector2> path;
        
        for (i = 0; i < ROW_SIZE; i++)
			for (j = 0; j < COL_SIZE; j++)
				map[i][j] = 65000;
        
        getAdjacent(start);
        
        for (i = 0; i < 6; i++)
            if (curr_adjcnt[i].x == dest.x && curr_adjcnt[i].y == dest.y)
            {
                path.push_front(start);
                path.push_back(dest);
                PrintPath(path);
                return path;
            }
        
        
        Vector2 midpoint = expansion(start, dest, unitClass);
        if (midpoint.x == -1 || midpoint.y == -1)
        {
            return pathRecovery(start, dest);
        }
        
        Vector2 curr_pos;
        curr_pos.x = midpoint.x;
        curr_pos.y = midpoint.y;
        
        
        while (!(curr_pos.x == start.x && curr_pos.y == start.y))
        {
            getAdjacent(curr_pos);
            for (i = 0; i < 6; i++)
            {
                if (curr_adjcnt[i].x > -1 && curr_adjcnt[i].y > -1)
                {
                    int curr_pos_weight = map[curr_pos.x][curr_pos.y];
                    int curr_adjcnt_weight = map[curr_adjcnt[i].x][curr_adjcnt[i].y];
                    
                    if (!(curr_adjcnt[i].x == dest.x && curr_adjcnt[i].y == dest.y) && 
                        ((curr_pos_weight > 0 && curr_adjcnt_weight == curr_pos_weight - 1)||
                         (curr_pos_weight < 0 && curr_adjcnt_weight == findMin(dest) && curr_adjcnt_weight < 65000) ||
                         (curr_adjcnt_weight == 0)))
                    {
                        path.push_front(curr_adjcnt[i]);
                        curr_pos = curr_adjcnt[i];
                        i = 6;
                    }
                }
            }
        }
        
        path.push_back(midpoint);
        curr_pos.x = midpoint.x;
        curr_pos.y = midpoint.y;
        
        while (!(curr_pos.x == dest.x && curr_pos.y == dest.y))
        {
            
            getAdjacent(curr_pos);
            for (i = 0; i < 6; i++)
            {
                if (curr_adjcnt[i].x > -1 && curr_adjcnt[i].y > -1)
                {
                    int curr_pos_weight = map[curr_pos.x][curr_pos.y];
                    int curr_adjcnt_weight = map[curr_adjcnt[i].x][curr_adjcnt[i].y];
                    
                    if (!(curr_adjcnt[i].x == start.x && curr_adjcnt[i].y == start.y) && 
                        ((curr_pos_weight < 0 && curr_adjcnt_weight == curr_pos_weight + 1)||
                         (curr_pos_weight > 0 && curr_adjcnt_weight == findMax(start)) ||
                         (curr_adjcnt_weight == 0)))
                    {
                        path.push_back(curr_adjcnt[i]);
                        curr_pos = curr_adjcnt[i];
                        i = 6;
                    }
                }
            }
        }
        
        PrintPath(path);
        return path;
    }
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //////////////////////////////////////////////EXPANSION//////////////////////////////////////////////////
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////
    
    Vector2 PathFinding::expansion (Vector2 start_point, Vector2 end_point, UnitClass unit)
    {
		std::list <Vector2> start_hrzn;
		std::list <Vector2> dest_hrzn;
        Vector2 temp_coord;
        start_hrzn.push_front(start_point);
        dest_hrzn.push_front(end_point);
        
        map[start_point.x][start_point.y] = 0;
        map[end_point.x][end_point.y] = 0;
        
        while (!start_hrzn.empty())
        {
			std::list <Vector2> exp_hrzn = start_hrzn;
            
            for (int is_start_hrzn = 1; is_start_hrzn >= 0; is_start_hrzn--)
            {
				std::list <Vector2> temp_hrzn;
                
                while (!exp_hrzn.empty())
                {
                    
                    getAdjacent(exp_hrzn.front());
                    
                    for (int i = 0; i < 6; i++)
                    {
                        
                        if (curr_adjcnt[i].x > -1 && curr_adjcnt[i].y > -1)
                        {
                            if (is_start_hrzn)
                            {
                                if (find(dest_hrzn, curr_adjcnt[i]))
                                    return curr_adjcnt[i];
                            }
                            else
                            {
                                if (find(start_hrzn, curr_adjcnt[i]))
                                    return curr_adjcnt[i];
                            }
                            
                            if (map[curr_adjcnt[i].x][curr_adjcnt[i].y] == 65000)
                            {
                                if (isTraversable(unit, curr_adjcnt[i]))//external_map[curr_adjcnt] == unit_type)//can the unit go on the terrain?
								{
									if (is_start_hrzn)
									{
										//std::cout << "Weight changing this spot: "<<curr_adjcnt[i].x<<","<<curr_adjcnt[i].y<<"\n";
										map[curr_adjcnt[i].x][curr_adjcnt[i].y] = map[exp_hrzn.front().x][exp_hrzn.front().y] +1;
									}
									else
									{
										map[curr_adjcnt[i].x][curr_adjcnt[i].y] = map[exp_hrzn.front().x][exp_hrzn.front().y] -1;
									}
									temp_hrzn.push_front(curr_adjcnt[i]);
								}
								else
								{
									//std::cout << "Infinite weight this spot: "<<curr_adjcnt[i].x<<","<<curr_adjcnt[i].y<<"\n";
									map[curr_adjcnt[i].x][curr_adjcnt[i].y] = 65003;
								}
							}
						}
					}
					exp_hrzn.pop_front();
				}
				if (is_start_hrzn)
				{
					start_hrzn = temp_hrzn;
					exp_hrzn = dest_hrzn;
				}
				else 
					dest_hrzn = temp_hrzn;
			}
		}
		temp_coord.x = -1;
		temp_coord.y = -1;
		return temp_coord;
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	///////////////////////////////////////////GETADJACENT///////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	void PathFinding::getAdjacent (Vector2 target)
	{
		int x = target.x, y = target.y, i;
        
        if (x % 2 == 1)
        {
            curr_adjcnt[0].x = x;
            curr_adjcnt[0].y = y+1;
            curr_adjcnt[1].x = x+1;
            curr_adjcnt[1].y = y;
            curr_adjcnt[2].x = x-1;
            curr_adjcnt[2].y = y;
            for (i = -1; i < 2; i++)
            {
                curr_adjcnt[i+4].x = x+i;
                curr_adjcnt[i+4].y = y-1;
            }
        }
        
        if (x % 2 == 0)
        {
            for (i = -1; i < 2; i++)
            {
                curr_adjcnt[i+1].x = x+i;
                curr_adjcnt[i+1].y = y+1;
            }
            
            curr_adjcnt[3].x = x-1;
            curr_adjcnt[3].y = y;
            curr_adjcnt[4].x = x;
            curr_adjcnt[4].y = y-1;
            curr_adjcnt[5].x = x+1;
            curr_adjcnt[5].y = y;
        }
	}
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////FIND////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool PathFinding::find (std::list <Vector2> search_list, Vector2 target)
	{
		std::list<Vector2>::iterator i;
		
		for (i = search_list.begin(); i != search_list.end(); ++i)
		{
			if (target.x == (*i).x && target.y == (*i).y)
				return true;
		}
		return false;
	}
	
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	////////////////////////////////////////////////isTraversable////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////
	bool PathFinding::isTraversable(UnitClass unit, Vector2 tile)
	{
		if (tile.x > 39 || tile.y > 39 || tile.x < 0 || tile.y < 0)
			return false;
		Terrain tile_terrain = GameEngine::get() -> getHexagon(tile.x, tile.y) -> getTerrainType();
		if (unit == UC_LAND)
			return (tile_terrain == T_LAND || tile_terrain == T_LAND2);
		if (unit == UC_SEA)
			return (tile_terrain == T_SEA || tile_terrain == T_SEA2);
		return true;
	}
    
    void PathFinding::PrintPath (std::list <Vector2> path)
    {
        //std::cout << "Path: \n";
        Vector2 curr;
        while (!path.empty())
        {
            curr = path.front();
			path.pop_front();
           // std::cout << "("<<curr.x<<","<<curr.y<<")\n";
        }
        return;
    }
    
    int PathFinding::findMin(Vector2 choord)
    {
        int min = 65000;
		int curr;
        for (int i = 0; i < 6; i++)
		{
			curr = map[curr_adjcnt[i].x][curr_adjcnt[i].y];
            if (curr < min && curr > -1 && curr < 65000 && !(curr_adjcnt[i].x == choord.x && curr_adjcnt[i].y == choord.y))
                min = map[curr_adjcnt[i].x][curr_adjcnt[i].y];
		}
        return min;
    }
    
    int PathFinding::findMax(Vector2 choord)
    {
        int max = -65000;
        for (int i = 0; i < 6; i++)
            if (map[curr_adjcnt[i].x][curr_adjcnt[i].y] > max && map[curr_adjcnt[i].x][curr_adjcnt[i].y] < 1 && !(curr_adjcnt[i].x == choord.x && curr_adjcnt[i].y == choord.y))
                max = map[curr_adjcnt[i].x][curr_adjcnt[i].y];
        return max;
    }
    
    std::list <Vector2> PathFinding::pathRecovery(Vector2 start, Vector2 dest)
    {
        Vector2 iter = start;
        Vector2 closest = start;
        
        bool xgreater = (dest.x > start.x);
        bool ygreater = (dest.y > start.y);
        
        while (iter.x < 40 && iter.y < 40 && iter.x > -1 && iter.y > -1)
        {
            if (map[iter.x][iter.y] > 0 && map[iter.x][iter.y] <  65000)
                closest = iter;
            if (abs(dest.x - iter.x) == abs(dest.y - iter.y))
            {
                if (xgreater)
                    iter.x++;
                else
                    iter.x--;

                if (ygreater)
                    iter.y++;
                else
                    iter.y--;
            }
            else{
                if (abs(dest.x - iter.x) > abs(dest.y - iter.y))
                {
                    if (xgreater)
                        iter.x++;
                    else
                        iter.x--;
                }
                else
                {
                    if (ygreater)
                        iter.y++;
                    else
                        iter.y--;            
                }
            }
        }
        
        Vector2 curr_pos = closest;
        std::list <Vector2> path;
        
        while (!(curr_pos.x == start.x && curr_pos.y == start.y))
        {            getAdjacent(curr_pos);
            for (int i = 0; i < 6; i++)
            {
                if (curr_adjcnt[i].x > -1 && curr_adjcnt[i].y > -1)
                {
                    int curr_pos_weight = map[curr_pos.x][curr_pos.y];
                    int curr_adjcnt_weight = map[curr_adjcnt[i].x][curr_adjcnt[i].y];
                    
                    if (!(curr_adjcnt[i].x == dest.x && curr_adjcnt[i].y == dest.y) && 
                        ((curr_pos_weight > 0 && curr_adjcnt_weight == curr_pos_weight - 1)||
                         (curr_pos_weight < 0 && curr_adjcnt_weight == findMin(dest) && curr_adjcnt_weight < 65000) ||
                         (curr_adjcnt_weight == 0)))
                    {
                        path.push_front(curr_pos);
                        curr_pos = curr_adjcnt[i];
                        i = 6;
                    }
                }
            }
        }
        
        path.push_front(start);
        PrintPath(path);
        return path;
    }
        
}
























