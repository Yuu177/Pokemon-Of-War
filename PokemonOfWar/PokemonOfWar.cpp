/******************************************
*	游戏名称：Pokemon Of War
*	编译环境：vc2017 + EasyX_20200109(beta)
*	Maker：	  panyu.tan
*	最初版本：2020/2/7
*	最后修改：2020/2/8
*******************************************/

#include <graphics.h>
#include <conio.h>
#include <time.h>
#include "tools.h"
#include "pokemon_skills.h"
//#include <stdio.h>
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



/*
struct player
{
	//携带的物品
	int Potion;
	int Super_Potion;
	int Hyper_Potion;
	int Max_Potion;
	//携带的宝可梦
	struct pokemon pokemons[4];
}struct_player;
*/



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



void starup_map_and_player();
void show_map();
void gameover();
void transparentimage(IMAGE *, int, int, IMAGE *, UINT);	//指定透明色贴图
void interface_change_animatio(int, int);
void is_fight();
void show_fight(pokemon *, pokemon *);
void startup_pokemon();
void show_dialog_box();
void CharToTchar(const char *, TCHAR *);
void fight_operation_interface(int *, int *, int *, TCHAR[][20], pokemon *, pokemon *, int *, int *, int *);
void use_skill(char *);
void fight_interface(pokemon *, int *, int, pokemon *, int *, int);
void show_fight_down_box(pokemon *, char *, int, int, bool);
void startup_font();
void load_pokemon_picture(pokemon *, int, int);
void pokemons_refresh();
void show_fight_right_box(int, int, int, pokemon *);



void startup_font()
{
	LOGFONT f;
	gettextstyle(&f);
	wcscpy_s(f.lfFaceName, _T("黑体"));
	f.lfHeight = 25;
	f.lfQuality = ANTIALIASED_QUALITY;		//抗锯齿
	settextstyle(&f);
	setbkmode(TRANSPARENT);					//设置文字背景透明			
	settextcolor(BLACK);
}



//根据战斗时候的不同选项在下方对话框显示文字描述
void show_fight_down_box(pokemon *pm, char *str, int fight_choose, int n, bool show_skill_end)
{
	setfillcolor(RGB(241, 145, 73));
	solidroundrect(0, WINDOWS_HIGH * 3 / 4, WINDOWS_WIDTH, WINDOWS_HIGH, 10, 10);
	setfillcolor(WHITE);
	solidroundrect(20, WINDOWS_HIGH * 3 / 4 + 5, WINDOWS_WIDTH - 20, WINDOWS_HIGH - 5, 10, 10);
	
	setfillcolor(RGB(49, 49, 49));
	POINT pts[] = { { 50,  WINDOWS_HIGH - 20 },{ 70,WINDOWS_HIGH - 20 },{ 50 + 10,  WINDOWS_HIGH - 10} };
	solidpolygon(pts, 3);
	//startup_font();

	//show_skill_end 为false时候表示：技能释放前，为true时是技能释放后
	//fight_choose = 1时为己方攻击描述，为0时候时敌方
	if ((fight_choose == 1 || fight_choose == 0) && show_skill_end == false)
	{
		char do_str[20] = "使出了";
		char name[30];
		strcpy(name,pm->name);
		strcat(name, do_str);
		TCHAR show_str1[30];
		CharToTchar(name, show_str1);
		outtextxy(50, WINDOWS_HIGH * 3 / 4 + 30, show_str1);
		
		char temp_str[20];
		strcpy(temp_str, str);
		char end_str[2] = "!";
		strcat(temp_str, end_str);
		TCHAR show_str2[20];
		CharToTchar(temp_str, show_str2);
		outtextxy(50, WINDOWS_HIGH * 3 / 4 + 60, show_str2);
		next_step();
	}
	else if ((fight_choose == 1 || fight_choose ==0) && show_skill_end == true)
	{
		char temp_str[30] = "对方受到了";
		char damage_str[10];
		_itoa(n, damage_str, 10);
		strcat(temp_str, damage_str);
		char end_str[20] = "点伤害！";
		strcat(temp_str, end_str);
		TCHAR show_str[30];
		CharToTchar(temp_str, show_str);
		outtextxy(50, WINDOWS_HIGH * 3 / 4 + 30, show_str);
		FlushBatchDraw();
		//这里是技能显示完后因为要显示造成多少伤害的描述，所以技能sleep需要修改减少1000ms
		Sleep(1000);
	}
	else if (fight_choose == 2)
	{
		char do_str[20] = "使用了";
		strcat(do_str, str);
		TCHAR show_str1[20];
		CharToTchar(do_str, show_str1);
		outtextxy(50, WINDOWS_HIGH * 3 / 4 + 30, show_str1);

		char name[30];
		strcpy(name, pm->name);
		char mid[20] = "回复了";
		strcat(name, mid);
		char add_str[10];
		_itoa(n, add_str, 10);
		strcat(name, add_str);
		char end_str[20] = "点生命！";
		strcat(name, end_str);
		TCHAR show_str2[30];
		CharToTchar(name, show_str2);
		outtextxy(50, WINDOWS_HIGH * 3 / 4 + 60, show_str2);
		next_step();
	}
	else if (fight_choose == 3)
	{
		char c_re_str[50] = "替换了宝可梦...回来吧";
		char bef_pm[20];
		strcpy(bef_pm, pm->name);
		strcat(c_re_str, bef_pm);
		TCHAR w_re_str[50];
		CharToTchar(c_re_str, w_re_str);
		outtextxy(50, WINDOWS_HIGH * 3 / 4 + 30, w_re_str);
		next_step();
		char c_str[20] = "就是你了";
		char aft_pm[20];
		strcpy(aft_pm, str);
		strcat(c_str, aft_pm);
		TCHAR w_str[20];
		CharToTchar(c_str, w_str);
		outtextxy(50, WINDOWS_HIGH * 3 / 4 + 60, w_str);
		next_step();
	}
}




void show_dialog_box()
{
	setfillcolor(RGB(19, 181, 177));
	solidroundrect(0, WINDOWS_HIGH * 3 / 4, WINDOWS_WIDTH, WINDOWS_HIGH, 10, 10);
	//solidroundrect(0, WINDOWS_HIGH * 4 / 5, WINDOWS_WIDTH, WINDOWS_HIGH, 10, 10);
	setfillcolor(WHITE);
	solidroundrect(20, WINDOWS_HIGH * 3 / 4 + 5, WINDOWS_WIDTH - 20, WINDOWS_HIGH - 5, 10, 10);
	//solidroundrect(20, WINDOWS_HIGH * 4 / 5 + 5, WINDOWS_WIDTH - 20, WINDOWS_HIGH - 5, 10, 10);

	setfillcolor(RGB(49, 49, 49));
	POINT pts[] = { { 50,  WINDOWS_HIGH - 20 },{ 70,WINDOWS_HIGH - 20 },{ 50 + 10,  WINDOWS_HIGH - 10} };
	solidpolygon(pts, 3);

	//startup_font();

	outtextxy(80, WINDOWS_HIGH * 4 / 5 + 20, _T("你醒了？勇者"));
	outtextxy(80, WINDOWS_HIGH * 4 / 5 + 60, _T("test english"));
	FlushBatchDraw();
	next_step();
}








void pokemons_refresh()
{
	for (int i = 0; i < POKEMON_NUMBER; i++)
	{
		if (PM[i].judge == 0 && PM[i].time < 10000)
		{
			PM[i].time++;
		}
		if (PM[i].judge == 0 && PM[i].time == 10000)
		{
			PM[i].judge = 1;
			PM[i].time = 0;
		}
	}
}




void is_fight()
{
	for (int i = 0; i < POKEMON_NUMBER; i++)
	{
		if (PM[i].x >= g_map_x + g_player_x && PM[i].x < g_map_x + g_player_x + PLAYER_WIDTH && 
			PM[i].y >= g_map_y + g_player_y && PM[i].y < g_map_y + g_player_y + PLAYER_HIGH && PM[i].judge == 1)
		{
			PM[i].judge = 0;
			interface_change_animatio(WINDOWS_WIDTH, WINDOWS_HIGH);
			show_fight(&PM[0], &PM[i]);
			interface_change_animatio(WINDOWS_WIDTH, WINDOWS_HIGH);
		}
	}
	
}



void pm_attack_pm(pokemon *pmA, pokemon *pmD, char *pmA_skill_name, 
					int pmA_skill_damage, int *fight_choose, int *now_bleed)
{
	//基础伤害＝(((攻击方等级×2÷5＋2)×技能威力×攻击力÷敌方防御力)÷50)＋2
	int damage = (((pmA->level * 2 / 5 + 2) * pmA_skill_damage * pmA->attack / pmD->defence) / 50) + 2;
	show_fight_down_box(pmA, pmA_skill_name, *fight_choose, damage, false);	//xxx使出了xx
	use_skill(pmA_skill_name);
	show_fight_down_box(pmA, pmA_skill_name, *fight_choose, damage, true);	//xx受到了xx
	*now_bleed -= damage;
}



void use_skill(char *skill_name)
{
	if (strcmp(skill_name, "boom") == 0)			boom();
	if (strcmp(skill_name, "scream") == 0)			scream();
	if (strcmp(skill_name, "catchit") == 0)			catchit();
	if (strcmp(skill_name, "lighting") == 0)		lighting();
	if (strcmp(skill_name, "flash") == 0)			flash();
	if (strcmp(skill_name, "thunder") == 0)			thunder();
	if (strcmp(skill_name, "waterattack") == 0)		waterattack();
	if (strcmp(skill_name, "sword") == 0)			sword();
	if (strcmp(skill_name, "firehigh") == 0)		firehigh();
}




void show_fight_right_box(int x, int y, int fight_choose, pokemon *pm)
{
	if (fight_choose == 1)
	{
		outtextxy(WINDOWS_WIDTH * 3 / 5 + 40, WINDOWS_HIGH * 3 / 4 + 30, _T("PP:"));
		outtextxy(WINDOWS_WIDTH * 3 / 5 + 40, WINDOWS_HIGH * 3 / 4 + 80, _T("技能强度:"));
		TCHAR damage[10];
		if (x == 60 && y == WINDOWS_HIGH * 3 / 4 + 30)
			wsprintf(damage, _T("%d"), pm->skill_1_damage);
		if (x == 260 && y == WINDOWS_HIGH * 3 / 4 + 30)
			wsprintf(damage, _T("%d"), pm->skill_2_damage);
		if (x == 60 && y == WINDOWS_HIGH * 3 / 4 + 90)
			wsprintf(damage, _T("%d"), pm->skill_3_damage);
		if (x == 260 && y == WINDOWS_HIGH * 3 / 4 + 90)
			wsprintf(damage, _T("%d"), pm->skill_4_damage);
		outtextxy(WINDOWS_WIDTH * 3 / 5 + 200, WINDOWS_HIGH * 3 / 4 + 80, damage);
	}
	else if (fight_choose == 2)
	{
		outtextxy(WINDOWS_WIDTH * 3 / 5 + 40, WINDOWS_HIGH * 3 / 4 + 30, _T("剩余:"));
		outtextxy(WINDOWS_WIDTH * 3 / 5 + 40, WINDOWS_HIGH * 3 / 4 + 80, _T("回复量:"));
		TCHAR bleed[10];
		if (x == 60 && y == WINDOWS_HIGH * 3 / 4 + 30)
			lstrcpy(bleed, _T("10"));
		if (x == 260 && y == WINDOWS_HIGH * 3 / 4 + 30)
			lstrcpy(bleed, _T("20"));
		if (x == 60 && y == WINDOWS_HIGH * 3 / 4 + 90)
			lstrcpy(bleed, _T("30"));
		if (x == 260 && y == WINDOWS_HIGH * 3 / 4 + 90)
			lstrcpy(bleed, _T("全回复"));
		outtextxy(WINDOWS_WIDTH * 3 / 5 + 200, WINDOWS_HIGH * 3 / 4 + 80, bleed);
	}
	else if (fight_choose == 3)
	{
		outtextxy(WINDOWS_WIDTH * 3 / 5 + 40, WINDOWS_HIGH * 3 / 4 + 30, _T("生命值:"));
		TCHAR bleed[10];
		if (x == 60 && y == WINDOWS_HIGH * 3 / 4 + 30)
			wsprintf(bleed, _T("%d"), PM[0].bleed);
		if (x == 260 && y == WINDOWS_HIGH * 3 / 4 + 30)
			wsprintf(bleed, _T("%d"), PM[1].bleed);
		if (x == 60 && y == WINDOWS_HIGH * 3 / 4 + 90)
			wsprintf(bleed, _T("%d"), PM[2].bleed);
		if (x == 260 && y == WINDOWS_HIGH * 3 / 4 + 90)
			wsprintf(bleed, _T("%d"), PM[3].bleed);
		outtextxy(WINDOWS_WIDTH * 3 / 5 + 200, WINDOWS_HIGH * 3 / 4 + 30, bleed);
	}
}


//点入战斗，物品，宝可梦界面
void fight_operation_interface(int *x, int *y, int *fight_choose, TCHAR show_str[][20], pokemon *own_pm, 
								pokemon *enemy_pm, int *fight_turn, int *own_now_bleed, int *enemy_now_bleed)
{
	setfillcolor(RGB(49, 49, 49));
	POINT pts[] = { { *x, *y },{ *x,*y + 20 },{ *x + 15, *y + 10 } };
	solidpolygon(pts, 3);
	outtextxy(80, WINDOWS_HIGH * 3 / 4 + 30, show_str[0]);
	outtextxy(280, WINDOWS_HIGH * 3 / 4 + 30, show_str[1]);
	outtextxy(80, WINDOWS_HIGH * 3 / 4 + 90, show_str[2]);
	outtextxy(280, WINDOWS_HIGH * 3 / 4 + 90, show_str[3]);

	char input;
	if (_kbhit())
	{
		input = _getch();
		if (input == 'w' && *y - 60 >= WINDOWS_HIGH * 3 / 4 + 30)
		{
			*y -= 60;
		}
		if (input == 's' && *y + 60 <= WINDOWS_HIGH * 3 / 4 + 90)
			*y += 60;
		if (input == 'a' && *x - 200 >= 60)
			*x -= 200;
		if (input == 'd' && *x + 200 <= 260)
			*x += 200;
		if (input == 'j')
		{
			if (*x == 60 && *y == WINDOWS_HIGH * 3 / 4 + 30)
			{
				if (*fight_choose == 1)
					pm_attack_pm(own_pm, enemy_pm, own_pm->skill_1_name, own_pm->skill_1_damage, 
						fight_choose, enemy_now_bleed);
				else if (*fight_choose == 2)
				{
					//使用了xx，xx回复了xx点生命
					show_fight_down_box(own_pm, "Potion", *fight_choose, 10, false);
					*own_now_bleed += 10;
				}
				else if (*fight_choose == 3)
				{
					show_fight_down_box(own_pm, PM[2].name, *fight_choose, 0, false);
					*own_pm = PM[2];
					*own_now_bleed = PM[2].bleed;
				}
			}

			if (*x == 260 && *y == WINDOWS_HIGH * 3 / 4 + 30)
			{
				if (*fight_choose == 1)
					pm_attack_pm(own_pm, enemy_pm, own_pm->skill_2_name, own_pm->skill_2_damage, 
						fight_choose, enemy_now_bleed);
				else if	(*fight_choose == 2)
				{
					show_fight_down_box(own_pm, "Super Potion", *fight_choose, 20, false);
					*own_now_bleed += 20;
				}
				else if (*fight_choose == 3) 
				{
					show_fight_down_box(own_pm, PM[2].name, *fight_choose, 0, false);
					*own_pm = PM[2];
					*own_now_bleed = PM[2].bleed;
				}
			}

			if (*x == 60 && *y == WINDOWS_HIGH * 3 / 4 + 90)
			{
				if (*fight_choose == 1)
					pm_attack_pm(own_pm, enemy_pm, own_pm->skill_3_name, own_pm->skill_3_damage, 
						fight_choose, enemy_now_bleed);
				else if (*fight_choose == 2)
				{
					show_fight_down_box(own_pm, "Hyper Potion", *fight_choose, 30, false);
					*own_now_bleed += 30;
				}
				else if (*fight_choose == 3)
				{
					show_fight_down_box(own_pm, PM[2].name, *fight_choose, 0, false);
					*own_pm = PM[2];
					*own_now_bleed = PM[2].bleed;
				}
			}

			if (*x == 260 && *y == WINDOWS_HIGH * 3 / 4 + 90)
			{
				if (*fight_choose == 1)
					pm_attack_pm(own_pm, enemy_pm, own_pm->skill_4_name, own_pm->skill_4_damage, 
								fight_choose, enemy_now_bleed);
				else if (*fight_choose == 2)
				{
					show_fight_down_box(own_pm, "Max Potion", *fight_choose, own_pm->bleed, false);
					*own_now_bleed = own_pm->bleed;
				}
				else if (*fight_choose == 3)
				{
					show_fight_down_box(own_pm, PM[2].name, *fight_choose, 0, false);
					*own_pm = PM[2];
					*own_now_bleed = PM[2].bleed;
				}
			}
			//操作完轮到对方回合
			*fight_turn = 0;
			//每一回合都要返回准备界面
			*fight_choose = 0;
		}
		if (input == 'k')	*fight_choose = 0;
	}
	//当选择光标指向时候显示详细的信息
	show_fight_right_box(*x, *y, *fight_choose, own_pm);
}



void load_pokemon_picture(pokemon *pm, int x, int y)
{
	char head_path[50] = "资源文件\\Pokemons\\";
	char pmName[15];
	strcpy(pmName, pm->name);
	strcat(head_path, pmName);
	char end_str[10] = ".png";
	strcat(head_path, end_str);
	TCHAR pm_path[50];
	CharToTchar(head_path, pm_path);
	IMAGE img_pm;
	loadimage(&img_pm, pm_path);
	transparentimage(NULL, x, y, &img_pm, RGB(255, 0, 255));
}




//战斗开始界面绘画封住函数
void fight_interface(pokemon *own_pm, int *own_now_bleed, int own_bleed_width,
						pokemon *enemy_pm, int *enemy_now_bleed, int enemy_bleed_width)
{
	//startup_font();
	//战斗场景图片
	IMAGE img_fight_map;
	loadimage(&img_fight_map, _T("资源文件\\maps\\fight_map.png"));
	putimage(0, 0, &img_fight_map);
	//我方宝可梦图片
	load_pokemon_picture(own_pm, 150, 300);
	//敌人图片
	load_pokemon_picture(enemy_pm, 520, 130);

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
	TCHAR ownName[20];
	CharToTchar(own_pm->name, ownName);
	char c_ownLv[10];
	char strLv[15] = "Lv:";
	_itoa(own_pm->level, c_ownLv, 10);
	strcat(strLv, c_ownLv);
	TCHAR w_ownLv[10];
	CharToTchar(strLv, w_ownLv);
	//显示我方宝可梦名字和等级
	outtextxy(WINDOWS_WIDTH * 3 / 5 + 20, 340, ownName);
	outtextxy(WINDOWS_WIDTH * 3 / 5 + 170, 340, w_ownLv);
	//血条
	if (*own_now_bleed >= own_pm->bleed)
	{
		own_bleed_width = 220;
		*own_now_bleed = own_pm->bleed;
	}
	if (*own_now_bleed <= 0)
	{
		own_bleed_width = 0;
		*own_now_bleed = 0;
	}
	own_bleed_width = ((double)*own_now_bleed / own_pm->bleed) * own_bleed_width;

	setcolor(BLACK);
	setfillcolor(RED);
	solidrectangle(WINDOWS_WIDTH * 3 / 5 + 20, 375, WINDOWS_WIDTH * 3 / 5 + 20 + 220, 375 + 10);
	setfillcolor(GREEN);
	solidrectangle(WINDOWS_WIDTH * 3 / 5 + 20, 375, WINDOWS_WIDTH * 3 / 5 + 20+ own_bleed_width, 375 + 10);

	char left_bleed[10];
	_itoa(*own_now_bleed, left_bleed, 10);
	char full_bleed[10];
	_itoa(own_pm->bleed, full_bleed, 10);
	char temp[2] = "/";
	strcat(left_bleed, temp);
	strcat(left_bleed, full_bleed);
	TCHAR show_bleed_str[10];
	CharToTchar(left_bleed, show_bleed_str);
	//用x/y方式显示血量
	outtextxy(WINDOWS_WIDTH * 3 / 5 + 20, 390, show_bleed_str);	

	//经验蓝条
	setfillcolor(RGB(149, 149, 149));
	solidrectangle(WINDOWS_WIDTH * 3 / 5 + 130, 395, WINDOWS_WIDTH * 3 / 5 + 240, 395 + 8);
	setfillcolor(RGB(0, 255, 255));
	solidrectangle(WINDOWS_WIDTH * 3 / 5 + 130, 395, WINDOWS_WIDTH * 3 / 5 + 230, 395 + 8);

	//左上方敌方状态框
	setfillcolor(WHITE);
	fillroundrect(130, 100, 130 + 250, 100 + 70, 10, 10);
	TCHAR enemyName[20];
	CharToTchar(enemy_pm->name, enemyName);
	char c_enemyLv[10];
	char str_Lv[15] = "Lv:";
	_itoa(enemy_pm->level, c_enemyLv, 10);
	strcat(str_Lv, c_enemyLv);
	TCHAR w_enemyLv[10];
	CharToTchar(str_Lv, w_enemyLv);
	//显示敌人名字和等级
	outtextxy(130 + 15, 110, enemyName);	
	outtextxy(295, 110, w_enemyLv);
	//敌方血条
	if (*enemy_now_bleed >= enemy_pm->bleed)
	{
		enemy_bleed_width = 220;
		*enemy_now_bleed = enemy_pm->bleed;
	}
	if (*enemy_now_bleed <= 0)
	{
		enemy_bleed_width = 0;
		*enemy_now_bleed = 0;
	}
	enemy_bleed_width = ((double)*enemy_now_bleed / enemy_pm->bleed) * enemy_bleed_width;
	setfillcolor(RED);
	solidrectangle(145, 145, 365, 155);
	setfillcolor(GREEN);
	solidrectangle(145, 145, 145 + enemy_bleed_width, 155);
}




void show_fight(pokemon *own_pm, pokemon *enemy_pm)
{
	//1为player攻击，0为地方攻击
	int fight_turn = 1;
	//0为战斗准备，1为进入战斗，2为使用物品，3为替换宝可梦，4为逃跑
	int fight_choose = 0;
	//战斗准备选择光标 0
	int choose_x = WINDOWS_WIDTH * 3 / 5 + 30;
	int choose_y = WINDOWS_HIGH * 3 / 4 + 30;
	//技能选择光标坐标 1
	int skill_x = 60;
	int skill_y = WINDOWS_HIGH * 3 / 4 + 30;
	//物品选择光标 2
	int good_x = 60;
	int good_y = WINDOWS_HIGH * 3 / 4 + 30;
	//宝可梦选择光标 3
	int pokemon_x = 60;
	int pokemon_y = WINDOWS_HIGH * 3 / 4 + 30;
	//敌方血量长度
	int enemy_bleed_width = 220;					//血量宽度
	int enemy_now_bleed = enemy_pm->bleed;			//当前血量
	//player血量长度
	int own_bleed_width = 220;						
	int own_now_bleed = own_pm->bleed;	

	/************************遇敌提示********************************/
	//战斗场景图片
	IMAGE img_fight_map;
	loadimage(&img_fight_map, _T("资源文件\\maps\\fight_map.png"));
	putimage(0, 0, &img_fight_map);
	//敌人图片
	load_pokemon_picture(enemy_pm, 520, 130);
	
	setfillcolor(RGB(241, 145, 73));
	solidroundrect(0, WINDOWS_HIGH * 3 / 4, WINDOWS_WIDTH, WINDOWS_HIGH, 10, 10);
	setfillcolor(WHITE);
	solidroundrect(20, WINDOWS_HIGH * 3 / 4 + 5, WINDOWS_WIDTH - 20, WINDOWS_HIGH - 5, 10, 10);
	//startup_font();
	outtextxy(50, WINDOWS_HIGH * 3 / 4 + 30, _T("野生的敌人出现！！！"));
	FlushBatchDraw();
	next_step();
	
	while (1)
	{
		//战斗开始界面绘画封住函数
		fight_interface(own_pm, &own_now_bleed, own_bleed_width, 
			enemy_pm, &enemy_now_bleed, enemy_bleed_width);
		//生命值是否为0
		if (own_now_bleed <= 0)
		{
			outtextxy(50, WINDOWS_HIGH * 3 / 4 + 30, _T("you loss!"));
			break;
		}
		if (enemy_now_bleed <= 0)
		{
			outtextxy(50, WINDOWS_HIGH * 3 / 4 + 30, _T("winner"));
			break;
		}
		//准备界面
		if (fight_choose == 0)
		{
			//敌方回合
			if (fight_turn == 0)
			{
				srand(time(NULL));
				int temp = rand() % 4 + 1;
				if (temp == 1)
					pm_attack_pm(enemy_pm, own_pm, enemy_pm->skill_1_name, enemy_pm->skill_1_damage, 
									&fight_choose, &own_now_bleed);
				else if (temp == 2)
					pm_attack_pm(enemy_pm, own_pm, enemy_pm->skill_2_name, enemy_pm->skill_2_damage, 
									&fight_choose, &own_now_bleed);
				else if (temp == 3)
					pm_attack_pm(enemy_pm, own_pm, enemy_pm->skill_3_name, enemy_pm->skill_3_damage, 
									&fight_choose, &own_now_bleed);
				else if (temp == 4)
					pm_attack_pm(enemy_pm, own_pm, enemy_pm->skill_4_name, enemy_pm->skill_4_damage, 
									&fight_choose, &own_now_bleed);
				//释放完技能结束敌方回合
				fight_turn = 1;
				// 清除键盘缓冲区, 防止切换过场动画时候按到其他按键
				FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
			}

			//我方回合
			if (fight_turn == 1)
			{
				//startup_font();
				outtextxy(WINDOWS_WIDTH * 3 / 5 + 50, WINDOWS_HIGH * 3 / 4 + 30, _T("战斗"));
				outtextxy(WINDOWS_WIDTH * 3 / 5 + 200, WINDOWS_HIGH * 3 / 4 + 30, _T("物品"));
				outtextxy(WINDOWS_WIDTH * 3 / 5 + 50, WINDOWS_HIGH * 3 / 4 + 90, _T("宝可梦"));
				outtextxy(WINDOWS_WIDTH * 3 / 5 + 200, WINDOWS_HIGH * 3 / 4 + 90, _T("逃跑"));
				//准备界面选择光标
				setfillcolor(RGB(49, 49, 49));
				POINT pts[] = { { choose_x, choose_y },{ choose_x,choose_y + 20 },
								{ choose_x + 15, choose_y + 10 } };
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
		}
		//技能选择界面
		if (fight_choose == 1)
		{
			TCHAR show_str[4][20];
			TCHAR pm_skill[20];
			CharToTchar(own_pm->skill_1_name, pm_skill);
			lstrcpyW(show_str[0], pm_skill);
			CharToTchar(own_pm->skill_2_name, pm_skill);
			lstrcpyW(show_str[1], pm_skill);
			CharToTchar(own_pm->skill_3_name, pm_skill);
			lstrcpyW(show_str[2], pm_skill);
			CharToTchar(own_pm->skill_4_name, pm_skill);
			lstrcpyW(show_str[3], pm_skill);
			fight_operation_interface(&skill_x, &skill_y, &fight_choose, show_str, own_pm, enemy_pm, 
										&fight_turn, &own_now_bleed, &enemy_now_bleed);
		}

		//物品选择界面
		if (fight_choose == 2)
		{
			TCHAR show_str[4][20];
			lstrcpyW(show_str[0], _T("Potion"));
			lstrcpyW(show_str[1], _T("Super_Potion"));
			lstrcpyW(show_str[2], _T("Hyper_Potion"));
			lstrcpyW(show_str[3], _T("Max_Potion"));
			fight_operation_interface(&good_x, &good_y, &fight_choose, show_str, own_pm, enemy_pm, 
										&fight_turn, &own_now_bleed, &enemy_now_bleed);
		}

		//替换宝可梦选择界面
		if (fight_choose == 3)
		{
			TCHAR show_str[4][20];
			lstrcpyW(show_str[0], _T("Charizard"));
			lstrcpyW(show_str[1], _T("岑樱"));
			lstrcpyW(show_str[2], _T("北洛"));
			lstrcpyW(show_str[3], _T("-"));
			fight_operation_interface(&pokemon_x, &pokemon_y, &fight_choose, show_str, own_pm, enemy_pm, 
										&fight_turn, &own_now_bleed, &enemy_now_bleed);
		}
		FlushBatchDraw();
	}
}



void starup_map_and_player()
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

	//初始化字体
	startup_font();
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
	starup_map_and_player();
	while (1)
	{
		show_map();
		is_fight();
		pokemons_refresh();
		operate();
	}
	gameover();
	return 0;
}

void startup_pokemon()
{
	PM[0].x = 0;
	PM[0].y = 0;
	PM[0].number = 0;
	strcpy(PM[0].name, "Pikachu");
	PM[0].level = 5;
	PM[0].experience = 0;
	PM[0].bleed = 10;
	PM[0].attack = 10;
	PM[0].defence = 10;
	PM[0].judge = 0;
	PM[0].time = 0;
	strcpy(PM[0].skill_1_name, "scream");
	PM[0].skill_1_damage = 50;
	strcpy(PM[0].skill_2_name, "catchit");
	PM[0].skill_2_damage = 200;
	strcpy(PM[0].skill_3_name, "lighting");
	PM[0].skill_3_damage = 40;
	strcpy(PM[0].skill_4_name, "flash");
	PM[0].skill_4_damage = 70;


	PM[1].x = 367;
	PM[1].y = 420;
	PM[1].number = 1;
	strcpy(PM[1].name, "Entei");
	PM[1].level = 5;
	PM[1].experience = 0;
	PM[1].bleed = 100;
	PM[1].attack = 10;
	PM[1].defence = 5;
	PM[1].judge = 1;
	PM[1].time = 0;
	strcpy(PM[1].skill_1_name, "boom");
	PM[1].skill_1_damage = 50;
	strcpy(PM[1].skill_2_name, "boom");
	PM[1].skill_2_damage = 200;
	strcpy(PM[1].skill_3_name, "boom");
	PM[1].skill_3_damage = 40;
	strcpy(PM[1].skill_4_name, "boom");
	PM[1].skill_4_damage = 70;


	PM[2].x = 0;
	PM[2].y = 0;
	PM[2].number = 2;
	strcpy(PM[2].name, "Charizard");
	PM[2].level = 10;
	PM[2].experience = 0;
	PM[2].bleed = 50;
	PM[2].attack = 10;
	PM[2].defence = 10;
	PM[2].judge = 1;
	PM[2].time = 0;
	strcpy(PM[2].skill_1_name, "boom");
	PM[2].skill_1_damage = 50;
	strcpy(PM[2].skill_2_name, "catchit");
	PM[2].skill_2_damage = 100;
	strcpy(PM[2].skill_3_name, "boom");
	PM[2].skill_3_damage = 40;
	strcpy(PM[2].skill_4_name, "boom");
	PM[2].skill_4_damage = 70;


	PM[3].x = 700;
	PM[3].y = 420;
	PM[3].number = 3;
	strcpy(PM[3].name, "Snorlax");
	PM[3].level = 10;
	PM[3].experience = 0;
	PM[3].bleed = 500;
	PM[3].attack = 10;
	PM[3].defence = 10;
	PM[3].judge = 1;
	PM[3].time = 0;
	strcpy(PM[3].skill_1_name, "boom");
	PM[3].skill_1_damage = 50;
	strcpy(PM[3].skill_2_name, "catchit");
	PM[3].skill_2_damage = 100;
	strcpy(PM[3].skill_3_name, "boom");
	PM[3].skill_3_damage = 40;
	strcpy(PM[3].skill_4_name, "boom");
	PM[3].skill_4_damage = 70;
}