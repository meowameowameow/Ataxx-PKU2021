#include<iostream>
#include<math.h>
#include<cstdlib>
#include<algorithm>
#include<time.h>
using namespace std;

int map[7][7] = { 0 };
int mycolor;
int turnID;
static int delta[24][2] = { { 1,1 },{ 0,1 },{ -1,1 },{ -1,0 },
{ -1,-1 },{ 0,-1 },{ 1,-1 },{ 1,0 },
{ 2,0 },{ 2,1 },{ 2,2 },{ 1,2 },
{ 0,2 },{ -1,2 },{ -2,2 },{ -2,1 },
{ -2,0 },{ -2,-1 },{ -2,-2 },{ -1,-2 },
{ 0,-2 },{ 1,-2 },{ 2,-2 },{ 2,-1 } };     

struct cho {
    int fx, fy, tx, ty;
    int direction;
    int jiazhi = 0;
};
cho choice[6][1000];
int movecout[6] = { 0 };
int cun[6][7][7];
const int maxceng = 4; 
int tmax[7] = { -1000000,-1000000,-1000000,-1000000,-1000000,-1000000,-1000000 };
int tmin[7] = { 1000000,1000000,1000000,1000000,1000000,1000000,1000000 };
int mmax = -1000000, mx = 0, my = 0, fromx = 0, fromy = 0;
clock_t starttime;

void copy(int map[][7], int cun[][7]);
void change2(int, int, int, int, int, int map[][7]);
int pinggu(int map[][7]);
void computer(int);
void player(int);
void findmove(int, int);
bool cmp(cho x, cho y);

int main()
{
    istream::sync_with_stdio(false);
    int x0, y0, x1, y1;

    map[0][0] = map[6][6] = 1; 
    map[6][0] = map[0][6] = -1; 

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

    // 做出决策（你只需修改以下部分）

    starttime = clock();
    findmove(mycolor, 0);
    if (movecout[0] > 0) {
       
        copy(map, cun[0]);
        sort(choice[0], choice[0] + movecout[0], cmp);
        for (int i = 0; i < movecout[0] && i < 36 && (((double)clock() - starttime) / CLOCKS_PER_SEC) < 0.940; i++) {
            if (choice[0][i].direction < 8) {       
                bool repeat = false;
                for (int j = i - 1; j >= 0 && choice[0][j].direction < 8 && choice[0][j].jiazhi == choice[0][i].jiazhi; j--) {
                    if (choice[0][j].tx == choice[0][i].tx && choice[0][j].ty == choice[0][i].ty)
                        repeat = true;
                }
                if (repeat) continue;
            }
            change2(choice[0][i].fx, choice[0][i].fy, choice[0][i].tx, choice[0][i].ty, mycolor, map);
            tmin[1] = 1000000;
            tmax[1] = -1000000;
            player(1);

            copy(cun[0], map);
            if (tmax[0] > mmax) {
                fromx = choice[0][i].fx; fromy = choice[0][i].fy; mx = choice[0][i].tx; my = choice[0][i].ty; mmax = tmax[0];
            }
        }
        // 决策结束，输出结果（你只需修改以上部分）
        cout << fromx << " " << fromy << " " << mx << " " << my;
    }
    else cout << -1 << " " << -1 << " " << -1 << " " << -1;  
    return 0;
}

void copy(int map[][7], int tmpmap[][7]) {
    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 7; j++)
            tmpmap[i][j] = map[i][j];
    return;
}

void change2(int x1, int y1, int x2, int y2, int color, int map[][7]) {
    for (int dir = 0; dir < 8; dir++) {
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

int pinggu(int map[][7]) {
    int changfang[7][7] = { 0 };
    int sum1 = 0, sum2 = 0;
    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 7; j++) {
            if (map[i][j] == mycolor) {
                sum1++;
                if (j > 0) {
                    changfang[i][j] = changfang[i][j - 1] + 1;
                }
                else {
                    changfang[i][j] = 1;
                }
                continue;
            }
            if (map[i][j] == -mycolor) {
                sum2++; continue;
            }
        }
    int max = 0;
    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 7; j++) {
            if (changfang[i][j] == 0) continue;
            int h = 1, length = changfang[i][j];
            int tmax = length;
            while (i - h >= 0 && changfang[i - h][j] > 0) {
                length = length < changfang[i - h][j] ? length : changfang[i - h][j];
                int s = length * (h + 1);
                tmax = tmax > s ? tmax : s;
                h++;
            }
            max = max > tmax ? max : tmax;
        }
    return (sum1 - sum2) * 200 + max;
}

void computer(int ceng) {
    if (ceng > maxceng) {
        int tmp = pinggu(map);
        tmin[ceng - 1] = tmin[ceng - 1] < tmp ? tmin[ceng - 1] : tmp;
        return;
    }
    findmove(mycolor, ceng);
    if (movecout[ceng] == 0) {
        tmax[ceng] = pinggu(map);
    }
    else {
        copy(map, cun[ceng]);

        sort(choice[ceng], choice[ceng] + movecout[ceng], cmp);
        for (int i = 0; i < movecout[ceng] && i < 36; i++) {
            if (choice[ceng][i].direction < 8) {
                bool repeat = false;
                for (int j = i - 1; j >= 0 && choice[ceng][j].direction < 8 && choice[ceng][j].jiazhi == choice[ceng][i].jiazhi; j--) {
                    if (choice[ceng][j].tx == choice[ceng][i].tx && choice[ceng][j].ty == choice[ceng][i].ty)
                        repeat = true;
                }
                if (repeat) continue;
            }

            change2(choice[ceng][i].fx, choice[ceng][i].fy, choice[ceng][i].tx, choice[ceng][i].ty, mycolor, map);

            tmin[ceng + 1] = 1000000;
            tmax[ceng + 1] = -1000000;
            player(ceng + 1);
            copy(cun[ceng], map);
            if (tmax[ceng] >= tmin[ceng - 1]) return;
        }
    }
    tmin[ceng - 1] = tmin[ceng - 1] < tmax[ceng] ? tmin[ceng - 1] : tmax[ceng];
}

void player(int ceng) {
    if (ceng > maxceng) {
        int tmp = pinggu(map);
        tmax[ceng - 1] = tmax[ceng - 1] > tmp ? tmax[ceng - 1] : tmp;
        return;
    }
    findmove(-mycolor, ceng);
    if (movecout[ceng] == 0) {
        tmin[ceng] = pinggu(map);
    }
    else {
        copy(map, cun[ceng]);

        sort(choice[ceng], choice[ceng] + movecout[ceng], cmp);
        for (int i = 0; i < movecout[ceng] && i < 37; i++) {
            if (choice[ceng][i].direction < 8) {
                bool repeat = false;
                for (int j = i - 1; j >= 0 && choice[ceng][j].direction < 8 && choice[ceng][j].jiazhi == choice[ceng][i].jiazhi; j--) {
                    if (choice[ceng][j].tx == choice[ceng][i].tx && choice[ceng][j].ty == choice[ceng][i].ty)
                        repeat = true;
                }
                if (repeat) continue;
            }
            change2(choice[ceng][i].fx, choice[ceng][i].fy, choice[ceng][i].tx, choice[ceng][i].ty, -mycolor, map);
            tmax[ceng + 1] = -1000000;
            tmin[ceng + 1] = 1000000;
            computer(ceng + 1);
            copy(cun[ceng], map);
            if (tmin[ceng] <= tmax[ceng - 1]) return;
        }
    }
    tmax[ceng - 1] = tmax[ceng - 1] > tmin[ceng] ? tmax[ceng - 1] : tmin[ceng];
}

void findmove(int color, int ceng) {
    movecout[ceng] = 0;
    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 7; j++) {
            if (map[i][j] != color) continue;

            for (int dir = 0; dir < 24; dir++) {
                int x = i + delta[dir][0];
                int y = j + delta[dir][1];
                if (x < 0 || x>6 || y < 0 || y>6 || map[x][y] != 0) continue;
                choice[ceng][movecout[ceng]].fx = i; choice[ceng][movecout[ceng]].fy = j;
                choice[ceng][movecout[ceng]].tx = x; choice[ceng][movecout[ceng]].ty = y;
                choice[ceng][movecout[ceng]].direction = dir;

                for (int d = 0; d < 8; d++)
                {
                    int xx = x + delta[d][0];
                    int yy = y + delta[d][1];
                    if (map[xx][yy] != -color || x < 0 || x>6 || y < 0 || y>6)
                        continue;
                    choice[ceng][movecout[ceng]].jiazhi++;
                }
                if (dir < 8) choice[ceng][movecout[ceng]].jiazhi++;
                movecout[ceng]++;
            }
        }
}
bool cmp(cho x, cho y) {
    if (x.jiazhi != y.jiazhi)
        return x.jiazhi > y.jiazhi;
    return x.direction < y.direction;
}