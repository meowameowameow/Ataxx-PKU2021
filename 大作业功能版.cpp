/*
同化棋 田宇轩 2100013126
*/

#include<iostream>
#include<cstring>
#include<string>
#include<string.h>
#include<math.h>
#include<ctime>
#include<fstream>
#include<cstdlib>
#include<Windows.h>
#include<algorithm>
using namespace std;

int map[7][7] = { 0 }, now[7][7], last[7][7];
int cnt1 = 2, cnt2 = 2;//记录黑白棋子数量
int lastcnt1 = 2, lastcnt2 = 2, nowcnt1 = 2, nowcnt2 = 2;
bool endd = false;//用于结束/退出功能
static int delta[24][2] = { { 1,1 },{ 0,1 },{ -1,1 },{ -1,0 },
{ -1,-1 },{ 0,-1 },{ 1,-1 },{ 1,0 },
{ 2,0 },{ 2,1 },{ 2,2 },{ 1,2 },
{ 0,2 },{ -1,2 },{ -2,2 },{ -2,1 },
{ -2,0 },{ -2,-1 },{ -2,-2 },{ -1,-2 },
{ 0,-2 },{ 1,-2 },{ 2,-2 },{ 2,-1 } };      //共24个移动方向，前8个为相邻，后16个为跳跃
int cun[7][7][7];//保存棋盘状态，用于回溯
const int maxceng = 4;      //规定向下搜索的最大深度（可改变
int tmax[7] = { -1000000,-1000000,-1000000,-1000000,-1000000,-1000000,-1000000 };//记录max结点的最大值,可见只有偶数项0,2,4有用
int tmin[7] = { 1000000,1000000,1000000,1000000,1000000,1000000,1000000 };//记录min结点的最小值,只有奇数项1,3,5有用
int mycolor = 1;
int movecout[7] = { 0 };//记录每一层选择的个数
struct cho {
    int fx, fy, tx, ty;
    int direction;
    int jiazhi;
};
cho choice[6][1000];//记录每一层的所有选择;/'l
void renew();//初始化
void game();//游戏主程序
void menu();//菜单功能
void print(int map[][7]);//输出棋盘
bool continueornot(int, int map[][7]);//判断一方是否有子可下，用于判断棋局结束
void change(int, int, int, int, int, int map[][7]);//任一方落子后 ，改变颜色，并计数
void copy(int map[][7], int cun[][7]);//复制棋盘
void change2(int, int, int, int, int, int map[][7]);//任一方落子后，改变颜色，但不计数   (用于程序3）
int pinggu(int map[][7]);//计算并返回盘面上的白棋（电脑）减黑棋（玩家）数
void computer(int);//模拟电脑下棋，为max节点
void player(int);//模拟玩家下棋，为min节点
void findmove(int, int);//找出所有能走的选择
bool cmp(cho x, cho y);//一层贪心的排序比较

int main() {//主函数
    renew();
    menu();     //直接进入菜单
    cout << "对局结束，欢迎下次来玩" << endl;
    return 0;
}

void menu() {//菜单功能
    int order;
    cout << " ┏━━━━━━━━━━━━━━━┓ " << endl;
    cout << " ┃ 欢迎来玩同化棋┃" << endl;
    cout << " ┃    1.新开局   ┃" << endl;
    cout << " ┃     2.读盘    ┃" << endl;
    cout << " ┃     3.存盘    ┃" << endl;
    cout << " ┃     4.退出    ┃" << endl;
    cout << " ┗━━━━━━━━━━━━━━━┛ " << endl;
    cout << " 继续游戏请输入其他数字" << endl;
    cin >> order; cin.ignore();

    if (order == 4) {
        endd = true;
        return;
    }
    if (order == 3) {  //  考虑存盘之后还继续吗？存盘如何编写
        ofstream fout;
        fout.open("remember.txt");
        if (!fout) {
            cout << "存盘失败，请稍后再试" << endl;
            return;
        }
        else {
            fout << cnt1 << " " << cnt2 << endl;
            for (int i = 0; i < 7; i++) {
                for (int j = 0; j < 6; j++) {
                    fout << map[i][j] << " ";
                }
                fout << map[i][6] << endl;
            }
            fout.close();
            system("cls");
            print(map);
            cout << "存盘成功，请继续输入" << endl;
            game();
            return;
        }
    }
    else {
        if (order == 1) {
            renew();  cnt1 = 2;  cnt2 = 2;
        }
        if (order == 2) {
            renew();
            ifstream fin;
            fin.open("remember.txt");
            if (!fin) {
                cout << "文件打开失败，请稍后再试" << endl;
            }
            fin >> cnt1 >> cnt2;
            for (int i = 0; i < 7; i++)
                for (int j = 0; j < 7; j++)
                    fin >> map[i][j];
            fin.close();
        }
        system("cls");
        print(map); //输出盘面
        game();
    }
}

void print(int map[][7]) {//输出棋盘

    cout << "     ━━━━━━━━━━━━━━━━━━━━━━━━━ " << endl;
    cout << "     玩家: " << cnt1 << "     VS     电脑: " << cnt2 << endl;
    cout << "     ━━━━━━━━━━━━━━━━━━━━━━━━━ " << endl;
    cout << "     1   2   3   4   5   6   7  " << endl;
    cout << "   ┏ ━ ┳ ━ ┳ ━ ┳ ━ ┳ ━ ┳ ━ ┳ ━ ┓" << endl;
    cout << " 1 ┃";
    for (int i = 0; i < 7; i++)
        if (map[0][i] == 1) cout << " ●┃";
        else if (map[0][i] == -1) cout << " ○┃";
        else cout << "   ┃";
    cout << endl;
    for (int i = 1; i < 7; i++) {
        cout << "   ┣ ━ ╋ ━ ╋ ━ ╋ ━ ╋ ━ ╋ ━ ╋ ━ ┫" << endl;
        cout << " " << i + 1 << " ┃";
        for (int j = 0; j < 7; j++)
            if (map[i][j] == 1) cout << " ●┃";
            else if (map[i][j] == -1) cout << " ○┃";
            else cout << "   ┃";
        cout << endl;
    }
    cout << "   ┗ ━ ┻ ━ ┻ ━ ┻ ━ ┻ ━ ┻ ━ ┻ ━ ┛" << endl;
}

void renew() {//初始化
    memset(map, 0, sizeof(map));//全部置0
    map[0][0] = map[6][6] = -1;  //左上右下放玩家，记为-1，是黑棋，数量记为cnt2      
    map[6][0] = map[0][6] = 1; //右上左下放电脑，记为1，是白棋，数量记为cnt1
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

void change(int x1, int y1, int x2, int y2, int color, int map[][7]) { //任一方落子后 改变颜色,并计数
    if (color == 1) {
        for (int dir = 0; dir < 8; dir++)
        {
            int x = x2 + delta[dir][0];
            int y = y2 + delta[dir][1];
            if (map[x][y] != -1 || x < 0 || x>6 || y < 0 || y>6)
                continue;
            map[x][y] = 1;
            cnt2++; cnt1--;
        }
        map[x2][y2] = 1; cnt2++;
        if (abs(x2 - x1) > 1 || abs(y2 - y1) > 1) {
            map[x1][y1] = 0; cnt2--;
        }
    }
    else {      //color为-1
        for (int dir = 0; dir < 8; dir++)
        {
            int x = x2 + delta[dir][0];
            int y = y2 + delta[dir][1];
            if (map[x][y] != 1 || x < 0 || x>6 || y < 0 || y>6)
                continue;
            map[x][y] = -1;
            cnt1++; cnt2--;
        }
        map[x2][y2] = -1; cnt1++;
        if (abs(x2 - x1) > 1 || abs(y2 - y1) > 1) {
            map[x1][y1] = 0; cnt1--;
        }
    }
}

void copy(int map1[][7], int map2[][7]) {//复制棋盘
    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 7; j++)
            map2[i][j] = map1[i][j];
    return;
}

void change2(int x1, int y1, int x2, int y2, int color, int map[][7]) {//任一方落子后 改变颜色,但不计数   (用于程序3）
    if (color == 1) {
        for (int dir = 0; dir < 8; dir++) // 影响邻近8个位置
        {
            int x = x2 + delta[dir][0];
            int y = y2 + delta[dir][1];
            if (map[x][y] != -1 || x < 0 || x>6 || y < 0 || y>6)
                continue;
            map[x][y] = 1;
        }
        map[x2][y2] = 1;
        if (abs(x2 - x1) > 1 || abs(y2 - y1) > 1) {
            map[x1][y1] = 0;
        }
    }
    else {      //color为-1
        for (int dir = 0; dir < 8; dir++) // 影响邻近8个位置
        {
            int x = x2 + delta[dir][0];
            int y = y2 + delta[dir][1];
            if (map[x][y] != 1 || x < 0 || x>6 || y < 0 || y>6)
                continue;
            map[x][y] = -1;
        }
        map[x2][y2] = -1;
        if (abs(x2 - x1) > 1 || abs(y2 - y1) > 1) {
            map[x1][y1] = 0;
        }
    }
}
 
//估值函数
int pinggu(int map[][7]) {
    int changfang[7][7] = { 0 };
    int sum1 = 0, sum2 = 0;
    for (int i = 0; i < 7; i++)
        for (int j = 0; j < 7; j++) {
            if (map[i][j] == mycolor) {
                sum1++;
                if (j > 0) changfang[i][j] = changfang[i][j - 1] + 1;
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
        tmax[ceng] = pinggu(map);//走完了，不能用简单地用评估函数判断了，待改进!!!!!!
    }
    else {
        copy(map, cun[ceng]);

        sort(choice[ceng], choice[ceng] + movecout[ceng], cmp);//一层贪心排序还是放在主程序中吧
        for (int i = 0; i < movecout[ceng]; i++) {
            if (choice[ceng][i].direction < 8) {       //如果导致的结果相同，则重复了，直接跳过
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
            if (tmax[ceng] >= tmin[ceng - 1]) return;  //如果当前这一步的最大结果tmax[ceng]，大于已搜索过的局部最小结果tmin[ceng-1]
        }                                                             //就可以剪枝了，因为对手在上一步必不可能采取这种下法而让我（电脑）得到更好的结果
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
        tmin[ceng] = pinggu(map); //终盘阶段，马上走完了，不能用简单地用评估函数判断了，待改进!!!!!!
    }
    else {
        copy(map, cun[ceng]);

        sort(choice[ceng], choice[ceng] + movecout[ceng], cmp); //一层贪心排序还是放在主程序中吧
        for (int i = 0; i < movecout[ceng]; i++) {
            if (choice[ceng][i].direction < 8) {       //如果导致的结果相同，则重复了，直接跳过
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
            if (tmin[ceng] <= tmax[ceng - 1]) return;//如果当前这一步的最小结果tmin[ceng]，小于已搜索过的局部最大结果tmax[ceng-1]
        }                                                 //就可以剪枝了，因为我（电脑）在上一步必不可能采取这种下法而让我（电脑）得到更差的结果
    }
    tmax[ceng - 1] = tmax[ceng - 1] > tmin[ceng] ? tmax[ceng - 1] : tmin[ceng];
}

void computerturn() {
    findmove(mycolor, 0);
    int mmax = -1000000, mx = 0, my = 0, fromx = 0, fromy = 0;
    copy(map, cun[0]);
    sort(choice[0], choice[0] + movecout[0], cmp);//一层贪心排序还是放在主程序中吧
    for (int i = 0; i < movecout[0]; i++) {
        if (choice[0][i].direction < 8) {       //如果导致的结果相同，则重复了，直接跳过
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
    change(fromx, fromy, mx, my, 1, map);

    /* 调试用
    end = clock();   //结束时间
    cout << "time = " << double((double)end - start) / CLOCKS_PER_SEC << "s" << endl;  //输出时间（单位：ｓ）
    cout << "选择种数：" << movecout[0] << endl;
    cout<<"终止点价值:"<<choice[0][40].jiazhi<<endl;
    cout << "预剪枝数" << jianzhi << endl;
    */
    return;
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

                choice[ceng][movecout[ceng]].jiazhi = 0;
                for (int d = 0; d < 8; d++) // 计数邻近8个位置的敌方棋子数,用于一步贪心
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

void game() {//游戏主程序
    int x1, y1, x2, y2;
    char tmp[20] = { '\0' };
    copy(map, now);
    for (int tttt = 0;; tttt++) {
        copy(now, last); lastcnt1 = nowcnt1; lastcnt2 = nowcnt2;
        copy(map, now); nowcnt1 = cnt1; nowcnt2 = cnt2;
        bool continuee = false;     //判断电脑走完，棋局是否结束
        if (continueornot(-1, map))
            continuee = true;
        if (!continuee) {
            if (cnt1 >= 25) cout << "胜利!" << endl;
            else cout << "失败" << endl;
            return;
        }

        bool right_input = false; bool huiqi = false;
        for (int tt = 0;; tt++) {
            cout << "请输入要移动的棋子的坐标；调用菜单请输入0";
            if (!huiqi) cout << "，悔棋请输入9";
            cout << endl;
            cin.getline(tmp, 20);
            if (tmp[1] != '\0' && tmp[3] != '\0') { cout << "非法输入" << endl; continue; }
            if (tmp[1] == '\0') {
                if (!(tmp[0] == '0' || (!huiqi && tmp[0] == '9'))) {
                    cout << "非法输入!" << endl;
                    continue;
                }
                if (tmp[0] == '0') menu();
                if (endd) { system("cls"); return; }
                if (tmp[0] == '9') {
                    copy(last, map);
                    cnt1 = lastcnt1; cnt2 = lastcnt2;
                    system("cls"); print(map);
                    huiqi = true;
                    continue;
                }
            }
            else if (tmp[1] != ' ' || tmp[0] < '1' || tmp[0]>'7' || tmp[2] < '1' || tmp[2]>'7') { cout << "非法输入" << endl; continue; }
            x1 = tmp[0] - '0'; y1 = tmp[2] - '0';

            if (map[x1 - 1][y1 - 1] != -1) {
                cout << "未选中棋子!请重新输入" << endl; continue;
            }
            else {
                cout << "请输入目标位置" << endl;
                cin.getline(tmp, 20);
                if (tmp[1] != ' ' || tmp[3] != '\0' || tmp[0] < '1' || tmp[0]>'7' || tmp[2] < '1' || tmp[2]>'7') {
                    cout << "非法输入" << endl; continue;
                }
                else x2 = tmp[0] - '0'; y2 = tmp[2] - '0';
                if (abs(x2 - x1) > 2 || abs(y2 - y1) > 2 || (x1 == x2 && y1 == y2) || map[x2 - 1][y2 - 1] != 0) {
                    cout << "移动不合法!请重新输入" << endl;
                    continue;
                }
                else right_input = true;
            }
            if (right_input) break;
        }

        change(x1 - 1, y1 - 1, x2 - 1, y2 - 1, -1, map);
        system("cls");
        print(map);
        bool continueee = false;     //判断玩家走完，棋局是否结束       
        if (continueornot(1, map))
            continueee = true;
        if (!continueee) {
            if (cnt2 >= 25) cout << "失败" << endl;
            else cout << "胜利" << endl;
            return;
        }
        computerturn();  //电脑进行决策 (随机 /一步贪心 /多步贪心即minmax算法/ 加剪枝/再加记忆数组/完善评估函数）
        Sleep(100);
        system("cls");
        print(map);     //电脑走完，输出盘面
    }
}






