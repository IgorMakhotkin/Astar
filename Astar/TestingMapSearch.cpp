#include "StdAfx.h"
//#include <cfixcc.h>
//#include "MapSearch.h"
//class TestingMapSearch : public cfixcc::TestFixture
//{
//private:
//
//public:
//	void TestIsGoal()
//	
//	{
//		MapSearchNode nodeTestStart;
//		MapSearchNode nodeTestEnd;
//		nodeTestStart.x=5;
//		nodeTestStart.y=6;
//
//		nodeTestEnd.x=5;
//		nodeTestEnd.y=6;
//		CFIX_LOG(L"Test 1. Parametrs: 5,6;5,6; expected: true");
//		CFIX_ASSERT(nodeTestStart.IsGoal(nodeTestEnd));
//
//		nodeTestStart.x=6;
//		nodeTestStart.y=6;
//		nodeTestEnd.x=5;
//		nodeTestEnd.y=6;
//		CFIX_LOG(L"Test 2. Parametrs: 5,6;6,6; expected: false");
//		CFIX_ASSERT(!nodeTestStart.IsGoal(nodeTestEnd));
//	}
//
//	void TestGoalDistanceEstimate()
//	{
//		MapSearchNode nodeTestStart;
//		MapSearchNode nodeTestEnd;
//		nodeTestStart.x=3;
//		nodeTestStart.y=0;
//
//		nodeTestEnd.x=5;
//		nodeTestEnd.y=6;
//		float exp=8.0;
//		CFIX_LOG(L"Test 3. Parametrs: 3,0;5,6; expected: 8.0");
//		CFIXCC_ASSERT_EQUALS(exp,nodeTestStart.GoalDistanceEstimate(nodeTestEnd));
//	}
//	void TestGetCost()
//	{
//		MapSearchNode nodeTest1;
//		MapSearchNode nodeTest2;
//		nodeTest2.x=2;
//		nodeTest2.y=1;
//		int world_map[3][3]={
//		{1,1,1},
//		{1,9,1},
//		{1,9,1}};
//		float exp=1.4;
//		CFIX_LOG(L"Test 4. expected: 1.4");
//		CFIXCC_ASSERT_EQUALS(exp,nodeTest1.GetCost(nodeTest2));
//
//	}
//	void TestIsSameState()
//	{
//		MapSearchNode nodeTest1;
//		MapSearchNode nodeTest2;
//		nodeTest1.x=2;
//		nodeTest1.y=1;
//		nodeTest2.x=2;
//		nodeTest2.y=1;
//		CFIX_LOG(L"Test 5. expected: true");
//		CFIXCC_ASSERT(nodeTest1.IsSameState(nodeTest2));
//	}
//
//
//};
//
//CFIXCC_BEGIN_CLASS(TestingMapSearch)
//	CFIXCC_METHOD(TestIsGoal)
//	CFIXCC_METHOD(TestGoalDistanceEstimate)
//	CFIXCC_METHOD(TestGetCost)
//	CFIXCC_METHOD(TestIsSameState)
//	
//CFIXCC_END_CLASS()
//
