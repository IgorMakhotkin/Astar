Astar.exe map1.txt data1.txt >> result/1.txt
Astar.exe map2.txt data2.txt >> result/2.txt
Astar.exe map3.txt data3.txt >> result/3.txt
Astar.exe map4.txt data4.txt >> result/4.txt
FOR /L %%a IN (1,1,4) DO (
     
	sprog.exe result/%%a.txt expected/%%a.txt
)
pause