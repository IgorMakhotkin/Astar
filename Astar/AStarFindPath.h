
#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <algorithm>
#include <set>
#include <vector>
#include <cfloat>

using namespace std;



template <class T> class AStar;

/*!< Класс поиска */
template <class UserState> class AStarFindPath
{

public: 
		
	enum
	{
	
		SEARCH_SUCCEEDED,
		SEARCH_FAILED,
		SEARCH_OUT_OF_MEMORY,
		SEARCH_SEARCHING,
		
	};


	

	public:

	class Node
	{
		public:

			Node *neighbor; 
			Node *following; 
			
			float g; /*!< стоимость перемещения из рассматриваемого узла в соседний */
			float h; /*!< эвристическая оценка расстояния до цели */
			float f; /*!< суммарная стоимость пути по клетки */

			Node() :
				neighbor( 0 ),
				following( 0 ),
				g( 0.0f ),
				h( 0.0f ),
				f( 0.0f )
			{			
			}

			UserState m_UserState;
	};
		

	/*!
	\brief Функция сравнения, сравнивающая два узла 
	*/

	class HeapCompare_f 
	{
		public:

			bool operator() ( const Node *x, const Node *y ) const
			{
				return x->f > y->f;
			}
	};


public: /*! методы */

	
	/*!
	 \brief Установливает начальные и конечные координаты точек
	 \param[in] указатель на стартовую точку
     \param[in] указатель на начальную точку
	*/

	void SetStartAndGoalStates( UserState &Start, UserState &Goal )
	{
		m_CancelRequest = false;

		m_Start = AllocateNode();
		m_Goal = AllocateNode();

		assert((m_Start != NULL && m_Goal != NULL));
		
		m_Start->m_UserState = Start;
		m_Goal->m_UserState = Goal;

		m_State = SEARCH_SEARCHING;

		m_Start->g = 0; 
		m_Start->h = m_Start->m_UserState.GoalDistanceEstimate( m_Goal->m_UserState );
		m_Start->f = m_Start->g + m_Start->h;
		m_Start->neighbor = 0;

		/*!< Помещаем стартовое значение в открытый список */

		m_OpenList.push_back( m_Start ); 

		/*!< Сортируем список по значению f */
		push_heap( m_OpenList.begin(), m_OpenList.end(), HeapCompare_f() );

		/*!< Инициализатор для шагов поиска */
		m_Steps = 0;
	}

	/*!< Поиск шага */
	unsigned int SearchStep()
	{
	
		/*!< Если список оказался пустым раньше,чем нашелся путь или закончились узлы для проверки, то прерываем поиск */
		if( m_OpenList.empty() || m_CancelRequest )
		{
			FreeAllNodes();
			m_State = SEARCH_FAILED;
			return m_State;
		}
		
		/*!< Увеличиваем счетчик шагов */
		m_Steps ++;

		/*!< Выбираем узел с найменьшим f */
		Node *n = m_OpenList.front(); // получить указатель на узел
		pop_heap( m_OpenList.begin(), m_OpenList.end(), HeapCompare_f() );
		m_OpenList.pop_back();

		/*!< Проеверяем не является ли узел конеченым */
		if( n->m_UserState.IsGoal( m_Goal->m_UserState ) )
		{
			/*!< Используем узел цели, чтобы скопировать соседний указатель */
			m_Goal->neighbor = n->neighbor;
			m_Goal->g = n->g;

			
			if( false == n->m_UserState.IsSameState( m_Start->m_UserState ) )
			{
				FreeNode( n );

				/*!< устанавливаем указать в следующием узле*/
				Node *nodeFollowing = m_Goal;
				Node *nodeNeighbor = m_Goal->neighbor;

				do 
				{
					nodeNeighbor->following = nodeFollowing;

					nodeFollowing = nodeNeighbor;
					nodeNeighbor = nodeNeighbor->neighbor;
				
				} 
				while( nodeFollowing != m_Start ); // 

			}

			/*!< Удаляем узлы которые не подходят для пути*/
			FreeUnusedNodes();

			m_State = SEARCH_SUCCEEDED;

			return m_State;
		}
		else  
		{

		

			m_Successors.clear(); /*!< отчищаем вектор для узлов приемников */

			/*!< Функция для добавления узла-приемника  AddSuccessor*/
			bool ret = n->m_UserState.GetSuccessors( this, n->neighbor ? &n->neighbor->m_UserState : NULL ); 

			if( !ret )
			{

			    typename vector< Node * >::iterator successor;

				/*!< удаляем узлы добавленые ранее*/
				for( successor = m_Successors.begin(); successor != m_Successors.end(); successor ++ )
				{
					FreeNode( (*successor) );
				}

				m_Successors.clear(); /*!< отчищаем вектор для узлов приемников */

				/*!< Освобождаем память*/

				m_State = SEARCH_OUT_OF_MEMORY;
				return m_State;
			}
			
			/*!< Проверяем каждого приемника текущего узла*/
			for( typename vector< Node * >::iterator successor = m_Successors.begin(); successor != m_Successors.end(); successor ++ )
			{

				/*!< 	передает значение g для узла приемника*/
				float newg = n->g + n->m_UserState.GetCost( (*successor)->m_UserState );

				/*!< Проверяем находится ли узел в открытом или закрытом списке
				 Проводим поиск по открытому списку*/

				typename vector< Node * >::iterator openlist_result;

				for( openlist_result = m_OpenList.begin(); openlist_result != m_OpenList.end(); openlist_result ++ )
				{
					if( (*openlist_result)->m_UserState.IsSameState( (*successor)->m_UserState ) )
					{
						break;					
					}
				}

				if( openlist_result != m_OpenList.end() )
				{

					/*!< Обнаружили текущий узел в открытом списке*/

					if( (*openlist_result)->g <= newg )
					{
						FreeNode( (*successor) );

						
						continue;
					}
				}

				typename vector< Node * >::iterator closedlist_result;

				for( closedlist_result = m_ClosedList.begin(); closedlist_result != m_ClosedList.end(); closedlist_result ++ )
				{
					if( (*closedlist_result)->m_UserState.IsSameState( (*successor)->m_UserState ) )
					{
						break;					
					}
				}

				if( closedlist_result != m_ClosedList.end() )
				{

					/*!< Обнаружили текущий узел в закрытом списке*/

					if( (*closedlist_result)->g <= newg )
					{
						
						FreeNode( (*successor) );

						continue;
					}
				}

				/*!< Этот узел является лучшим узлом до сих пор с этим конкретным состоянием*/

				(*successor)->neighbor = n;
				(*successor)->g = newg;
				(*successor)->h = (*successor)->m_UserState.GoalDistanceEstimate( m_Goal->m_UserState );
				(*successor)->f = (*successor)->g + (*successor)->h;

				/*!< Удаляем узел-преемник из закрытого списка, если он был в нем*/

				if( closedlist_result != m_ClosedList.end() )
				{
					/*!< удаление*/
					FreeNode(  (*closedlist_result) ); 
					m_ClosedList.erase( closedlist_result );

				
					
				}

				/*!< Обновляем список*/
				if( openlist_result != m_OpenList.end() )
				{	   

					FreeNode( (*openlist_result) ); 
			   		m_OpenList.erase( openlist_result );
					make_heap( m_OpenList.begin(), m_OpenList.end(), HeapCompare_f() );
			
				}

			
				m_OpenList.push_back( (*successor) );

				/*!< Снова сортируем список*/
				push_heap( m_OpenList.begin(), m_OpenList.end(), HeapCompare_f() );

			}

			

			m_ClosedList.push_back( n );

		} 

 		return m_State; 

	}

	/*!< Вызываем метод для передачи узла приемника в список приемников*/

	bool AddSuccessor( UserState &State )
	{
		Node *node = AllocateNode();

		if( node )
		{
			node->m_UserState = State;

			m_Successors.push_back( node );

			return true;
		}

		return false;
	}

	/*!< Удаляем все решения после нахождения результата*/

	void FreeSolutionNodes()
	{
		Node *n = m_Start;

		if( m_Start->following )
		{
			do
			{
				Node *del = n;
				n = n->following;
				FreeNode( del );

				del = NULL;

			} while( n != m_Goal );

			FreeNode( n ); /*!< Удаляем конечную точку*/

		}
		else
		{
			/*!< Если начальная точка совпадает с конечной, то удаляем начальный и конечный узел */
			FreeNode( m_Start );
			FreeNode( m_Goal );
		}

	}

	/*!< Функции для вычисления решения */

	/*!< Получить начальный узел */
	UserState *GetSolutionStart()
	{
		m_CurrentSolutionNode = m_Start;
		if( m_Start )
		{
			return &m_Start->m_UserState;
		}
		else
		{
			return NULL;
		}
	}
	
	/*!< Получить следующий узел */
	UserState *GetSolutionNext()
	{
		if( m_CurrentSolutionNode )
		{
			if( m_CurrentSolutionNode->following )
			{

				Node *following = m_CurrentSolutionNode->following;

				m_CurrentSolutionNode = m_CurrentSolutionNode->following;

				return &following->m_UserState;
			}
		}

		return NULL;
	}
	
	/*!< Получить конечный узел */
	UserState *GetSolutionEnd()
	{
		m_CurrentSolutionNode = m_Goal;
		if( m_Goal )
		{
			return &m_Goal->m_UserState;
		}
		else
		{
			return NULL;
		}
	}
	
	/*!< Итератор шага решения */
	UserState *GetSolutionPrev()
	{
		if( m_CurrentSolutionNode )
		{
			if( m_CurrentSolutionNode->neighbor )
			{

				Node *neighbor = m_CurrentSolutionNode->neighbor;

				m_CurrentSolutionNode = m_CurrentSolutionNode->neighbor;

				return &neighbor->m_UserState;
			}
		}

		return NULL;
	}

	/*!<Получите окончательную стоимость решения
 Возвращает FLT_MAX, если цель не определена или нет решения */

	float GetSolutionCost()
	{
		if( m_Goal && m_State == SEARCH_SUCCEEDED )
		{
			return m_Goal->g;
		}
		else
		{
			return FLT_MAX;
		}
	}

	

	/*!< Получить количество шагов */

	int GetStepCount() { return m_Steps; }



private: 

	/*!< Метод освобождает выделеную память когда поиск провалился */
	void FreeAllNodes()
	{
		/*!< итератор удаляющий из открытого списка все узлы */
		typename vector< Node * >::iterator iterOpen = m_OpenList.begin();

		while( iterOpen != m_OpenList.end() )
		{
			Node *n = (*iterOpen);
			FreeNode( n );

			iterOpen ++;
		}

		m_OpenList.clear();

		/*!< итератор удаляющий из закрытого списка все узлы*/
		typename vector< Node * >::iterator iterClosed;

		for( iterClosed = m_ClosedList.begin(); iterClosed != m_ClosedList.end(); iterClosed ++ )
		{
			Node *n = (*iterClosed);
			FreeNode( n );
		}

		m_ClosedList.clear();

		/*!< Удаляем конечную точку*/

		FreeNode(m_Goal);
	}


	/*!< Удаляет все неизмользуемые узлы которые остались по завершению поиска*/
	void FreeUnusedNodes()
	{
		/*!< итератор удаляющий из открытого списка неиспользуемые узлы*/
		typename vector< Node * >::iterator iterOpen = m_OpenList.begin();

		while( iterOpen != m_OpenList.end() )
		{
			Node *n = (*iterOpen);

			if( !n->following )
			{
				FreeNode( n );

				n = NULL;
			}

			iterOpen ++;
		}

		m_OpenList.clear();

		/*!< итератор удаляющий из закрытого списка неиспользуемые  узлы*/
		typename vector< Node * >::iterator iterClosed;

		for( iterClosed = m_ClosedList.begin(); iterClosed != m_ClosedList.end(); iterClosed ++ )
		{
			Node *n = (*iterClosed);

			if( !n->following )
			{
				FreeNode( n );
				n = NULL;

			}
		}

		m_ClosedList.clear();

	}

	
	Node *AllocateNode()
	{
		Node *p = new Node;
		return p;
	}

	void FreeNode( Node *node )
	{

		m_AllocateNodeCount --;
		delete node;
	}

private: 

	/*!< Открытый список */
	vector< Node *> m_OpenList;

	/*!< Закрытый список */
	vector< Node * > m_ClosedList; 

	/*!< Список узлов-приемников */
	vector< Node * > m_Successors;


	/*!< Состояние */
	unsigned int m_State;

	/*!< Счетчик шагов */
	int m_Steps;

	/*!< Указатели на начальную и конечную точку */
	Node *m_Start;
	Node *m_Goal;

	Node *m_CurrentSolutionNode;

	int m_AllocateNodeCount;

	bool m_CancelRequest;

};

template <class T> class AStar
{
public:
	virtual ~AStar() {}
	virtual float GoalDistanceEstimate( T &nodeGoal ) = 0; /*!< Эвристическая функция, которая вычисляет ориентировочную стоимость от узла до цели */
	virtual bool IsGoal( T &nodeGoal ) = 0; /*!< Возвращает true, если этот узел является целью */
	virtual bool GetSuccessors( AStarFindPath<T> *astarfindpath, T *neighbor_node ) = 0; /*!< Получает список всех соседей  этого узла и добавляет их через AStarFindPath.addSuccessor () */
	virtual float GetCost( T &successor ) = 0; /*!<Вычисляет стоимость перехода от этого узла к узлу-соседу */
	virtual bool IsSameState( T &rhs ) = 0; /*!< Возвращает true, если этот узел совпадает с узлом rhs */
};
