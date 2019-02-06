#include "AStarFindPath.h" 
#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <math.h>


/*!
\brief Класс описывающий операции над узлом 
*/

class MapSearchNode
{
public:
	int x;	 /*!< Координаты иследуемого узла*/
	int y;	
	
	MapSearchNode() { x = y = 0; }
	MapSearchNode( int px, int py ) { x=px; y=py; }

	float GoalDistanceEstimate( MapSearchNode &nodeGoal );
	bool IsGoal( MapSearchNode &nodeGoal );
	bool GetSuccessors( AStarFindPath<MapSearchNode> *astarfindpath, MapSearchNode *neighbor_node );
	float GetCost( MapSearchNode &successor );
	bool IsSameState( MapSearchNode &rhs );

	void PrintNodeInfo(); 


};

int MAP_WIDTH; /*!< Ширина*/
int MAP_HEIGHT; /*!< Высота*/
float world_map [1000][1000]; /*!< Массив чисел карты поиска*/
char input_map[1000][1000]; /*!< Массив символов для печати найденого пути*/

float GetMap( int x, int y )
{
	if( x < 0 ||
	    x >= MAP_WIDTH ||
		 y < 0 ||
		 y >= MAP_HEIGHT
	  )
	{
		return 9.0;	 
	}

	return world_map[x][y];
}
/*!
\brief Возвращает true, если этот узел совпадает с узлом rhs
\param[in] указатель на узел 
*/
bool MapSearchNode::IsSameState( MapSearchNode &rhs )
{


	if( (x == rhs.x) &&
		(y == rhs.y) )
	{
		return true;
	}
	else
	{
		return false;
	}

}
/*!< Распечатывает координаты пути */
void MapSearchNode::PrintNodeInfo()
{
	input_map[x][y]='*';
	char str[100];
	sprintf( str, "Координаты узла : (%d,%d)\n", x,y );
	
	cout << str;
}

/*!
\brief эвристическая функция, которая оценивает расстояние от узла до конечной цели
\param[in] указатель на конечный узел
*/

float MapSearchNode::GoalDistanceEstimate( MapSearchNode &nodeGoal )
{
	return  fabsf(x - nodeGoal.x) + fabsf(y - nodeGoal.y);	
}

/*!
  \brief Проверяет является ли текущий узел целью
  \param[in] текущий узел
  \return Возвращает true, если этот узел является целью
*/

bool MapSearchNode::IsGoal( MapSearchNode &nodeGoal )
{

	if( (x == nodeGoal.x) &&
		(y == nodeGoal.y) )
	{
		return true;
	}

	return false;
}

/*!
\brief Получает список всех соседей  этого узла и добавляет их через AStarFindPath.addSuccessor ()
*/
bool MapSearchNode::GetSuccessors( AStarFindPath<MapSearchNode> *astarfindpath, MapSearchNode *neighbor_node )
{

	int neighbor_x = -1; 
	int neighbor_y = -1; 

	if( neighbor_node )
	{
		neighbor_x = neighbor_node->x;
		neighbor_y = neighbor_node->y;
	}
	

	MapSearchNode NewNode;
	world_map[x][y]=1.0;
	

	if( (GetMap( x-1, y ) !=9.0) 
		&& !((neighbor_x == x-1) && (neighbor_y == y))
	  ) 
	{
		NewNode = MapSearchNode( x-1, y );
		astarfindpath->AddSuccessor( NewNode );
		world_map[x-1][y]=1.0;
	}	

	if( (GetMap( x, y-1 ) !=9.0) 
		&& !((neighbor_x == x) && (neighbor_y == y-1))
	  ) 
	{
		NewNode = MapSearchNode( x, y-1 );
		astarfindpath->AddSuccessor( NewNode );
		world_map[x][y-1]=1.0;
	}	

	if( (GetMap( x+1, y ) !=9.0)
		&& !((neighbor_x == x+1) && (neighbor_y == y))
	  ) 
	{
		NewNode = MapSearchNode( x+1, y );
		astarfindpath->AddSuccessor( NewNode );
		world_map[x+1][y]=1.0;
	}	

		
	if( (GetMap( x, y+1 ) !=9.0) 
		&& !((neighbor_x == x) && (neighbor_y == y+1))
		)
	{
		NewNode = MapSearchNode( x, y+1 );
		astarfindpath->AddSuccessor( NewNode );
		world_map[x][y+1]=1.0;
	}	

		if( (GetMap( x-1, y+1 ) !=9.0) 
		&& !((neighbor_x == x-1) && (neighbor_y == y+1))
	  ) 
	{
		NewNode = MapSearchNode( x-1, y+1 );
		astarfindpath->AddSuccessor( NewNode );
		world_map[x-1][y+1]=1.4;
	}	

	if( (GetMap( x+1, y+1 ) !=9.0) 
		&& !((neighbor_x == x+1) && (neighbor_y == y+1))
	  ) 
	{
		NewNode = MapSearchNode( x+1, y+1 );
		astarfindpath->AddSuccessor( NewNode );
		world_map[x+1][y+1]=1.4;
	}	

	if( (GetMap( x+1, y-1 ) !=9.0)
		&& !((neighbor_x == x+1) && (neighbor_y == y-1))
	  ) 
	{
		NewNode = MapSearchNode( x+1, y-1 );
		astarfindpath->AddSuccessor( NewNode );
		world_map[x+1][y-1]=1.4;
	}	

		
	if( (GetMap( x-1, y-1 ) !=9.0) 
		&& !((neighbor_x == x-1) && (neighbor_y == y-1))
		)
	{
		NewNode = MapSearchNode( x-1, y-1 );
		astarfindpath->AddSuccessor( NewNode );
		world_map[x-1][y-1]=1.4;
	}	

	return true;
}

/*!
\brief Получаем стоимость передвежения по клетки 
*/

float MapSearchNode::GetCost( MapSearchNode &successor )
{
	
	if(world_map[x][y]=1.4)
	{return  1.4;}
	if(world_map[x][y]=1.0)
	{return  1.0;};

}