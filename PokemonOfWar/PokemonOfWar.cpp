/******************************************
*	游戏名称：Pokemon Of War
*	编译环境：vc2017 + EasyX_20200109(beta)
*	Maker：	  panyu.tan
*	最初版本：2020/2/7
*	最后修改：2020/2/8
*******************************************/

#include <graphics.h>
#include <conio.h>
#include "tools.h"
#pragma comment( lib, "MSIMG32.LIB")	// 引用该库才能使用 TransparentBlt 函数


#define WINDOWS_WIDTH 800
#define WINDOWS_HIGH 600
#define PLAYER_WIDTH 32
#define PLAYER_HIGH 50					//这个高度人物的头顶会多出5个像素
#define POKEMON_NUMBER 5

IMAGE g_img_city_map;					//1280*880
int   canvas[1281][881] = { 0 };		//定义地图画布坐标,使每个像素点坐标为0，0为无障碍，1为有障碍不能通过
IMAGE g_img_player_walk;
int   g_player_picture_i;				//player图片,i为整张人物图的横坐标，j为纵坐标
int   g_player_picture_j;
int   g_player_x;						//player初始位置
int   g_player_y;
int   g_map_x;							//map截取的位置
int   g_map_y;



struct pokemon
{
	int  x;								//宝可梦在地图的位置
	int  y;
	int  number;						//编号
	char name[30];
	int  level;
	int  experience;					//宝可梦经验
	int  bleed;							//血量
	int  attack;
	int  defence;						//防御
	int  judge;							//遭遇判断，1为可以遇到，0为在宝可梦时间没有到之前不会刷新
	int	 time;							//宝可梦刷新时间周期
	char skill_1_name[30];				//技能名字
	int  skill_1_damage;				//技能基础伤害
	char skill_2_name[30];
	int  skill_2_damage;
	char skill_3_name[30];
	int  skill_3_damage;
	char skill_4_name[30];
	int  skill_4_damage;
}PM[POKEMON_NUMBER];



void starup_map_player();
void show_map();
void gameover();
void transparentimage(IMAGE *, int, int, IMAGE *, UINT);	//指定透明色贴图
void interface_change_animatio(int, int);
void is_fight();
void show_fight(pokemon *, pokemon *);
void startup_pokemon();
void show_dialog_box();
void CharToTchar(const char *, TCHAR *);



void show_dialog_box()
{
	setfillcolor(RGB(19, 181, 177));
	solidroundrect(0, WINDOWS_HIGH * 4 / 5, WINDOWS_WIDTH, WINDOWS_HIGH, 10, 10);
	setfillcolor(WHITE);
	solidroundrect(20, WINDOWS_HIGH * 4 / 5 + 5, WINDOWS_WIDTH - 20, WINDOWS_HIGH - 5, 10, 10);

	LOGFONT f;
	gettextstyle(&f);
	wcscpy_s(f.lfFaceName, _T("黑体"));
	f.lfHeight = 30;
	f.lfQuality = ANTIALIASED_QUALITY;		//抗锯齿
	settextstyle(&f);
	setbkmode(TRANSPARENT);					//设置文字背景透明			
	settextcolor(BLACK);

	outtextxy(80, WINDOWS_HIGH * 4 / 5 + 20, _T("你醒了？勇者"));
	outtextxy(80, WINDOWS_HIGH * 4 / 5 + 60, _T("test english"));
	FlushBatchDraw();
	Sleep(1000);
}



void startup_pokemon()
{
	PM[0].x = 367;
	PM[0].y = 420;
	strcpy(PM[0].skill_1_name, "scream");
	strcpy(PM[0].skill_2_name, "thrune");
	strcpy(PM[0].skill_3_name, "haaaaa");
	strcpy(PM[0].skill_4_name, "fireandwater");
}



void is_fight()
{
	for (int i = 0; i < POKEMON_NUMBER; i++)
	{
		if (PM[i].x >= g_map_x + g_player_x && PM[i].x < g_map_x + g_player_x + PLAYER_WIDTH
			&& PM[i].y >= g_map_y + g_player_y && PM[i].y < g_map_y + g_player_y + PLAYER_HIGH)
		{
			interface_change_animatio(WINDOWS_WIDTH, WINDOWS_HIGH);
			show_fight(&PM[0], &PM[1]);
			interface_change_animatio(WINDOWS_WIDTH, WINDOWS_HIGH);
		}
	}
	
}



void show_fight(pokemon *own_pm, pokemon *enemy_pm)
{
	int fight_choose = 0;//0为战斗准备，1为进入战斗，2为使用物品，3为替换宝可梦，4为逃跑
	//战斗准备选择光标
	int choose_x = WINDOWS_WIDTH * 3 / 5 + 30;
	int choose_y = WINDOWS_HIGH * 3 / 4 + 30;
	//技能选择光标坐标
	int skill_x = 60;
	int skill_y = WINDOWS_HIGH * 3 / 4 + 30;

	/************************遇敌提示********************************/
	//战斗场景图片
	IMAGE img_fight_map;
	loadimage(&img_fight_map, _T("资源文件\\maps\\fight_map.png"));
	putimage(0, 0, &img_fight_map);
	//敌人图片
	IMAGE img_enemy_pm;
	loadimage(&img_enemy_pm, _T("资源文件\\Pokemons\\enemy\\Entei.png"));
	transparentimage(NULL, 520, 130, &img_enemy_pm, RGB(255, 0, 255));
	setfillcolor(RGB(241, 145, 73));
	solidroundrect(0, WINDOWS_HIGH * 3 / 4, WINDOWS_WIDTH, WINDOWS_HIGH, 10, 10);
	setfillcolor(WHITE);
	solidroundrect(20, WINDOWS_HIGH * 3 / 4 + 5, WINDOWS_WIDTH - 20, WINDOWS_HIGH - 5, 10, 10);
	LOGFONT f;
	gettextstyle(&f);
	wcscpy_s(f.lfFaceName, _T("宋体"));
	f.lfHeight = 25;
	f.lfQuality = ANTIALIASED_QUALITY;		//抗锯齿
	settextstyle(&f);
	setbkmode(TRANSPARENT);					//设置文字背景透明			
	settextcolor(BLACK);
	outtextxy(50, WINDOWS_HIGH * 3 / 4 + 30, _T("野生的敌人出现！！！"));
	FlushBatchDraw();
	Sleep(1000);

	//清除键盘缓冲区,防止切换过场动画时候按到其他按键
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));	

	while (1)
	{
		//战斗场景图片
		IMAGE img_fight_map;
		loadimage(&img_fight_map, _T("资源文件\\maps\\fight_map.png"));
		putimage(0, 0, &img_fight_map);

		//我方宝可梦图片
		IMAGE img_own_pm;
		loadimage(&img_own_pm, _T("资源文件\\Pokemons\\own\\Pikachu.png"));
		transparentimage(NULL, 150, 300, &img_own_pm, RGB(255, 0, 255));	//粉红色背景255，0，255

		//敌人图片
		IMAGE img_enemy_pm;
		loadimage(&img_enemy_pm, _T("资源文件\\Pokemons\\enemy\\Entei.png"));
		transparentimage(NULL, 520, 130, &img_enemy_pm, RGB(255, 0, 255));

		//最下方的战斗状态框
		setfillcolor(RGB(241, 145, 73));
		solidroundrect(0, WINDOWS_HIGH * 3 / 4, WINDOWS_WIDTH, WINDOWS_HIGH, 10, 10);
		setfillcolor(WHITE);
		solidroundrect(20, WINDOWS_HIGH * 3 / 4 + 5, WINDOWS_WIDTH - 20, WINDOWS_HIGH - 5, 10, 10);

		//右边的框框
		setfillcolor(RGB(160, 160, 160));
		fillroundrect(WINDOWS_WIDTH * 3 / 5, WINDOWS_HIGH * 3 / 4, WINDOWS_WIDTH, WINDOWS_HIGH, 10, 10);
		setfillcolor(WHITE);
		solidroundrect(WINDOWS_WIDTH * 3 / 5 + 10, WINDOWS_HIGH * 3 / 4 + 10,
			WINDOWS_WIDTH - 10, WINDOWS_HIGH - 10, 10, 10);


		//右下方我方状态框
		setfillcolor(WHITE);
		fillroundrect(WINDOWS_WIDTH * 3 / 5, 330, WINDOWS_WIDTH * 3 / 5 + 250, 330 + 90, 10, 10);

		//左上方敌方状态框
		setfillcolor(WHITE);
		fillroundrect(130, 100, 130 + 250, 100 + 70, 10, 10);
		
		if (fight_choose == 0)
		{
			LOGFONT f;
			gettextstyle(&f);
			wcscpy_s(f.lfFaceName, _T("宋体"));
			f.lfHeight = 25;
			f.lfQuality = ANTIALIASED_QUALITY;		//抗锯齿
			settextstyle(&f);
			setbkmode(TRANSPARENT);					//设置文字背景透明			
			settextcolor(BLACK);

			outtextxy(WINDOWS_WIDTH * 3 / 5 + 50, WINDOWS_HIGH * 3 / 4 + 30, _T("战斗"));
			outtextxy(WINDOWS_WIDTH * 3 / 5 + 200, WINDOWS_HIGH * 3 / 4 + 30, _T("物品"));
			outtextxy(WINDOWS_WIDTH * 3 / 5 + 50, WINDOWS_HIGH * 3 / 4 + 90, _T("宝可梦"));
			outtextxy(WINDOWS_WIDTH * 3 / 5 + 200, WINDOWS_HIGH * 3 / 4 + 90, _T("逃跑"));
			
			//战斗准备选择光标
			setfillcolor(RGB(49, 49, 49));
			POINT pts[] = { { choose_x, choose_y },{ choose_x,choose_y + 20 },{ choose_x + 15, choose_y + 10 } };
			solidpolygon(pts, 3);
			char input;
			if (_kbhit())
			{
				input = _getch();
				if (input == 'w' && choose_y - 60 >= WINDOWS_HIGH * 3 / 4 + 30)
					choose_y -= 60;
				if (input == 's' && choose_y + 60 <= WINDOWS_HIGH * 3 / 4 + 90)
					choose_y += 60;
				if (input == 'a' && choose_x - 150 >= WINDOWS_WIDTH * 3 / 5 + 30)
					choose_x -= 150;
				if (input == 'd' && choose_x + 150 <= WINDOWS_WIDTH * 3 / 5 + 180)
					choose_x += 150;
				if (input == 'j')
				{
					if (choose_x == WINDOWS_WIDTH * 3 / 5 + 30 && choose_y == WINDOWS_HIGH * 3 / 4 + 30)
						fight_choose = 1;	//战斗
					if (choose_x == WINDOWS_WIDTH * 3 / 5 + 180 && choose_y == WINDOWS_HIGH * 3 / 4 + 30)
						fight_choose = 2;	//物品
					if (choose_x == WINDOWS_WIDTH * 3 / 5 + 30 && choose_y == WINDOWS_HIGH * 3 / 4 + 90)
						fight_choose = 3;	//替换宝可梦
					if (choose_x == WINDOWS_WIDTH * 3 / 5 + 180 && choose_y == WINDOWS_HIGH * 3 / 4 + 90)
					{
						fight_choose = 4;	//逃跑
						break;
					}
				}
			}
		}

		if (fight_choose == 1)
		{
			setfillcolor(RGB(49, 49, 49));
			POINT pts[] = { { skill_x, skill_y },{ skill_x,skill_y + 20 },{ skill_x + 15, skill_y + 10 } };
			solidpolygon(pts, 3);

			TCHAR pm_skill[20];
			CharToTchar(own_pm->skill_1_name, pm_skill);
			outtextxy(80, WINDOWS_HIGH * 3 / 4 + 30, pm_skill);
			CharToTchar(own_pm->skill_2_name, pm_skill);
			outtextxy(280, WINDOWS_HIGH * 3 / 4 + 30, pm_skill);
			CharToTchar(own_pm->skill_3_name, pm_skill);
			outtextxy(80, WINDOWS_HIGH * 3 / 4 + 90, pm_skill);
			CharToTchar(own_pm->skill_4_name, pm_skill);
			outtextxy(280, WINDOWS_HIGH * 3 / 4 + 90, pm_skill);

			char input;
			if (_kbhit())
			{
				input = _getch();
				if (input == 'w' && skill_y - 60 >= WINDOWS_HIGH * 3 / 4 + 30)
					skill_y -= 60;
				if (input == 's' && skill_y + 60 <= WINDOWS_HIGH * 3 / 4 + 90)
					skill_y += 60;
				if (input == 'a' && skill_x - 200 >= 60)
					skill_x -= 200;
				if (input == 'd' && skill_x + 200 <= 260)
					skill_x += 200;
				if (input == 'j')
				{
					if (skill_x == 60 && skill_y == WINDOWS_HIGH * 3 / 4 + 30)
						//fight_choose = 1;	//技能1
					if (skill_x == 260 && skill_y == WINDOWS_HIGH * 3 / 4 + 30)
						//fight_choose = 2;	//技能2
					if (skill_x == 60 && skill_y == WINDOWS_HIGH * 3 / 4 + 90)
						//fight_choose = 3;	//技能3
					if (skill_x == 260 && skill_y == WINDOWS_HIGH * 3 / 4 + 90)
					{
						//fight_choose = 4;	//技能4
					}
				}
				if (input == 'k')
					fight_choose = 0;
			}
		}

		FlushBatchDraw();

	}
}



void starup_map_player()
{
	initgraph(WINDOWS_WIDTH, WINDOWS_HIGH);

	loadimage(&g_img_city_map, _T("资源文件\\maps\\city_map.png"));
	g_map_x = 0;
	g_map_y = 0;
	for (int i = 0; i < 1281; i++)
	{
		for (int j = 0; j < 881; j++)
		{
			if (i < 140 || j >= 737 || j < 205)							//外围
				canvas[i][j] = 1;
			if ((i >= 140 && i < 278) && (j >= 205 && j < 394))			//左上侧空地
				canvas[i][j] = 1;
			if ((i >= 278 && i < 786) && (j >= 205 && j < 285))			//左上侧黄色小房子
				canvas[i][j] = 1;
			if ((i >= 918 && i < 1070) && (j >= 205 && j < 285))		//右边蓝色房子附近小石块
				canvas[i][j] = 1;
			if ((i >= 1135 && i < 1242) && (j >= 227) && j < 320)		//右边地洞
				canvas[i][j] = 1;
			if ((i >= 458 && i < 711) && (j >= 285) && j < 365)			//中间红色房子
				canvas[i][j] = 1;
			if ((i >= 140 && i < 218) && (j >= 463 && j < 737))			//左下的树木
				canvas[i][j] = 1;
			if ((i >= 284 && i < 508) && (j >= 485 && j < 635))			//左下黄色房子
				canvas[i][j] = 1;
			if ((i >= 743 && i < 927) && (j >= 450 && j < 517))			//右下黄色房子
				canvas[i][j] = 1;
			if (i >= 743 && (j >= 640 && j < 737))						//右下横着的树木
				canvas[i][j] = 1;
			if ((i >= 927 && i < 1070) && (j >= 445 && j < 737))		//右下的树木	
				canvas[i][j] = 1;
		}
	}

	loadimage(&g_img_player_walk, _T("资源文件\\player1.png"));
	g_player_picture_i = g_player_picture_j = 0;
	g_player_x = 150;
	g_player_y = 395;

	BeginBatchDraw();
}



void show_map()
{
	putimage(0, 0, WINDOWS_WIDTH, WINDOWS_HIGH, &g_img_city_map, g_map_x, g_map_y);
	
	//人物行走图
	HDC dstDC = GetImageHDC(NULL);
	HDC srcDC = GetImageHDC(&g_img_player_walk);
	int srcX = g_player_picture_i * PLAYER_WIDTH;
	int srcY = g_player_picture_j * PLAYER_HIGH;
	//绿色背景
	TransparentBlt(dstDC, g_player_x, g_player_y, PLAYER_WIDTH, PLAYER_HIGH, 
		srcDC, srcX, srcY, PLAYER_WIDTH, PLAYER_HIGH, RGB(29, 248, 6));
	//putimage(g_player_x, g_player_y, PLAYER_WIDTH, PLAYER_HIGH, &g_player_walk, g_player_picture_i*PLAYER_WIDTH, g_player_picture_j*PLAYER_HIGH);
	//transparentimage(NULL, g_player_x, g_player_y, PLAYER_WIDTH, PLAYER_HIGH, &g_img_player_walk, RGB(29,248,6));
	
	FlushBatchDraw();
}



void operate()
{
	int step = 10;
	char input;
	if (_kbhit())
	{
		input = _getch();
		g_player_picture_i++;

		if (input == 's' )
		{
			g_player_picture_j = 0;
			if (canvas[g_map_x + g_player_x][g_map_y + g_player_y + PLAYER_HIGH + step] == 0)
			{
				if (g_player_y <= WINDOWS_HIGH / 2 || g_map_y + WINDOWS_HIGH > 870)
					g_player_y += step;
				else
					g_map_y += step;
			}
		}
		if (input == 'a')
		{
			g_player_picture_j = 1;
			if (canvas[g_map_x + g_player_x - step][g_map_y + g_player_y] == 0)
			{
				if (g_player_x > WINDOWS_WIDTH / 2 || g_map_x < 10)
					g_player_x -= step;
				else
					g_map_x -= step;
			}	
		}
		if (input == 'd')
		{
			g_player_picture_j = 2;
			//+PLAYER_WIDTH 和 +PLAYER_HIGH 是因为绘画人物时候是从左上角开始
			if (canvas[g_map_x + g_player_x + PLAYER_WIDTH + step][g_map_y + g_player_y + PLAYER_HIGH] == 0)
			{
				if (g_player_x <= WINDOWS_WIDTH / 2 || g_map_x + WINDOWS_WIDTH > 1270)
					g_player_x += step;
				else
					g_map_x += step;
			}
		}
		if (input == 'w')
		{
			g_player_picture_j = 3;
			if (canvas[g_map_x + g_player_x][g_map_y + g_player_y - step] == 0)
			{
				if (g_player_y > WINDOWS_HIGH / 2 || g_map_y < 10)
					g_player_y -= step;
				else
					g_map_y -= step;
			}
		}
		if (input == 'j')
		{
			//interface_change_animatio(WINDOWS_WIDTH, WINDOWS_HIGH);
			show_dialog_box();
		}
		if (g_player_picture_i == 4)
			g_player_picture_i = 0;
	}
}



void gameover()
{
	EndBatchDraw();
	_getch();
	closegraph();
}



int main(void)
{
	startup_pokemon();
	starup_map_player();
	while (1)
	{
		show_map();
		is_fight();
		operate();
	}
	gameover();
	return 0;
}