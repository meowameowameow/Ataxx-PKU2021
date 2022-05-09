# Ataxx-PKU2021
minimax算法搜索5层，使用alpha-beta剪枝与预排序优化

关于各版本：   
botzone初版 搜索4层，使用alpha-beta剪枝;    
botzone改进版 搜索5层，加入预排序，即首先根据一步贪心结果排序，取前四十种走法，并剪掉导致结果相同的走法;    
大作业功能版：控制台应用，用制表符，输出棋盘；实现存盘，读盘，新游戏，悔棋功能；   

关于算法思路：   
建议阅读http://home.ustc.edu.cn/~baj/publications/concluding2007-Bai.pdf

优化建议：    
1.优化算法，参见上面的文章    
2.更好的评估函数 个人仅采用 (我方棋子数-对方棋子数)×大权重 + 最大矩形面积×小权重 的估值函数，建议进一步优化    
3.采用一维数组而非二维数组储存盘面，进一步，使用哈希表与位运算，增大搜索层数    
4.终盘阶段优化：只剩不到5个空位（小于搜索层数，会走出奇怪/有害的走法，建议改进收尾时的算法    
5.细节：在保存与回溯时，程序将7×7棋盘全部复制，实际上只有一小部分（个位数 的棋子发生变动，建议改进    

关于botzone天梯与班赛：    
采用minimax算法，优化加深搜索层数，即可取得较好的名次，大多数人也是这样做的XD    
入学以来的第一次大作业，不必焦虑，加油:-)    
