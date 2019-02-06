#include "AStarFindPath.h"
#include "stdafx.h"
#include <iostream>
#include <stdio.h>
#include <math.h>
#include "MapSearch.h"
#include <ctime>


using namespace std;




int main( int argc, char *argv[] )
{
	srand(time(0));
	setlocale (LC_ALL, "Russian");


	if (argc < 3 )
	{
		printf("���������� ������� �����\n");
		return 0;
	}
	char fileName1[100];/*!< ���������� ��� ����� ������� ����� */
	char fileName2[100];/*!< ���������� ��� ����� ������� ����� */
	strcpy(fileName1,"C:\\Astar\\Debug\\");
	strcat(fileName1,argv[1]);
	strcpy(fileName2,"C:\\Astar\\Debug\\");
	strcat(fileName2,argv[2]);


	/*!< ��������� ������ ���� */

		FILE *inmap= fopen(fileName1, "r");
	if(inmap==NULL)
	{printf("���� �� ������ \n"); 
	return 0;}
	else
	{
		fscanf(inmap,"%d",&MAP_WIDTH);
		if(MAP_WIDTH==NULL)
		{printf("������ ������� ����������� \n"); 
		return 0;}

		fscanf(inmap,"%d",&MAP_HEIGHT);
		if(MAP_HEIGHT==NULL)
		{printf("������ ������� ����������� \n"); 
		return 0;}

		for(int i=0;i<MAP_WIDTH;i++)
		{
			for (int j=0;j<MAP_HEIGHT;j++)
			{
				fscanf(inmap,"%f", &world_map[i][j]);
			}
		}
		fclose(inmap);
	}



	/*!< ������� ��������� ������ ������ */

		AStarFindPath<MapSearchNode> astarfindpath;

	unsigned int SearchCount = 0;

	const unsigned int NumSearches = 1;

	while(SearchCount < NumSearches)
	{
		/*!< ������� ��������� ����� */
		MapSearchNode nodeStart;
		/*!< ������� �������� ����� */
		MapSearchNode nodeEnd;

		/*!< ��������� ������ ���� */
		FILE *indata= fopen(fileName2, "r");
		if(indata==NULL)
		{printf("���� �� ������ \n"); 
		return 0;}
		else
		{
			fscanf(indata,"%d",&nodeStart.x);
			fscanf(indata,"%d",&nodeStart.y);
			fscanf(indata,"%d",&nodeEnd.x);
			fscanf(indata,"%d",&nodeEnd.y);

			fclose(indata);
		}
		/*!< ��������� ��������� �������� */
		if(nodeStart.x<0||nodeStart.y<0)
		{printf("���������� ��������� ����� ������� �����������\n");
		return 0;}
		/*!< ��������� ��������� �������� */
		if(nodeEnd.x<0||nodeEnd.y<0)
		{printf("���������� �������� ����� ������� �����������\n");
		return 0;}

		/*!< ��������� ��������� �������� �� �������������� � ����� ������  */
		if(nodeStart.x>MAP_WIDTH || nodeStart.y>MAP_HEIGHT) 
		{printf("���������� ��������� ����� �� ����������� ������� ������ \n"); 
		return 0;
		} 
		if(nodeEnd.x>MAP_WIDTH || nodeEnd.y>MAP_HEIGHT) 
		{printf("���������� �������� ����� �� ����������� ������� ������ \n");
		return 0;
		}
		/*!< ��������� ��������� �������� �� �������������� � ��������� ����������� */
		if(world_map[nodeStart.x][nodeStart.y]==9)
		{printf("���������� ��������� ����� ����������� ��������� ����������� \n");
		return 0;
		}
		if(world_map[nodeEnd.x][nodeEnd.y]==9)
		{printf("���������� �������� ����� ����������� ��������� ����������� \n");
		return 0;
		}




		astarfindpath.SetStartAndGoalStates( nodeStart, nodeEnd );

		unsigned int SearchState;
		unsigned int SearchSteps = 0;

		do
		{
			SearchState = astarfindpath.SearchStep();

			SearchSteps++;


		}
		while( SearchState == AStarFindPath<MapSearchNode>::SEARCH_SEARCHING );

		if( SearchState == AStarFindPath<MapSearchNode>::SEARCH_SUCCEEDED )
		{
			cout << "����� ����\n";

			MapSearchNode *node = astarfindpath.GetSolutionStart();


			int steps = 0;

			node->PrintNodeInfo();

			for( ;; )
			{
				node = astarfindpath.GetSolutionNext();

				if( !node )
				{
					break;
				}

				node->PrintNodeInfo();
				steps ++;

			};
			cout << "���� ������� " << steps << endl;

			/*!< ������� ��� ���� ����� ���������� ������� */
			astarfindpath.FreeSolutionNodes();
		}
		else if( SearchState == AStarFindPath<MapSearchNode>::SEARCH_FAILED ) 
		{
			cout << "����� ��������, ���� �� ������\n";

		}

		/*!< ���������� ���������� ������, ����� ������� ������ ����� */
		cout << "���� ������ : " << SearchSteps << "\n";

		SearchCount ++;


		/*!< ������������� ������ */

		for (int h=0;h<MAP_WIDTH;h++)
		{

			for (int k=0;k<MAP_HEIGHT;k++)
			{
				if(world_map[h][k]!=9.0&&input_map[h][k]!='*')
				{
					input_map[h][k]=' ';
				}
				if (world_map[h][k]==9.0)
				{
					input_map[h][k]='#';

				}
			}

		}

		input_map[nodeStart.x][nodeStart.y]='A';
		input_map[nodeEnd.x][nodeEnd.y]='�';
	}
	for (int y=0;y<MAP_WIDTH;y++)
	{
		cout << "\n";
		for (int u=0; u<MAP_HEIGHT;u++)
		{cout << input_map[y][u];}
	}

	cout << "\n";
	cout << "����� ������: " << clock()/1000.0 <<endl;
	return 0;

}