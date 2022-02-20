#include<iostream>
#include<math.h>
#include<string>
#include<cstdlib>
using namespace std;

int map[7][7] = { 0 };
int mycolor;
int turnID;
static int delta[24][2] = { { 1,1 },{ 0,1 },{ -1,1 },{ -1,0 },
{ -1,-1 },{ 0,-1 },{ 1,-1 },{ 1,0 },
{ 2,0 },{ 2,1 },{ 2,2 },{ 1,2 },
{ 0,2 },{ -1,2 },{ -2,2 },{ -2,1 },
{ -2,0 },{ -2,-1 },{ -2,-2 },{ -1,-2 },
{ 0,-2 },{ 1,-2 },{ 2,-2 },{ 2,-1 } };      //共24个移动方向，前8个为相邻，后16个为跳跃

int cun[5][7][7];//保存棋盘状态，用于回溯
const int maxceng = 3;      //规定向下搜索的最大深度（可改变
int tmax[6] = { -1000000,0,-1000000,0,-1000000,-0 };//记录max结点的最大值,可见只有偶数项0,2,4有用
int tmin[6] = { 0,1000000,0,1000000,0,1000000 };//记录min结点的最小值,只有奇数项1,3,5有用

bool continueornot(int, int map[][7]);//判断一方是否有子可下，用于判断棋局结束
void copy(int map[][7], int cun[][7]);//复制棋盘
void change2(int, int, int, int, int, int map[][7]);//任一方落子后，改变颜色，但不计数 
int pinggu(int map[][7]);//计算并返回盘面上的白棋（电脑）减黑棋（玩家）数
void computermax(int);//模拟电脑下棋，为max节点
void playermin(int);//模拟玩家下棋，为min节点

int main()
{
    istream::sync_with_stdio(false);
    int x0, y0, x1, y1;
    // 初始化棋盘
    map[0][0] = map[6][6] = 1;  //|黑|白| ！！！！！！ 1为黑，-1为白
    map[6][0] = map[0][6] = -1; //|白|黑|

    // 分析自己收到的输入和自己过往的输出，并恢复状态
    
    mycolor = -1; // 默认我为白方，直到收到-1-1-1-1
    cin >> turnID;
    for (int i = 0; i < turnID - 1; i++)
    {
        // 根据这些输入输出逐渐恢复状态到当前回合
        cin >> x0 >> y0 >> x1 >> y1;
        if (x1 >= 0)
            change2(x0, y0, x1, y1, -mycolor, map); // 模拟对方落子
        else
            mycolor = 1;
        cin >> x0 >> y0 >> x1 >> y1;
        if (x1 >= 0)
            change2(x0, y0, x1, y1, mycolor, map); // 模拟己方落子
    }

    // 看看自己本回合输入
    cin >> x0 >> y0 >> x1 >> y1;
    if (x1 >= 0)
        change2(x0, y0, x1, y1, -mycolor, map); // 模拟对方落子
    else
        mycolor = 1;

    if (!continueornot(mycolor, map))
        cout << -1 << " " << -1 << " " << -1 << " " << -1;
    else {
        // 做出决策（你只需修改以下部分）
        int mmax = -1000000, mx = 0, my = 0, fromx = 0, fromy = 0;
        copy(map, cun[0]);
        tmax[0] = -1000000; tmin[0] = 1000000;
        for (int i = 0; i < 7; i++) {
            for (int j = 0; j < 7; j++) {
                if (map[i][j] !=mycolor) continue;

                for (int t = 0; t < 24; t++) {
                    int x = i + delta[t][0];
                    int y = j + delta[t][1];
                    if (x < 0 || x>6 || y < 0 || y>6 || map[x][y] != 0) continue;
                    change2(i, j, x, y, mycolor, map);            
                    tmin[1] = 1000000;
                    playermin(1);

                    copy(cun[0], map);
                    if (tmax[0] > mmax) {
                        fromx = i; fromy = j; mx = x; my = y; mmax = tmax[0];
                    }
                }
            }
        }
        // 决策结束，输出结果（你只需修改以上部分）
        cout << fromx<< " " << fromy << " " << mx << " " << my;    
    }
    return 0;
}

bool continueornot(int color, int map[][7]) {//用于判断是否结束
    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 7; j++)
            if (map[i][j] == 0) {
                for (int k = 0; k < 24; k++) {
                    int x = i + delta[k][0];
                    int y = j + delta[k][1];
                    if (x < 0 || x>6 || y < 0 || y>6) continue;
                    if (map[x][y] == color)
                        return true;
                }
            }
    return false;
}

void copy(int map[][7], int tmpmap[][7]) {//复制棋盘
    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 7; j++)
            tmpmap[i][j] = map[i][j];
    return;
}

void change2(int x1, int y1, int x2, int y2, int color, int map[][7]) {//任一方落子后 改变颜色,但不计数  
    for (int dir = 0; dir < 8; dir++){
        int x = x2 + delta[dir][0];
        int y = y2 + delta[dir][1];
        if (map[x][y] != -color || x < 0 || x>6 || y < 0 || y>6)
            continue;
         map[x][y] = color;
    }
    map[x2][y2] = color;
    if (abs(x2 - x1) > 1 || abs(y2 - y1) > 1) {
        map[x1][y1] = 0;
    }    
}

int pinggu(int map[][7]) {//估值函数
    int sum1 = 0, sum2 = 0;
    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 7; j++) {
            if (map[i][j] == mycolor)  sum1++;
            if (map[i][j] == -mycolor) sum2++; 
        }
   return sum1 - sum2;
}

void  computermax(int ceng) {//max节点
    if (ceng < maxceng) {
        copy(map, cun[ceng]);
        for (int i = 0; i < 7; i++) {
            for (int j = 0; j < 7; j++) {
                if (map[i][j] != mycolor) continue;
                for (int t = 0; t < 24; t++) {
                    int x = i + delta[t][0];
                    int y = j + delta[t][1];
                    if (x < 0 || x>6 || y < 0 || y>6 || map[x][y] != 0) continue;
                    change2(i, j, x, y, mycolor, map);
                    tmin[ceng + 1] = 1000000;
                    playermin(ceng + 1);
                    copy(cun[ceng], map);
                    if (tmax[ceng] >= tmin[ceng - 1]) return;//如果当前这一步的最大结果tmax[ceng]，大于已搜索过的局部最小结果tmin[ceng-1]                   
                }                                                           //就可以剪枝了，因为对手在上一步必不可能采取这种下法而让我（电脑）得到更好的结果
            }
        }
        if (tmax[ceng] == -1000000) tmax[ceng] = pinggu(map);
        tmin[ceng - 1] = tmin[ceng - 1] < tmax[ceng] ? tmin[ceng - 1] : tmax[ceng];
    }
    else {
        copy(map, cun[ceng]);
        for (int i = 0; i < 7; i++) {
            for (int j = 0; j < 7; j++) {
                if (map[i][j] != mycolor) continue;
                for (int t = 0; t < 24; t++) {
                    int x = i + delta[t][0];
                    int y = j + delta[t][1];
                    if (x < 0 || x>6 || y < 0 || y>6 || map[x][y] != 0) continue;
                    change2(i, j, x, y, mycolor, map);
                    int tmp = pinggu(map);
                    tmax[ceng] = tmax[ceng] > tmp ? tmax[ceng] : tmp;
                    copy(cun[ceng], map);
                    if (tmax[ceng] >= tmin[ceng - 1]) return;                    
                }
            }
        }
        if (tmax[ceng] == -1000000) tmax[ceng] = pinggu(map);
        tmin[ceng - 1] = tmin[ceng - 1] < tmax[ceng] ? tmin[ceng - 1] : tmax[ceng];
        return;
    }
}

void playermin(int ceng) {//模拟玩家下棋，为min节点
    if (ceng < maxceng) {
        copy(map, cun[ceng]);
        for (int i = 0; i < 7; i++) {
            for (int j = 0; j < 7; j++) {
                if (map[i][j] != -mycolor) continue;
                for (int t = 0; t < 24; t++) {
                    int x = i + delta[t][0];
                    int y = j + delta[t][1];
                    if (x < 0 || x>6 || y < 0 || y>6 || map[x][y] != 0) continue;
                    change2(i, j, x, y, -mycolor, map);
                    tmax[ceng + 1] = -1000000;
                    computermax(ceng + 1);
                    copy(cun[ceng], map);
                    if (tmin[ceng] <= tmax[ceng - 1]) return;//如果当前这一步的最小结果tmin[ceng]，小于已搜索过的局部最大结果tmax[ceng-1]                  
                }                                                //就可以剪枝了，因为我（电脑）在上一步必不可能采取这种下法而让我（电脑）得到更差的结果
            }
        }
        if (tmin[ceng] == 1000000) tmin[ceng] = pinggu(map);
        tmax[ceng - 1] = tmax[ceng - 1] > tmin[ceng] ? tmax[ceng - 1] : tmin[ceng];
    }
    else {
        copy(map, cun[ceng]);
        for (int i = 0; i < 7; i++) {
            for (int j = 0; j < 7; j++) {
                if (map[i][j] != -mycolor) continue;
                for (int t = 0; t < 24; t++) {
                    int x = i + delta[t][0];
                    int y = j + delta[t][1];
                    if (x < 0 || x>6 || y < 0 || y>6 || map[x][y] != 0) continue;
                    change2(i, j, x, y, -mycolor, map);
                    int tmp = pinggu(map);
                    tmin[ceng] = tmin[ceng] < tmp ? tmin[ceng] : tmp;
                    copy(cun[ceng], map);
                    if (tmin[ceng] <= tmax[ceng - 1]) return;
                    
                }
            }
        }
        if (tmin[ceng] == 1000000) tmin[ceng] = pinggu(map);
        tmax[ceng - 1] = tmax[ceng - 1] > tmin[ceng] ? tmax[ceng - 1] : tmin[ceng];
        return;
    }
}


