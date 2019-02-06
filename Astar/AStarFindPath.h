
#include <iostream>
#include <stdio.h>
#include <assert.h>
#include <algorithm>
#include <set>
#include <vector>
#include <cfloat>

using namespace std;



template <class T> class AStar;

/*!< ����� ������ */
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
			
			float g; /*!< ��������� ����������� �� ���������������� ���� � �������� */
			float h; /*!< ������������� ������ ���������� �� ���� */
			float f; /*!< ��������� ��������� ���� �� ������ */

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
	\brief ������� ���������, ������������ ��� ���� 
	*/

	class HeapCompare_f 
	{
		public:

			bool operator() ( const Node *x, const Node *y ) const
			{
				return x->f > y->f;
			}
	};


public: /*! ������ */

	
	/*!
	 \brief ������������� ��������� � �������� ���������� �����
	 \param[in] ��������� �� ��������� �����
     \param[in] ��������� �� ��������� �����
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

		/*!< �������� ��������� �������� � �������� ������ */

		m_OpenList.push_back( m_Start ); 

		/*!< ��������� ������ �� �������� f */
		push_heap( m_OpenList.begin(), m_OpenList.end(), HeapCompare_f() );

		/*!< ������������� ��� ����� ������ */
		m_Steps = 0;
	}

	/*!< ����� ���� */
	unsigned int SearchStep()
	{
	
		/*!< ���� ������ �������� ������ ������,��� ������� ���� ��� ����������� ���� ��� ��������, �� ��������� ����� */
		if( m_OpenList.empty() || m_CancelRequest )
		{
			FreeAllNodes();
			m_State = SEARCH_FAILED;
			return m_State;
		}
		
		/*!< ����������� ������� ����� */
		m_Steps ++;

		/*!< �������� ���� � ���������� f */
		Node *n = m_OpenList.front(); // �������� ��������� �� ����
		pop_heap( m_OpenList.begin(), m_OpenList.end(), HeapCompare_f() );
		m_OpenList.pop_back();

		/*!< ���������� �� �������� �� ���� ��������� */
		if( n->m_UserState.IsGoal( m_Goal->m_UserState ) )
		{
			/*!< ���������� ���� ����, ����� ����������� �������� ��������� */
			m_Goal->neighbor = n->neighbor;
			m_Goal->g = n->g;

			
			if( false == n->m_UserState.IsSameState( m_Start->m_UserState ) )
			{
				FreeNode( n );

				/*!< ������������� ������� � ���������� ����*/
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

			/*!< ������� ���� ������� �� �������� ��� ����*/
			FreeUnusedNodes();

			m_State = SEARCH_SUCCEEDED;

			return m_State;
		}
		else  
		{

		

			m_Successors.clear(); /*!< �������� ������ ��� ����� ���������� */

			/*!< ������� ��� ���������� ����-���������  AddSuccessor*/
			bool ret = n->m_UserState.GetSuccessors( this, n->neighbor ? &n->neighbor->m_UserState : NULL ); 

			if( !ret )
			{

			    typename vector< Node * >::iterator successor;

				/*!< ������� ���� ���������� �����*/
				for( successor = m_Successors.begin(); successor != m_Successors.end(); successor ++ )
				{
					FreeNode( (*successor) );
				}

				m_Successors.clear(); /*!< �������� ������ ��� ����� ���������� */

				/*!< ����������� ������*/

				m_State = SEARCH_OUT_OF_MEMORY;
				return m_State;
			}
			
			/*!< ��������� ������� ��������� �������� ����*/
			for( typename vector< Node * >::iterator successor = m_Successors.begin(); successor != m_Successors.end(); successor ++ )
			{

				/*!< 	�������� �������� g ��� ���� ���������*/
				float newg = n->g + n->m_UserState.GetCost( (*successor)->m_UserState );

				/*!< ��������� ��������� �� ���� � �������� ��� �������� ������
				 �������� ����� �� ��������� ������*/

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

					/*!< ���������� ������� ���� � �������� ������*/

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

					/*!< ���������� ������� ���� � �������� ������*/

					if( (*closedlist_result)->g <= newg )
					{
						
						FreeNode( (*successor) );

						continue;
					}
				}

				/*!< ���� ���� �������� ������ ����� �� ��� ��� � ���� ���������� ����������*/

				(*successor)->neighbor = n;
				(*successor)->g = newg;
				(*successor)->h = (*successor)->m_UserState.GoalDistanceEstimate( m_Goal->m_UserState );
				(*successor)->f = (*successor)->g + (*successor)->h;

				/*!< ������� ����-�������� �� ��������� ������, ���� �� ��� � ���*/

				if( closedlist_result != m_ClosedList.end() )
				{
					/*!< ��������*/
					FreeNode(  (*closedlist_result) ); 
					m_ClosedList.erase( closedlist_result );

				
					
				}

				/*!< ��������� ������*/
				if( openlist_result != m_OpenList.end() )
				{	   

					FreeNode( (*openlist_result) ); 
			   		m_OpenList.erase( openlist_result );
					make_heap( m_OpenList.begin(), m_OpenList.end(), HeapCompare_f() );
			
				}

			
				m_OpenList.push_back( (*successor) );

				/*!< ����� ��������� ������*/
				push_heap( m_OpenList.begin(), m_OpenList.end(), HeapCompare_f() );

			}

			

			m_ClosedList.push_back( n );

		} 

 		return m_State; 

	}

	/*!< �������� ����� ��� �������� ���� ��������� � ������ ����������*/

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

	/*!< ������� ��� ������� ����� ���������� ����������*/

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

			FreeNode( n ); /*!< ������� �������� �����*/

		}
		else
		{
			/*!< ���� ��������� ����� ��������� � ��������, �� ������� ��������� � �������� ���� */
			FreeNode( m_Start );
			FreeNode( m_Goal );
		}

	}

	/*!< ������� ��� ���������� ������� */

	/*!< �������� ��������� ���� */
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
	
	/*!< �������� ��������� ���� */
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
	
	/*!< �������� �������� ���� */
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
	
	/*!< �������� ���� ������� */
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

	/*!<�������� ������������� ��������� �������
 ���������� FLT_MAX, ���� ���� �� ���������� ��� ��� ������� */

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

	

	/*!< �������� ���������� ����� */

	int GetStepCount() { return m_Steps; }



private: 

	/*!< ����� ����������� ��������� ������ ����� ����� ���������� */
	void FreeAllNodes()
	{
		/*!< �������� ��������� �� ��������� ������ ��� ���� */
		typename vector< Node * >::iterator iterOpen = m_OpenList.begin();

		while( iterOpen != m_OpenList.end() )
		{
			Node *n = (*iterOpen);
			FreeNode( n );

			iterOpen ++;
		}

		m_OpenList.clear();

		/*!< �������� ��������� �� ��������� ������ ��� ����*/
		typename vector< Node * >::iterator iterClosed;

		for( iterClosed = m_ClosedList.begin(); iterClosed != m_ClosedList.end(); iterClosed ++ )
		{
			Node *n = (*iterClosed);
			FreeNode( n );
		}

		m_ClosedList.clear();

		/*!< ������� �������� �����*/

		FreeNode(m_Goal);
	}


	/*!< ������� ��� �������������� ���� ������� �������� �� ���������� ������*/
	void FreeUnusedNodes()
	{
		/*!< �������� ��������� �� ��������� ������ �������������� ����*/
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

		/*!< �������� ��������� �� ��������� ������ ��������������  ����*/
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

	/*!< �������� ������ */
	vector< Node *> m_OpenList;

	/*!< �������� ������ */
	vector< Node * > m_ClosedList; 

	/*!< ������ �����-���������� */
	vector< Node * > m_Successors;


	/*!< ��������� */
	unsigned int m_State;

	/*!< ������� ����� */
	int m_Steps;

	/*!< ��������� �� ��������� � �������� ����� */
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
	virtual float GoalDistanceEstimate( T &nodeGoal ) = 0; /*!< ������������� �������, ������� ��������� ��������������� ��������� �� ���� �� ���� */
	virtual bool IsGoal( T &nodeGoal ) = 0; /*!< ���������� true, ���� ���� ���� �������� ����� */
	virtual bool GetSuccessors( AStarFindPath<T> *astarfindpath, T *neighbor_node ) = 0; /*!< �������� ������ ���� �������  ����� ���� � ��������� �� ����� AStarFindPath.addSuccessor () */
	virtual float GetCost( T &successor ) = 0; /*!<��������� ��������� �������� �� ����� ���� � ����-������ */
	virtual bool IsSameState( T &rhs ) = 0; /*!< ���������� true, ���� ���� ���� ��������� � ����� rhs */
};
