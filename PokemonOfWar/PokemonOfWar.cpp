/******************************************
*	英文名称：Pokemon Of War
*	中文名称：战争里的口袋妈妈
*	编译环境：vc2017 + EasyX_20200109(beta)
*	Maker：	  panyu.tan
*	最初版本：2020/2/7
*	最后修改：2020/2/27
*******************************************/

#include <graphics.h>
#include <conio.h>
#include <time.h>
#include <stdlib.h>
#include <stdio.h>
#include "tools.h"
#include "pokemon_skills.h"
#include "music.h"

#pragma comment( lib, "MSIMG32.LIB")	// 引用该库才能使用 TransparentBlt 函数
#pragma comment( lib, "Winmm.lib")		// 使用播放音乐


///////////////宏定义///////////////
#define WINDOWS_WIDTH 800				
#define WINDOWS_HIGH 600
#define PLAYER_WIDTH 32
#define PLAYER_HIGH 50					//这个高度人物的头顶会多出5个像素
#define POKEMON_NUMBER 5
#define NPC_NUMBER 10


///////////////全局变量////////////////
int	  g_canvas[1281][881] = { 0 };		//定义地图画布坐标,使每个像素点坐标为0，0为无障碍，1为有障碍不能通过
IMAGE g_img_city_map;					//1280*880
IMAGE g_img_player_walk;
int	  g_player_picture_i;				//player图片,i为整张人物图的横坐标，j为纵坐标
int   g_player_picture_j;
int   g_player_x;						//player初始位置
int   g_player_y;
int   g_map_x;							//map截取的位置
int   g_map_y;

int g_game_state = 0;					//0为初始菜单界面，1为游戏界面
int g_plot = 0;							//剧情判断
int g_is_win = 0;						//战斗胜利判断，0未loss，1未win

time_t g_time_start, g_time_end;		//计算通关游戏时间


//npc坐标
//右下角女孩
int g_npc_zhang_x;
int g_npc_zhang_y;
//开始右边
int g_npc_green_x;
int g_npc_green_y;
//地穴旁边勇士
int g_npc_ys_x;
int g_npc_ys_y;
//右下角卡卡西
int g_npc_kk_x;
int g_npc_kk_y;

int g_npc_burrow_x;
int g_npc_burrow_y;

int g_npc_sec_office_x;
int g_npc_sec_office_y;

int g_npc_office_x;
int g_npc_office_y;

int g_npc_shop_x;
int g_npc_shop_y;

int g_npc_hospital_x;
int g_npc_hospital_y;

int g_npc_dorm_x;
int g_npc_dorm_y;

//枚举地图类型
enum Map { SCHOOL, DORM, SEC_OFFICE, OFFICE, SHOP, HOSPITAL, BURROW };


//物品
struct good
{
	char name[20];
	int left;
	int add;
};

//npc
struct Npc
{
	int x;
	int y;
	int map;
}NPC[NPC_NUMBER];


//宝可梦技能
struct skill
{
	char name[30];
	int damage;
	int init_pp;
	int left_pp;
}g_s_pm_skills[15];


//宝可梦
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
	int  is_own;						//判断该宝可梦是否被收服，1为被收服
	int  is_change;						//判断该宝可梦在这次战斗中是否被替换过，1为没有被替换过
	struct skill s_skill[4];
}PM[POKEMON_NUMBER];


//游戏玩家
struct player
{
	//携带的物品
	struct good s_good[4];
	//携带的宝可梦
	struct pokemon s_pokemons[4];
}struct_player;





///////////////函数声明//////////////////
void startup_gamegraph();												//初始化游戏窗口和字体
void starup_map_and_player_position();									//初始化地图和人物的位置
void show_map();														//绘画地图
void gameover();
void keyboard_operation();												//在大地图上的键盘操作
void is_fight();														//判断在地图上是否遭遇敌人，然后进入战斗
void show_fight(pokemon *, pokemon *);									//进入战斗
void startup_pokemon();													//初始化宝可梦属性
void show_dialog_box_words(TCHAR*, TCHAR*, TCHAR*);						//对话框
//战斗准备时候的操作
void fight_operation_interface(int *, int *, int *, TCHAR[][20], pokemon *, pokemon *, int *, int *, int *);
void use_skill(char *);													//使用技能
void fight_interface(pokemon *, int *, int, pokemon *, int *, int);		//战斗开始界面绘画封住函数
void pm_attack_pm(pokemon *, pokemon *, char *, int , int *);			//宝可梦攻击
void show_use_skill_words(char *, char *);								//使用技能时显示的文字描述
void show_use_good_words(char *, char *, int, char *);					//使用物品显示的文字描述
void show_change_pokemon_words(char *, char *);							//替换宝可梦显示的文字描述
void show_damage_words(int);											//显示技能造成伤害的文字描述
void startup_font(int, int, COLORREF);									//初始化字体
void load_pokemon_picture(pokemon *, int, int);							//载入宝可梦的图片
void pokemons_refresh();												//刷新宝可梦
void skills_details(int, int, pokemon *);								//选择技能的时候在右边显示技能详细信息
void goods_details(int, int);											//显示物品的详细信息
void pokemons_details(int, int);										//显示宝可梦的详细信息
void start_menu();														//开始界面
void startup_player();													//初始化玩家
void pokemon_operate(pokemon *, int *, pokemon *, int *, int *);		//替换宝可梦操作
void pp_good_operate(pokemon *, struct good *, int *, int *);			//使用pp药剂操作
void bleed_good_operate(pokemon *, int *, struct good *, int *, int *);	//使用回复生命值药剂操作
//使用技能操作
void skill_operate(pokemon *, pokemon *, int *, struct skill *, int *, int *);	
void miss_enemy_words(pokemon *);										//遇到敌人时候，显示对话
void enemy_fight_turn(pokemon *, pokemon *, int *, int *);				//敌人攻击回合
void own_fight_turn(int *, int *, int *, int *);						//我方攻击回合
int  calculate_bleed(int *, pokemon *, int);							//计算血量
void recovery_pp();														//恢复pp
void next_step();														//下一步，按一下确认键
void startup_struct_skill();											//初始化宝可梦技能
void startup_npc();														//初始化npc位置

//读档和存档的函数
void readRecordFile();
void writeRecordFile();


/////////////////////地图///////////////////
void judge_into_map();													//判断是否进入其他地图
//进入地图
void into_map(int, int, int, int, TCHAR * ,int (*)[600], int, int, int, int, TCHAR *, enum Map e_map); 
//进入地图后的键盘操作
void into_map_keyboard_operation(int(*)[600], int *, int *, int, int, int, int, enum Map e_map);
void dorm_map();														//地图详细
void hospital_map();
void office_map();
void shop_map();
void sec_office_map();
void burrow_map();

//////////////剧情和对话////////////
void judge_plot_and_talk(int player_x, int player_y, enum Map e_map);	//判读是否进入剧情或者和npc对话
void beginning_of_plot();												//开始游戏剧情
void end_of_plot();														//游戏结束动画
void plot_1();
void plot_2();
void plot_3();
void plot_4();
void plot_5();
void plot_6();
void plot_7();
void get_thing(TCHAR *);												//显示获得物品

void npc_drom_talk1();													//npc对话内容
void npc_zhang_talk1();
void npc_zhang_talk2();
void npc_burrow_talk1();
void npc_burrow_talk2();
void npc_shop_talk1();
void npc_shop_talk2();
void npc_office_talk1();
void npc_office_talk2();
void npc_sec_office_talk1();
void npc_sec_office_talk2();
void npc_green_takl1();
void npc_green_takl2();
void npc_green_takl3();
void npc_hospital_talk1();
void npc_kk_talk1();
void npc_ys_talk1();






/////////主函数//////////////

int main(void)
{
	startup_gamegraph();
	startup_struct_skill();
	startup_skill_music();
	startup_npc();
	startup_pokemon();
	startup_player();					//player里包含宝可梦，所以需要先初始化宝可梦再初始化player
	starup_map_and_player_position();
	start_menu();						//需要先初始化窗口才能够对背景，字体等操作
	while (1)
	{
		show_map();
		judge_into_map();
		is_fight();
		pokemons_refresh();
		keyboard_operation();
	}
	gameover();
	return 0;
}




void startup_gamegraph()
{
	initgraph(WINDOWS_WIDTH, WINDOWS_HIGH);
	//初始化字体
	startup_font(25, 0, BLACK);
	BeginBatchDraw();
}





void get_thing(TCHAR *w_good_name)
{
	music_get();
	show_dialog_box_words(_T(""), _T("得到了"), w_good_name);
}





void next_step()
{
	FlushBatchDraw();
	//清除键盘缓冲区,防止切换过场动画时候按到其他按键
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	while (1)
	{
		char input = _getch();
		if (input == 'j')
		{
			music_next();
			break;
		}
	}
}






void recovery_pp()
{
	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			struct_player.s_pokemons[i].s_skill[j].left_pp = struct_player.s_pokemons[i].s_skill[j].init_pp;
		}
	}
}






void readRecordFile()
{
	FILE *fp;
	fp = fopen("资源文件\\save\\gamedata.dat", "r");
	if (fp != NULL)
	{
		fscanf(fp, "%d %d %d %d %d %d %d %d %d %d %d %d",
			&g_map_x, &g_map_y, &g_player_x, &g_player_y, &g_player_picture_i, &g_player_picture_j, 
			&g_plot, &g_time_start,
			&struct_player.s_good[0].left, &struct_player.s_good[1].left,
			&struct_player.s_good[2].left, &struct_player.s_good[3].left);
	}
	else
	{
		fclose(fp);
		HWND wnd = GetHWnd();
		MessageBox(wnd, _T("无法找到存档"), _T("警告"), MB_OK | MB_ICONWARNING);
		exit(1);
	}
	fclose(fp);
}





void writeRecordFile()
{
	FILE *fp;
	fp = fopen("资源文件\\save\\gamedata.dat", "w");
	if (fp != NULL)
	{
		fprintf(fp, "%d %d %d %d %d %d %d %d %d %d %d %d",
			g_map_x, g_map_y, g_player_x, g_player_y, g_player_picture_i, g_player_picture_j, 
			g_plot, g_time_start,
			struct_player.s_good[0].left, struct_player.s_good[1].left,
			struct_player.s_good[2].left, struct_player.s_good[3].left);
	}
	else
	{
		fclose(fp);
		HWND wnd = GetHWnd();
		MessageBox(wnd, _T("存档失败"), _T("警告"), MB_OK | MB_ICONWARNING);
		exit(1);
	}
	fclose(fp);
}





void startup_player()
{
	strcpy(struct_player.s_good[0].name, "Potion");
	struct_player.s_good[0].left = 4;
	struct_player.s_good[0].add = 30;

	strcpy(struct_player.s_good[1].name, "Super_Potion");
	struct_player.s_good[1].left = 2;
	struct_player.s_good[1].add = 40;

	strcpy(struct_player.s_good[2].name, "Hyper_Potion");
	struct_player.s_good[2].left = 2;
	struct_player.s_good[2].add = 50;

	strcpy(struct_player.s_good[3].name, "pp补充剂");
	struct_player.s_good[3].left = 1;
	struct_player.s_good[3].add = 20;


	struct_player.s_pokemons[0] = PM[0];
	struct_player.s_pokemons[1] = PM[2];
}





void startup_npc()
{
	//右下角女孩
	g_npc_zhang_x = 707;
	g_npc_zhang_y = 695;
	NPC[0].x = g_npc_zhang_x;
	NPC[0].y = g_npc_zhang_y;
	NPC[0].map = SCHOOL;
	//开始右边
	g_npc_green_x = 430;
	g_npc_green_y = 340;
	NPC[1].x = g_npc_green_x;		
	NPC[1].y = g_npc_green_y;
	NPC[1].map = SCHOOL;
	//地穴旁边勇士
	g_npc_ys_x = 960;
	g_npc_ys_y = 316;
	NPC[2].x = g_npc_ys_x;
	NPC[2].y = g_npc_ys_y;
	NPC[2].map = SCHOOL;
	
	//右下角卡卡西
	g_npc_kk_x = 1070;
	g_npc_kk_y = 595;
	NPC[3].x = g_npc_kk_x;
	NPC[3].y = g_npc_kk_y;
	NPC[3].map = SCHOOL;

	g_npc_burrow_x = 377;
	g_npc_burrow_y = 130;
	NPC[4].x = g_npc_burrow_x;
	NPC[4].y = g_npc_burrow_y;
	NPC[4].map = BURROW;

	g_npc_sec_office_x = 375;
	g_npc_sec_office_y = 190;
	NPC[5].x = g_npc_sec_office_x;
	NPC[5].y = g_npc_sec_office_y;
	NPC[5].map = SEC_OFFICE;

	g_npc_office_x = 572;
	g_npc_office_y = 229;
	NPC[6].x = g_npc_office_x;
	NPC[6].y = g_npc_office_y;
	NPC[6].map = OFFICE;

	g_npc_shop_x = 363;
	g_npc_shop_y = 197;
	NPC[7].x = g_npc_shop_x;
	NPC[7].y = g_npc_shop_y;
	NPC[7].map = SHOP;

	g_npc_hospital_x = 410;
	g_npc_hospital_y = 255;
	NPC[8].x = g_npc_hospital_x;
	NPC[8].y = g_npc_hospital_y;
	NPC[8].map = HOSPITAL;

	g_npc_dorm_x = 254;
	g_npc_dorm_y = 215;
	NPC[9].x = g_npc_dorm_x;
	NPC[9].y = g_npc_dorm_y;
	NPC[9].map = DORM;
}





void npc_drom_talk1()
{
	show_dialog_box_words(_T("小七:"), _T("歪比歪比？歪比巴布"), _T("are u good 马拉西亚？"));
	show_dialog_box_words(_T("小七:"), _T("怎么上交论文吗？"), _T("打印好纸质版，然后去上交给教务秘书就行"));
}


void npc_zhang_talk1()
{
	show_dialog_box_words(_T("小张:"), _T("你找到你的u盘了？"), _T("太好了！"));
}

void npc_zhang_talk2()
{
	show_dialog_box_words(_T("小张:"), _T("今天天气不错"), _T("不是吗？"));
	show_dialog_box_words(_T("小张:"), _T("毕业了去哪里吗"), _T("我想再去一遍海边"));
}


void npc_burrow_talk1()
{
	show_dialog_box_words(_T("失误招领处大爷:"), _T("拿了快滚"), _T("别让我看见你"));
}

void npc_burrow_talk2()
{
	show_dialog_box_words(_T("？？？:"), _T("歪比歪比？歪比巴布"), _T("are u good 马拉西亚？"));
}

void npc_shop_talk1()
{
	show_dialog_box_words(_T("打印店老板:"), _T("好汉饶命，我再也不敢了"), _T("下次请你吃饭"));

}
void npc_shop_talk2()
{
	show_dialog_box_words(_T("打印店老板:"), _T("需要打印吗？"), _T("把你的u盘给我吧"));

}

void npc_office_talk1()
{
	show_dialog_box_words(_T("导员:"), _T("快去见教务秘书吧"), _T("小心点"));
}

void npc_office_talk2()
{
	show_dialog_box_words(_T("导员:"), _T("一个假期不见"), _T("你又长胖了"));
}


void npc_sec_office_talk1()
{
	show_dialog_box_words(_T("教务秘书:"), _T("...算你厉害"), _T("把你的论文给我吧（求饶）"));
	HWND wnd = GetHWnd();
	if (MessageBox(wnd, _T("是否上交论文？ \n上交论文游戏会结束通关"), _T("提示"), MB_YESNO | MB_ICONQUESTION) == IDYES)
		end_of_plot();
}

void npc_sec_office_talk2()
{
	show_dialog_box_words(_T("教务秘书:"), _T("同学你好"), _T("有什么需要帮助的？"));
}


void npc_green_takl1()
{
	show_dialog_box_words(_T("green girl:"), _T("欢迎来到战争里的口袋妈妈"), _T("my master is panyu.tan"));
	show_dialog_box_words(_T("green girl:"), _T("如果你有什么不懂的操作可以和我对话"), _T("也可以来找我聊天"));
	show_dialog_box_words(_T("green girl:"), _T("反正我也帮不了你"), _T("祝你好运！"));
}

void npc_green_takl2()
{
	show_dialog_box_words(_T("green girl:"), _T("什么？你喜欢我"), _T("对不起，我有男朋友了"));
	show_dialog_box_words(_T("green girl:"), _T("但是......"), _T("我不介意多一个"));
}

void npc_green_takl3()
{
	SYSTEMTIME time;
	GetLocalTime(&time);
	TCHAR w_year[50], w_mon[10], w_day[10], w_hour[10], w_min[10];
	wsprintf(w_year, _T("%d"), time.wYear);
	wsprintf(w_mon,  _T("%d"), time.wMonth);
	wsprintf(w_day,  _T("%d"), time.wDay);
	wsprintf(w_hour, _T("%d"), time.wHour);
	wsprintf(w_min,  _T("%d"), time.wMinute); 
	lstrcatW(w_year, _T("年"));
	lstrcatW(w_year, w_mon);
	lstrcatW(w_year, _T("月"));
	lstrcatW(w_year, w_day);
	lstrcatW(w_year, _T("日"));
	lstrcatW(w_year, w_hour);
	lstrcatW(w_year, _T("时"));
	lstrcatW(w_year, w_min);
	lstrcatW(w_year, _T("分"));

	show_dialog_box_words(_T("green girl:"), _T("时间真快,你知道吗"), _T("这个游戏的最初开发时间是2020年2月7日"));
	show_dialog_box_words(_T("green girl:"), _T("而现在是"), w_year);
}


void npc_hospital_talk1()
{
	show_dialog_box_words(_T("医生姐姐:"), _T("这里是医院"), _T("有什么需要帮助？"));
	show_dialog_box_words(_T("医生姐姐:"), _T("......"), _T("需要恢复宝可梦状态？"));
	music_get();
	show_dialog_box_words(_T("医生姐姐:"), _T("恢复了"), _T("全部宝可梦的技能点！"));
	recovery_pp();
}


void npc_kk_talk1()
{
	show_dialog_box_words(_T("卡卡西:"), _T("可恶"), _T("被你发现存档点了"));
	writeRecordFile();
	show_dialog_box_words(_T("卡卡西:"), _T("正在存档，请稍后......"), _T("存档成功！"));
}

void npc_ys_talk1()
{
	show_dialog_box_words(_T("勇士:"), _T("随着剧情的发展，每个人的对话都会有改变"), _T("你和green girl说过话了吗"));
	show_dialog_box_words(_T("勇士:"), _T("听说她经常会说一些奇怪的话"), _T("可怜的孩子..."));
}



void beginning_of_plot()
{
	setbkcolor(BLACK);
	cleardevice();
	show_dialog_box_words(_T(""), _T("2020年，这是一个宠物小精灵和人类和平相处的年代"), _T("人们通过用自己的小精灵与别人对战获得乐趣"));
	show_dialog_box_words(_T(""), _T("话说天下u盘"), _T("分久必合，合久必分"));
	show_dialog_box_words(_T(""), _T("刚到学校，u盘就丢了"), _T("真是麻烦的一天，论文还存在u盘里面"));
	show_dialog_box_words(_T(""), _T("毕业论文今天就要上交纸质版，"), _T("我得赶紧找到u盘，不然不用毕业"));
}



void end_of_plot()
{
	music_end();
	g_time_end = time(NULL);
	TCHAR w_time[30];
	TCHAR w_showtime[50] = _T("通关时间：");
	double time = difftime(g_time_end, g_time_start);
	int t = time / 60;
	wsprintf(w_time, _T("%d"), t);
	lstrcat(w_showtime, w_time);
	lstrcat(w_showtime, _T("分钟"));
	int y;
	for (y = 600; y > -375; y--)
	{
		setbkcolor(BLACK);
		cleardevice();
		startup_font(25, 0, WHITE);
		outtextxy(270, y, w_showtime);
		outtextxy(270, y + 50, _T("英文名称：Pokemon Of War"));
		outtextxy(270, y + 100, _T("中文名称：战争里的口袋妈妈"));
		outtextxy(270, y + 150, _T("图片素材来源：网络"));
		outtextxy(270, y + 200, _T("bgm来源：网络"));
		outtextxy(270, y + 250, _T("游戏设计：panyu.tan"));
		outtextxy(270, y + 300, _T("游戏开发：panyu.tan"));
		outtextxy(330, y + 350, _T("THE END"));
		FlushBatchDraw();
		Sleep(30);
	}
	HWND wnd = GetHWnd();
	MessageBox(wnd, _T("结束游戏"), _T("看完了"), MB_OK | MB_ICONWARNING);
	exit(0);
}



void plot_1()
{
	show_dialog_box_words(_T("小七:"), _T("你保存有论文的u盘丢了？"), _T("坏起来了..."));
	show_dialog_box_words(_T("小七:"), _T("......"), _T("那去校园路上问问别人有没有捡到吧"));
	show_dialog_box_words(_T("小七:"), _T("如果有什么不懂的地方可以回来问我"), _T("加油"));
}


void plot_2()
{
	show_dialog_box_words(_T("小张:"), _T("什么？你的u盘丢了？"), _T("你也太不小心了"));
	show_dialog_box_words(_T("小张:"), _T("你去失误招领处看看"), _T("说不定有人捡到放在那里了"));
	show_dialog_box_words(_T("小张:"), _T("失误招领在哪里吗？"), _T("你上了四年学居然不知道......"));
	show_dialog_box_words(_T("小张:"), _T("其实我也不知道！"), _T("路上小心！"));
}

void plot_3()
{
	show_dialog_box_words(_T("失误招领处大爷:"), _T("u盘？噢，好像今天有人捡到了放在这里"), _T("但是你怎么证明这是你的？"));
	show_dialog_box_words(_T("失误招领处大爷:"), _T("......"), _T("有本事你可以从我手里抢过去"));
}

void plot_4()
{
	show_dialog_box_words(_T("打印店老板:"), _T("小可爱你需要打印论文是吗？"), _T("ok，交给我吧"));
	show_dialog_box_words(_T("打印店老板:"), _T("先交1000RMB"), _T("什么？你没钱你也敢来打印？"));
}

void plot_5()
{
	show_dialog_box_words(_T("教务秘书:"), _T("你谁！交论文？"), _T("叫你导员签字！"));
}

void plot_6()
{
	show_dialog_box_words(_T("导员:"), _T("那个老太婆这么麻烦"), _T("给你通行证"));
}

void plot_7()
{
	show_dialog_box_words(_T("教务秘书:"), _T("“山有木兮木有枝...”"), _T("“在天愿做比翼鸟...”"));
	show_dialog_box_words(_T("教务秘书:"), _T("没看到我正在和我男朋友打电话吗？"), _T("你不用毕业了，收拾一下回家吧"));
	show_dialog_box_words(_T("教务秘书:"), _T("想要交论文？？？"), _T("那我有空的时候怎么不来"));
	show_dialog_box_words(_T("教务秘书:"), _T("还敢顶嘴"), _T("敬酒不喝喝罚酒"));
}




void judge_plot_and_talk(int player_x ,int player_y, enum Map e_map)
{
	for (int i = 0; i < NPC_NUMBER; i++)
	{
		if (NPC[i].map == e_map && abs(player_x - NPC[i].x) <= 30 && abs(player_y - NPC[i].y) <= 30)
		{
			// && NPC[i].map == e_map 如果相同坐标，通过判断传入的地区参数，来判断属于哪个地图来区分对话和剧情
			if (NPC[i].x == g_npc_dorm_x && NPC[i].y == g_npc_dorm_y )
			{
				if (g_plot <= 1)
				{
					g_plot = 1;
					plot_1();
				}
				else
					npc_drom_talk1();
			}

			else if (NPC[i].x == g_npc_zhang_x && NPC[i].y == g_npc_zhang_y)
			{
				if (g_plot >= 1 && g_plot <= 2)
				{
					g_plot = 2;
					plot_2();
				}
				else if (g_plot == 0 || g_plot >= 4)
					npc_zhang_talk2();
				else
					npc_zhang_talk1();
			}

			else if (NPC[i].x == g_npc_burrow_x && NPC[i].y == g_npc_burrow_y)
			{
				if (g_plot == 2)
				{
					plot_3();
					music_fight1();
					show_fight(&struct_player.s_pokemons[0], &PM[1]);	//触发剧情，进入战斗
					if (g_is_win == 1)
					{
						get_thing(_T("u盘"));
						g_plot = 3;
						music_bk2();
					}
					else
						music_bk1();
						
				}
				else if (g_plot >= 3)
					npc_burrow_talk1();
				else
					npc_burrow_talk2();
			}

			else if (NPC[i].x == g_npc_shop_x && NPC[i].y == g_npc_shop_y)
			{
				if (g_plot == 3)
				{
					plot_4();
					music_fight1();
					show_fight(&struct_player.s_pokemons[0], &PM[4]);	//触发剧情，进入战斗
					if (g_is_win == 1)
					{
						get_thing(_T("纸质论文"));
						g_plot = 4;
						music_bk3();
					}	
					else
					{
						music_bk2();
					}
				}
				else if (g_plot >= 4)
					npc_shop_talk1();
				else
					npc_shop_talk2();
			}

			else if (NPC[i].x == g_npc_office_x && NPC[i].y == g_npc_office_y)
			{
				if (g_plot == 5)
				{
					g_plot = 6;
					plot_6();
					get_thing(_T("通行证"));
				}
				else if (g_plot >= 5)
					npc_office_talk1();
				else
					npc_office_talk2();
			}

			else if (NPC[i].x == g_npc_sec_office_x && NPC[i].y == g_npc_sec_office_y)
			{
				if (g_plot >= 4 && g_plot <= 5)
				{
					g_plot = 5;
					plot_5();
				}
				else if (g_plot == 6)
				{
					plot_7();
					music_fight2();
					show_fight(&struct_player.s_pokemons[0], &PM[3]);	//触发剧情，进入战斗
					if (g_is_win == 1)
					{
						g_plot = 7;
						music_bk4();
					}
					else
						music_bk3();
						
					
				}
				else if (g_plot >= 7)
					npc_sec_office_talk1();
				else
					npc_sec_office_talk2();
			}

			else if (NPC[i].x == g_npc_green_x && NPC[i].y == g_npc_green_y)
			{
				if (g_plot == 0)
					npc_green_takl1();
				else if (g_plot == 1)
					npc_green_takl2();
				else if (g_plot > 1)
					npc_green_takl3();
			}

			else if (NPC[i].x == g_npc_hospital_x && NPC[i].y == g_npc_hospital_y)
			{
				npc_hospital_talk1();
			}

			else if (NPC[i].x == g_npc_kk_x && NPC[i].y == g_npc_kk_y)
			{
				npc_kk_talk1();
			}

			else if (NPC[i].x == g_npc_ys_x && NPC[i].y == g_npc_ys_y)
			{
				npc_ys_talk1();
			}
		}
	}
}





//判断是否进门
void judge_into_map()
{
	if (g_map_x + g_player_x >= 315 && g_map_x + g_player_x < 335 
		&& g_map_y + g_player_y < 260 && g_map_y + g_player_y >= 240 
		&& g_player_picture_j == 3)
		dorm_map();

	if (g_map_x + g_player_x >= 600 && g_map_x + g_player_x < 620
		&& g_map_y + g_player_y < 366 && g_map_y + g_player_y >= 346
		&& g_player_picture_j == 3)
		hospital_map();

	if (g_map_x + g_player_x >= 850 && g_map_x + g_player_x < 870
		&& g_map_y + g_player_y < 206 && g_map_y + g_player_y >= 186
		&& g_player_picture_j == 3)
		shop_map();

	if (g_map_x + g_player_x >= 780 && g_map_x + g_player_x < 800
		&& g_map_y + g_player_y < 530 && g_map_y + g_player_y >= 510
		&& g_player_picture_j == 3)
		office_map();

	if (g_map_x + g_player_x >= 395 && g_map_x + g_player_x < 415
		&& g_map_y + g_player_y < 638 && g_map_y + g_player_y >= 618
		&& g_player_picture_j == 3)
		sec_office_map();

	if (g_map_x + g_player_x >= 1170 && g_map_x + g_player_x < 1190
		&& g_map_y + g_player_y < 310 && g_map_y + g_player_y >= 290
		&& g_player_picture_j == 3)
		burrow_map();
}




void burrow_map()
{
	//定义该地图画布坐标,使每个像素点坐标为0，0为无障碍，1为有障碍不能通过
	int(*canvas)[600] = (int(*)[600])malloc(sizeof(int) * 800 * 600);
	for (int i = 0; i < 800; i++)
	{
		for (int j = 0; j < 600; j++)
		{
			if ((i < 360 || i >= 440) && (j >= 206 && j < 465))
				canvas[i][j] = 1;	//两边
			else if ((i < 335 || i >= 465) && j < 206)
				canvas[i][j] = 1;	//平台
			else
				canvas[i][j] = 0;
		}
	}
	into_map(385, 495, 365, 496, _T("资源文件\\maps\\burrow.png"),
			canvas, 546, 269, 568, 80, _T("资源文件\\npc\\npc7.png"), BURROW);
	free(canvas);
}




void sec_office_map()
{
	//定义该地图画布坐标,使每个像素点坐标为0，0为无障碍，1为有障碍不能通过
	int(*canvas)[600] = (int(*)[600])malloc(sizeof(int) * 800 * 600);
	for (int i = 0; i < 800; i++)
	{
		for (int j = 0; j < 600; j++)
		{
			if (((i >= 306 && i < 346) || (i >= 455 && i < 495)) && j >= 424 && j < 475)
				canvas[i][j] = 1;	//柱子
			else
				canvas[i][j] = 0;
		}
	}
	into_map(385, 509, 365, 510, _T("资源文件\\maps\\sec_office.png"),
			canvas, 560, 195, 605, 85, _T("资源文件\\npc\\npc3.png"), SEC_OFFICE);
	free(canvas);
}





void office_map()
{
	//定义该地图画布坐标,使每个像素点坐标为0，0为无障碍，1为有障碍不能通过
	int(*canvas)[600] = (int(*)[600])malloc(sizeof(int) * 800 * 600);
	for (int i = 0; i < 800; i++)
	{
		for (int j = 0; j < 600; j++)
		{
			if (i >= 448 && i < 511 && j >= 302 && j < 358)
				canvas[i][j] = 1;	//桌子
			else if ((i >= 524 || i < 352) && j < 208)
				canvas[i][j] = 1;	//最上面家具
			else if (((i >= 650 || i < 267) && j >= 432))
				canvas[i][j] = 1;	//下面的两棵树
			else
				canvas[i][j] = 0;
		}
	}
	into_map(395, 409, 370, 410, _T("资源文件\\maps\\office.png"), 
			canvas, 460, 230, 695, 185, _T("资源文件\\npc\\npc8.png"), OFFICE);
	free(canvas);
}





void shop_map()
{
	int(*canvas)[600] = (int(*)[600])malloc(sizeof(int) * 800 * 600);
	for (int i = 0; i < 800; i++)
	{
		for (int j = 0; j < 600; j++)
		{
			if (i >= 492 && i < 566 && j >= 307 && j < 390)
				canvas[i][j] = 1;	//右边第一个商品摆放的地方
			else if (i >= 663 && j >= 265 && j < 390)
				canvas[i][j] = 1;	//右边第二个商品摆放的地方
			else if (i >= 492 && i < 652 && j < 200)
				canvas[i][j] = 1;	//上面摆放处
			else if (i < 350 && j < 300)
				canvas[i][j] = 1;	//柜台
			else
				canvas[i][j] = 0;
		}
	}
	into_map(385, 415, 360, 416, _T("资源文件\\maps\\shop.png"), 
			canvas, 466, 229, 703, 175, _T("资源文件\\npc\\npc5.png"), SHOP);
	free(canvas);
}




void hospital_map()
{
	//定义该地图画布坐标,使每个像素点坐标为0，0为无障碍，1为有障碍不能通过
	int(*canvas)[600] = (int(*)[600])malloc(sizeof(int) * 800 * 600);
	for (int i = 0; i < 800; i++)
	{
		for (int j = 0; j < 600; j++)
		{
			if (i >= 291 && i < 511 && j < 230)
				canvas[i][j] = 1;	//柜台
			else if (i >= 556 && i < 622 && j >= 365 && j < 405)
				canvas[i][j] = 1;	//右下角桌子
			else
				canvas[i][j] = 0;
		}
	}
	into_map(395, 418, 370, 419, _T("资源文件\\maps\\hospital.png"), 
			canvas, 469, 139, 665, 184, _T("资源文件\\npc\\npc1.png"), HOSPITAL);
	free(canvas);
}




void dorm_map()
{
	int(*canvas)[600] = (int(*)[600])malloc(sizeof(int) * 800 * 600);
	for (int i = 0; i < 800; i++)
	{
		for (int j = 0; j < 600; j++)
		{
			if (i < 379 && j < 180)
				canvas[i][j] = 1;		//左上方柜子
			else if (i>= 379 && j < 160)
				canvas[i][j] = 1;		//窗户
			else if (i >= 546 && j < 200)
				canvas[i][j] = 1;		//右上角盆栽
			else if (i < 389 && i >= 327 && j < 320 && j >= 285)
				canvas[i][j] = 1;		//中间桌子
			else if(i>=590 && j>=440)
				canvas[i][j] = 1;		//右下角的树木
			else
				canvas[i][j] = 0;
		}
	}
	into_map(370, 409, 355, 410, _T("资源文件\\maps\\dorm.png"), 
			canvas, 460, 245, 624, 131, _T("资源文件\\npc\\npc6.png"), DORM);
	free(canvas);
}





void into_map_keyboard_operation(int(*canvas)[600], int *player_x, int *player_y, int down_border,int left_border,
						int right_border, int top_border, enum Map e_map)
{
	int step = 10;
	char input;
	if (_kbhit())
	{
		input = _getch();
		g_player_picture_i++;

		if (input == 's')
		{
			g_player_picture_j = 0;
			if (canvas[*player_x][*player_y + PLAYER_HIGH + step] == 0
				&& canvas[*player_x + PLAYER_WIDTH][*player_y + PLAYER_HIGH + step] == 0
				&& *player_y + PLAYER_HIGH < down_border)
			{
				*player_y += step;
			}
		}
		if (input == 'a')
		{
			g_player_picture_j = 1;
			if (canvas[*player_x - step][*player_y] == 0
				&& canvas[*player_x - step][*player_y + PLAYER_HIGH] == 0
				&& *player_x - step >= left_border)
			{
				*player_x -= step;
			}

		}
		if (input == 'd')
		{
			g_player_picture_j = 2;
			if (canvas[*player_x + PLAYER_WIDTH + step][*player_y] == 0
				&& canvas[*player_x + PLAYER_WIDTH + step][*player_y + PLAYER_HIGH] == 0
				&& *player_x + PLAYER_WIDTH + step < right_border)
			{
				*player_x += step;
			}
		}
		if (input == 'w')
		{
			g_player_picture_j = 3;
			if (canvas[*player_x][*player_y - step] == 0
				&& canvas[*player_x + PLAYER_WIDTH][*player_y - step] == 0
				&& *player_y - step >= top_border)
			{
				*player_y -= step;
			}
		}
		if (input == 'j')
		{
			g_player_picture_i--;
			judge_plot_and_talk(*player_x, *player_y, e_map);
		}
		if (g_player_picture_i == 4)
			g_player_picture_i = 0;
	}
}






//地图没有加黑色背景时高340
void into_map(int p_x, int p_y, int out_x, int out_y, TCHAR *map_path, int(*canvas)[600], int down_border, 
				int left_border, int right_border, int top_border ,  TCHAR *npc_path, enum Map e_map)
{
	music_open_door();
	setbkcolor(BLACK);
	cleardevice();

	IMAGE img_npc;
	loadimage(&img_npc, npc_path);
	
	IMAGE img_into_map;
	loadimage(&img_into_map, map_path);
	int player_x = p_x;
	int player_y = p_y;
	g_player_picture_i = 0;
	g_player_picture_j = 3;
	while (1)
	{
		//地图
		putimage(0, 0, &img_into_map);
		//npc
		for (int i = 0; i < NPC_NUMBER; i++)
		{
			if (NPC[i].map == e_map)
			{
				transparentimage(NULL, NPC[i].x, NPC[i].y, &img_npc, RGB(255, 0, 255));
			}
		}
		//player
		HDC dstDC = GetImageHDC(NULL);
		HDC srcDC = GetImageHDC(&g_img_player_walk);
		int srcX = g_player_picture_i * PLAYER_WIDTH;
		int srcY = g_player_picture_j * PLAYER_HIGH;
		TransparentBlt(dstDC, player_x, player_y, PLAYER_WIDTH, PLAYER_HIGH,
			srcDC, srcX, srcY, PLAYER_WIDTH, PLAYER_HIGH, RGB(29, 248, 6));
		FlushBatchDraw();

		into_map_keyboard_operation(canvas, &player_x, &player_y, down_border, left_border, right_border, top_border, e_map);

		if (player_x > out_x && player_x < out_x + 50 && player_y >= out_y && g_player_picture_j == 0)
		{
			music_open_door();
			player_y += 10;	//防止出门然后坐标合适又进门
			break;
		}
	}
}




void start_menu()
{
	g_time_start = time(NULL);
	music_start();
	int y = 205;
	while (g_game_state == 0)
	{
		//在设置背景色之后，并不会改变现有背景色，而是只改变背景色的值，
		//之后再执行绘图语句，例如 outtextxy，会使用新设置的背景色值。
		setbkcolor(WHITE);		
		//如果需要修改全部背景色，可以在设置背景色后执行 cleardevice() 函数
		cleardevice();	

		startup_font(50, 30, RED);
		outtextxy(200, 100, _T("Pokemon Of War"));
		startup_font(25, 0, BLACK);
		outtextxy(340, 200, _T("开始游戏"));
		outtextxy(340, 250, _T("读取存档"));
		outtextxy(340, 300, _T("游戏帮助"));
		outtextxy(340, 350, _T("退出游戏"));

		setfillcolor(RGB(49, 49, 49));
		POINT pts[] = { {320, y},{320, y + 16},{330, y + 8} };
		solidpolygon(pts, 3);
		FlushBatchDraw();
		
		char input;
		if (_kbhit())
		{
			input = _getch();
			if (input == 'w' && y - 50 >= 205)
			{
				music_choose();
				y -= 50;
			}
			if (input == 's' && y + 50 <= 355)
			{
				music_choose();
				y += 50;
			}
			if (input == 'j')
			{
				music_next();
				if (y == 205)
				{
					g_game_state = 1;
					beginning_of_plot();
					music_bk1();
				}
				if (y == 255)
				{
					readRecordFile();
					g_game_state = 1;
					music_bk1();
				}
				if (y == 305)
				{
					show_dialog_box_words(_T("游戏助手："), _T("J为确认，K为返回"), _T("W A S D 控制方向"));
				}
				//1.exit(1)表示异常退出, 在退出前可以给出一些提示信息或在调试程序中察看出错原因。
				//2.exit(0)表示正常退出。
				if (y == 355)	exit(0);
			}
		}
	}
}





void startup_font(int fheight, int fwidth, COLORREF textcolor)
{
	LOGFONT f;
	gettextstyle(&f);
	wcscpy_s(f.lfFaceName, _T("黑体"));
	f.lfHeight = fheight;
	f.lfWidth = fwidth;						//自动适应
	f.lfQuality = ANTIALIASED_QUALITY;		//抗锯齿
	settextstyle(&f);
	setbkmode(TRANSPARENT);					//设置文字背景透明			
	settextcolor(textcolor);
}





void show_use_skill_words(char *pm_name, char *skill_name)
{
	setfillcolor(RGB(241, 145, 73));
	solidroundrect(0, WINDOWS_HIGH * 3 / 4, WINDOWS_WIDTH, WINDOWS_HIGH, 10, 10);
	setfillcolor(WHITE);
	solidroundrect(20, WINDOWS_HIGH * 3 / 4 + 5, WINDOWS_WIDTH - 20, WINDOWS_HIGH - 5, 10, 10);
	
	setfillcolor(RGB(49, 49, 49));
	POINT pts[] = { { 50,  WINDOWS_HIGH - 20 },{ 70,WINDOWS_HIGH - 20 },{ 50 + 10,  WINDOWS_HIGH - 10} };
	solidpolygon(pts, 3);

	char do_str[20] = "使出了";
	char name[30];
	strcpy(name, pm_name);
	strcat(name, do_str);
	TCHAR show_str1[30];
	CharToTchar(name, show_str1);
	outtextxy(50, WINDOWS_HIGH * 3 / 4 + 30, show_str1);
		
	char temp_str[20];
	strcpy(temp_str, skill_name);
	char end_str[2] = "!";
	strcat(temp_str, end_str);
	TCHAR show_str2[20];
	CharToTchar(temp_str, show_str2);
	outtextxy(50, WINDOWS_HIGH * 3 / 4 + 60, show_str2);
	next_step();
}




void show_damage_words(int damage)
{
	setfillcolor(RGB(241, 145, 73));
	solidroundrect(0, WINDOWS_HIGH * 3 / 4, WINDOWS_WIDTH, WINDOWS_HIGH, 10, 10);
	setfillcolor(WHITE);
	solidroundrect(20, WINDOWS_HIGH * 3 / 4 + 5, WINDOWS_WIDTH - 20, WINDOWS_HIGH - 5, 10, 10);

	setfillcolor(RGB(49, 49, 49));
	POINT pts[] = { { 50,  WINDOWS_HIGH - 20 },{ 70,WINDOWS_HIGH - 20 },{ 50 + 10,  WINDOWS_HIGH - 10} };
	solidpolygon(pts, 3);

	char temp_str[30] = "对方受到了";
	char damage_str[10];
	_itoa(damage, damage_str, 10);
	strcat(temp_str, damage_str);
	char end_str[20] = "点伤害！";
	strcat(temp_str, end_str);
	TCHAR show_str[30];
	CharToTchar(temp_str, show_str);
	outtextxy(50, WINDOWS_HIGH * 3 / 4 + 30, show_str);
	FlushBatchDraw();
	//这里是技能显示完后因为要显示造成多少伤害的描述，所以技能sleep需要修改减少1000ms?
	Sleep(1000);
}



void show_use_good_words(char *pokemon_name, char *good_name, int add, char *words)
{
	setfillcolor(RGB(241, 145, 73));
	solidroundrect(0, WINDOWS_HIGH * 3 / 4, WINDOWS_WIDTH, WINDOWS_HIGH, 10, 10);
	setfillcolor(WHITE);
	solidroundrect(20, WINDOWS_HIGH * 3 / 4 + 5, WINDOWS_WIDTH - 20, WINDOWS_HIGH - 5, 10, 10);

	setfillcolor(RGB(49, 49, 49));
	POINT pts[] = { { 50,  WINDOWS_HIGH - 20 },{ 70,WINDOWS_HIGH - 20 },{ 50 + 10,  WINDOWS_HIGH - 10} };
	solidpolygon(pts, 3);

	char do_str[20] = "使用了";
	strcat(do_str, good_name);
	TCHAR show_str1[20];
	CharToTchar(do_str, show_str1);
	outtextxy(50, WINDOWS_HIGH * 3 / 4 + 30, show_str1);

	char name[30];
	strcpy(name, pokemon_name);
	char mid[20] = "回复了";
	strcat(name, mid);
	char add_str[10];
	_itoa(add, add_str, 10);
	strcat(name, add_str);
	strcat(name, words);
	TCHAR show_str2[30];
	CharToTchar(name, show_str2);
	outtextxy(50, WINDOWS_HIGH * 3 / 4 + 60, show_str2);
	next_step();
}






void show_change_pokemon_words(char *befor_pokemon_name, char *after_pokemon_name)
{
	setfillcolor(RGB(241, 145, 73));
	solidroundrect(0, WINDOWS_HIGH * 3 / 4, WINDOWS_WIDTH, WINDOWS_HIGH, 10, 10);
	setfillcolor(WHITE);
	solidroundrect(20, WINDOWS_HIGH * 3 / 4 + 5, WINDOWS_WIDTH - 20, WINDOWS_HIGH - 5, 10, 10);

	setfillcolor(RGB(49, 49, 49));
	POINT pts[] = { { 50,  WINDOWS_HIGH - 20 },{ 70,WINDOWS_HIGH - 20 },{ 50 + 10,  WINDOWS_HIGH - 10} };
	solidpolygon(pts, 3);

	char c_re_str[50] = "替换了宝可梦...回来吧";
	char bef_pm[20];
	strcpy(bef_pm, befor_pokemon_name);
	strcat(c_re_str, bef_pm);
	TCHAR w_re_str[50];
	CharToTchar(c_re_str, w_re_str);
	outtextxy(50, WINDOWS_HIGH * 3 / 4 + 30, w_re_str);
	next_step();
	char c_str[20] = "就是你了";
	char aft_pm[20];
	strcpy(aft_pm, after_pokemon_name);
	strcat(c_str, aft_pm);
	TCHAR w_str[20];
	CharToTchar(c_str, w_str);
	outtextxy(50, WINDOWS_HIGH * 3 / 4 + 60, w_str);
	next_step();
}






void show_dialog_box_words(TCHAR *name, TCHAR *say1, TCHAR *say2)
{
	setfillcolor(RGB(19, 181, 177));
	solidroundrect(0, WINDOWS_HIGH * 3 / 4, WINDOWS_WIDTH, WINDOWS_HIGH, 10, 10);
	setfillcolor(WHITE);
	solidroundrect(20, WINDOWS_HIGH * 3 / 4 + 5, WINDOWS_WIDTH - 20, WINDOWS_HIGH - 5, 10, 10);

	setfillcolor(RGB(49, 49, 49));
	POINT pts[] = { { 50,  WINDOWS_HIGH - 20 },{ 70,WINDOWS_HIGH - 20 },{ 50 + 10,  WINDOWS_HIGH - 10} };
	solidpolygon(pts, 3);

	//startup_font();
	outtextxy(50, WINDOWS_HIGH * 3 / 4 + 15, name);
	outtextxy(50, WINDOWS_HIGH * 3 / 4 + 50, say1);
	next_step();
	outtextxy(50, WINDOWS_HIGH * 3 / 4 + 80, say2);
	next_step();
}








void pokemons_refresh()
{
	for (int i = 0; i < POKEMON_NUMBER; i++)
	{
		if (PM[i].judge == 0 && PM[i].time < 500)
		{
			PM[i].time++;
		}
		if (PM[i].judge == 0 && PM[i].time == 500)
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
			//拷贝struct_player.s_pokemons[0] 到 own_pm，而不是用own_pm指向
			//pokemon own_pm = struct_player.s_pokemons[0];	
			show_fight(&struct_player.s_pokemons[0], &PM[i]);
		}
	}
	
}



void pm_attack_pm(pokemon *pmA, pokemon *pmD, char *pmA_skill_name, 
					int pmA_skill_damage, int *now_bleed)
{
	//基础伤害＝(((攻击方等级×2÷5＋2)×技能威力×攻击力÷敌方防御力)÷50)＋2
	int damage = (((pmA->level * 2 / 5 + 2) * pmA_skill_damage * pmA->attack / pmD->defence) / 50) + 2;
	show_use_skill_words(pmA->name, pmA_skill_name);	//xxx使出了xx
	use_skill(pmA_skill_name);
	show_damage_words(damage);	//xx受到了xx
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
	if (strcmp(skill_name, "eat") == 0)				eat();
	if (strcmp(skill_name, "wind") == 0)			wind();
	if (strcmp(skill_name, "musicattack") == 0)		musicattack();
	if (strcmp(skill_name, "lighton") == 0)			lighton();
	if (strcmp(skill_name, "star") == 0)			star();
	if (strcmp(skill_name, "windattack") == 0)		windattack();
}




void skills_details(int x, int y, pokemon *pm)
{
	outtextxy(WINDOWS_WIDTH * 3 / 5 + 40, WINDOWS_HIGH * 3 / 4 + 30, _T("PP:"));
	outtextxy(WINDOWS_WIDTH * 3 / 5 + 40, WINDOWS_HIGH * 3 / 4 + 80, _T("技能强度:"));
	TCHAR damage[10];
	TCHAR w_left_pp[10];
	TCHAR w_init_pp[10];
	TCHAR str1[2] = _T("/");
	if (x == 60 && y == WINDOWS_HIGH * 3 / 4 + 30)
	{
		wsprintf(damage, _T("%d"), pm->s_skill[0].damage);
		wsprintf(w_left_pp, _T("%d"), pm->s_skill[0].left_pp);
		wsprintf(w_init_pp, _T("%d"), pm->s_skill[0].init_pp);
		lstrcatW(w_left_pp, str1);
		lstrcatW(w_left_pp, w_init_pp);
	}
	if (x == 260 && y == WINDOWS_HIGH * 3 / 4 + 30)
	{
		wsprintf(damage, _T("%d"), pm->s_skill[1].damage);
		wsprintf(w_left_pp, _T("%d"), pm->s_skill[1].left_pp);
		wsprintf(w_init_pp, _T("%d"), pm->s_skill[1].init_pp);
		lstrcatW(w_left_pp, str1);
		lstrcatW(w_left_pp, w_init_pp);
	}
	if (x == 60 && y == WINDOWS_HIGH * 3 / 4 + 90)
	{
		wsprintf(damage, _T("%d"), pm->s_skill[2].damage);
		wsprintf(w_left_pp, _T("%d"), pm->s_skill[2].left_pp);
		wsprintf(w_init_pp, _T("%d"), pm->s_skill[2].init_pp);
		lstrcatW(w_left_pp, str1);
		lstrcatW(w_left_pp, w_init_pp);
	}
	if (x == 260 && y == WINDOWS_HIGH * 3 / 4 + 90)
	{
		wsprintf(damage, _T("%d"), pm->s_skill[3].damage);
		wsprintf(w_left_pp, _T("%d"), pm->s_skill[3].left_pp);
		wsprintf(w_init_pp, _T("%d"), pm->s_skill[3].init_pp);
		lstrcatW(w_left_pp, str1);
		lstrcatW(w_left_pp, w_init_pp);
	}
	outtextxy(WINDOWS_WIDTH * 3 / 5 + 200, WINDOWS_HIGH * 3 / 4 + 30, w_left_pp);
	outtextxy(WINDOWS_WIDTH * 3 / 5 + 200, WINDOWS_HIGH * 3 / 4 + 80, damage);
}





void goods_details(int x, int y)
{
	outtextxy(WINDOWS_WIDTH * 3 / 5 + 40, WINDOWS_HIGH * 3 / 4 + 30, _T("剩余:"));
	outtextxy(WINDOWS_WIDTH * 3 / 5 + 40, WINDOWS_HIGH * 3 / 4 + 80, _T("回复量:"));
	TCHAR w_add[10];
	TCHAR w_left[10];
	if (x == 60 && y == WINDOWS_HIGH * 3 / 4 + 30)
	{
		wsprintf(w_left, _T("%d"), struct_player.s_good[0].left);
		wsprintf(w_add, _T("%d"), struct_player.s_good[0].add);
	}
	if (x == 260 && y == WINDOWS_HIGH * 3 / 4 + 30)
	{
		wsprintf(w_left, _T("%d"), struct_player.s_good[1].left);
		wsprintf(w_add, _T("%d"), struct_player.s_good[1].add);
	}
	if (x == 60 && y == WINDOWS_HIGH * 3 / 4 + 90)
	{
		wsprintf(w_left, _T("%d"), struct_player.s_good[2].left);
		wsprintf(w_add, _T("%d"), struct_player.s_good[2].add);
	}
	if (x == 260 && y == WINDOWS_HIGH * 3 / 4 + 90)
	{
		wsprintf(w_left, _T("%d"), struct_player.s_good[3].left);
		wsprintf(w_add, _T("%d"), struct_player.s_good[3].add);
	}
	outtextxy(WINDOWS_WIDTH * 3 / 5 + 200, WINDOWS_HIGH * 3 / 4 + 30, w_left);
	outtextxy(WINDOWS_WIDTH * 3 / 5 + 200, WINDOWS_HIGH * 3 / 4 + 80, w_add);
}




void pokemons_details(int x, int y)
{
	outtextxy(WINDOWS_WIDTH * 3 / 5 + 40, WINDOWS_HIGH * 3 / 4 + 30, _T("生命值:"));
	outtextxy(WINDOWS_WIDTH * 3 / 5 + 40, WINDOWS_HIGH * 3 / 4 + 80, _T("出战状态:"));
	TCHAR w_bleed[10];
	TCHAR w_fight[15];
	if (x == 60 && y == WINDOWS_HIGH * 3 / 4 + 30)
	{
		if (struct_player.s_pokemons[0].is_own == 1)
		{
			wsprintf(w_bleed, _T("%d"), struct_player.s_pokemons[0].bleed);

			if (struct_player.s_pokemons[0].is_change == 1)
				lstrcpyW(w_fight, _T("可出战"));
			else
				lstrcpyW(w_fight, _T("不可出战"));
		}

		else
		{
			lstrcpyW(w_bleed, _T("-"));
			lstrcpyW(w_fight, _T("-"));
		}
	}

	if (x == 260 && y == WINDOWS_HIGH * 3 / 4 + 30)
	{
		if (struct_player.s_pokemons[1].is_own == 1)
		{
			wsprintf(w_bleed, _T("%d"), struct_player.s_pokemons[1].bleed);

			if (struct_player.s_pokemons[1].is_change == 1)
				lstrcpyW(w_fight, _T("可出战"));
			else
				lstrcpyW(w_fight, _T("不可出战"));
		}

		else
		{
			lstrcpyW(w_bleed, _T("-"));
			lstrcpyW(w_fight, _T("-"));
		}
	}

	if (x == 60 && y == WINDOWS_HIGH * 3 / 4 + 90)
	{
		if (struct_player.s_pokemons[2].is_own == 1)
		{
			wsprintf(w_bleed, _T("%d"), struct_player.s_pokemons[2].bleed);

			if (struct_player.s_pokemons[2].is_change == 1)
				lstrcpyW(w_fight, _T("可出战"));
			else
				lstrcpyW(w_fight, _T("不可出战"));
		}

		else
		{
			lstrcpyW(w_bleed, _T("-"));
			lstrcpyW(w_fight, _T("-"));
		}
	}

	if (x == 260 && y == WINDOWS_HIGH * 3 / 4 + 90)
	{
		if (struct_player.s_pokemons[3].is_own == 1)
		{
			wsprintf(w_bleed, _T("%d"), struct_player.s_pokemons[3].bleed);

			if (struct_player.s_pokemons[3].is_change == 1)
				lstrcpyW(w_fight, _T("可出战"));
			else
				lstrcpyW(w_fight, _T("不可出战"));
		}

		else
		{
			lstrcpyW(w_bleed, _T("-"));
			lstrcpyW(w_fight, _T("-"));
		}
	}
	outtextxy(WINDOWS_WIDTH * 3 / 5 + 200, WINDOWS_HIGH * 3 / 4 + 30, w_bleed);
	outtextxy(WINDOWS_WIDTH * 3 / 5 + 200, WINDOWS_HIGH * 3 / 4 + 80, w_fight);
}





void skill_operate(pokemon *own_pm, pokemon *enemy_pm, int *enemy_now_bleed,
						struct skill *s_skill, int *fight_turn, int *fight_choose)
{
	if (s_skill->left_pp > 0)
	{
		s_skill->left_pp--;
		pm_attack_pm(own_pm, enemy_pm, s_skill->name, s_skill->damage, enemy_now_bleed);
		*fight_turn = 0;
		*fight_choose = 0;
	}
}




void bleed_good_operate(pokemon *own_pm,int *own_now_bleed, struct good *s_good,int *fight_turn, int *fight_choose)
{
	if (s_good->left > 0)
	{
		//使用了xx，xx回复了xx点生命
		show_use_good_words(own_pm->name, s_good->name, s_good->add, "生命值");
		*own_now_bleed += s_good->add;
		s_good->left--;
		*fight_turn = 0;
		*fight_choose = 0;
	}
}



void pp_good_operate(pokemon *own_pm, struct good *s_good, int *fight_turn, int *fight_choose)
{
	if (s_good->left > 0)
	{
		show_use_good_words(own_pm->name, s_good->name, s_good->add, "技能点");
		for (int i = 0; i < 4; i++)
		{
			own_pm->s_skill[i].left_pp += s_good->add;
			if (own_pm->s_skill[i].left_pp > own_pm->s_skill[i].init_pp)
				own_pm->s_skill[i].left_pp = own_pm->s_skill[i].init_pp;
		}
		s_good->left--;
		*fight_turn = 0;
		*fight_choose = 0;
	}
}


void pokemon_operate(pokemon *own_pm, int *own_now_bleed, pokemon *change_pm, int *fight_turn, int *fight_choose)
{
	if (change_pm->is_own == 1 && change_pm->is_change == 1)
	{
		show_change_pokemon_words(own_pm->name, change_pm->name);
		change_pm->is_change = 0;  //交换完宝可梦，该宝可梦的is_change设置为0
		//如果own_pm = change_pm，这个函数结束之后，own_pm != change_pm，因为是局部变量，相当于值引用
		//替换了宝可梦
		*own_now_bleed = change_pm->bleed;
		pokemon pm = *own_pm;
		*own_pm = *change_pm;
		*change_pm = pm;
		//own_pm = change_pm;
		*fight_turn = 0;
		*fight_choose = 0;
	}
}



//战斗，物品，宝可梦操作界面
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
			music_choose();
			*y -= 60;
		}
		if (input == 's' && *y + 60 <= WINDOWS_HIGH * 3 / 4 + 90)
		{
			music_choose();
			*y += 60;
		}
		if (input == 'a' && *x - 200 >= 60)
		{
			music_choose();
			*x -= 200;
		}
		if (input == 'd' && *x + 200 <= 260)
		{
			music_choose();
			*x += 200;
		}
		if (input == 'j')
		{
			music_next();
			if (*x == 60 && *y == WINDOWS_HIGH * 3 / 4 + 30)
			{
				if (*fight_choose == 1)	//判断pp剩余时候才能使用技能 && own_pm->s_skill[0].left_pp > 0
				{
					skill_operate(own_pm, enemy_pm, enemy_now_bleed, &own_pm->s_skill[0], fight_turn, fight_choose);
				}
					
				else if (*fight_choose == 2) //&& struct_player.s_good[0].left > 0
				{
					//使用了xx，xx回复了xx点生命
					bleed_good_operate(own_pm, own_now_bleed, &struct_player.s_good[0], fight_turn, fight_choose);
				}
				else if (*fight_choose == 3)//&& struct_player.s_pokemons[0].is_own == 1 && struct_player.s_pokemons[0].is_change == 1
				{
					pokemon_operate(own_pm, own_now_bleed, &struct_player.s_pokemons[0], fight_turn, fight_choose);
				}
			}

			if (*x == 260 && *y == WINDOWS_HIGH * 3 / 4 + 30)
			{
				if (*fight_choose == 1)// && own_pm->s_skill[1].left_pp > 0
				{
					skill_operate(own_pm, enemy_pm, enemy_now_bleed, &own_pm->s_skill[1], fight_turn, fight_choose);
				}
					
				else if	(*fight_choose == 2)// && struct_player.s_good[1].left > 0
				{
					bleed_good_operate(own_pm, own_now_bleed, &struct_player.s_good[1], fight_turn, fight_choose);
				}
				else if (*fight_choose == 3)//&& struct_player.s_pokemons[1].is_own == 1 && struct_player.s_pokemons[1].is_change == 1
				{
					pokemon_operate(own_pm, own_now_bleed, &struct_player.s_pokemons[1], fight_turn, fight_choose);
				}
			}

			if (*x == 60 && *y == WINDOWS_HIGH * 3 / 4 + 90)
			{
				if (*fight_choose == 1)// && own_pm->s_skill[2].left_pp > 0
				{
					skill_operate(own_pm, enemy_pm, enemy_now_bleed, &own_pm->s_skill[2], fight_turn, fight_choose);
				}
				else if (*fight_choose == 2)// && struct_player.s_good[2].left > 0
				{
					bleed_good_operate(own_pm, own_now_bleed, &struct_player.s_good[2], fight_turn, fight_choose);
				}
				else if (*fight_choose == 3)// && struct_player.s_pokemons[2].is_own == 1 && struct_player.s_pokemons[2].is_change == 1
				{
					pokemon_operate(own_pm, own_now_bleed, &struct_player.s_pokemons[2], fight_turn, fight_choose);
				}
			}

			if (*x == 260 && *y == WINDOWS_HIGH * 3 / 4 + 90)
			{
				if (*fight_choose == 1)//&& own_pm->s_skill[3].left_pp > 0
				{
					skill_operate(own_pm, enemy_pm, enemy_now_bleed, &own_pm->s_skill[3], fight_turn, fight_choose);
				}
				else if (*fight_choose == 2)//&& struct_player.s_good[3].left > 0
				{
					pp_good_operate(own_pm, &struct_player.s_good[3], fight_turn, fight_choose);
				}
				else if (*fight_choose == 3)// && struct_player.s_pokemons[3].is_own == 1 && struct_player.s_pokemons[3].is_change == 1
				{
					pokemon_operate(own_pm, own_now_bleed, &struct_player.s_pokemons[3], fight_turn, fight_choose);
				}
			}
		}
		if (input == 'k')
		{
			music_next();
			*fight_choose = 0;
		}
			
	}
	//当选择光标指向时候显示详细的信息
	if(*fight_choose == 1)
		skills_details(*x, *y, own_pm);
	else if(*fight_choose == 2)
		goods_details(*x, *y);
	else if(*fight_choose == 3)
		pokemons_details(*x, *y);
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



int calculate_bleed(int *now_bleed, pokemon *pm, int bleed_width)
{
	if (*now_bleed >= pm->bleed)
	{
		bleed_width = 220;
		*now_bleed = pm->bleed;
	}
	if (*now_bleed <= 0)
	{
		bleed_width = 0;
		*now_bleed = 0;
	}
	bleed_width = ((double)*now_bleed / pm->bleed) * bleed_width;
	return bleed_width;
}



//战斗开始界面绘画封住函数
void fight_interface(pokemon *own_pm, int *own_now_bleed, int own_bleed_width,
						pokemon *enemy_pm, int *enemy_now_bleed, int enemy_bleed_width)
{
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
	own_bleed_width = calculate_bleed(own_now_bleed, own_pm, own_bleed_width);
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
	enemy_bleed_width = calculate_bleed(enemy_now_bleed, enemy_pm, enemy_bleed_width);
	setfillcolor(RED);
	solidrectangle(145, 145, 365, 155);
	setfillcolor(GREEN);
	solidrectangle(145, 145, 145 + enemy_bleed_width, 155);
}



void miss_enemy_words(pokemon *enemy_pm)
{
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
	outtextxy(50, WINDOWS_HIGH * 3 / 4 + 30, _T("敌人出现！！！"));
	FlushBatchDraw();
	next_step();
}







void enemy_fight_turn(pokemon *enemy_pm, pokemon *own_pm, int *fight_turn, int *own_now_bleed)
{
	srand(time(NULL));
	int temp = rand() % 4 + 1;
	if (temp == 1)
		pm_attack_pm(enemy_pm, own_pm, enemy_pm->s_skill[0].name, enemy_pm->s_skill[0].damage, own_now_bleed);
	else if (temp == 2)
		pm_attack_pm(enemy_pm, own_pm, enemy_pm->s_skill[1].name, enemy_pm->s_skill[1].damage, own_now_bleed);
	else if (temp == 3)
		pm_attack_pm(enemy_pm, own_pm, enemy_pm->s_skill[2].name, enemy_pm->s_skill[2].damage, own_now_bleed);
	else if (temp == 4)
		pm_attack_pm(enemy_pm, own_pm, enemy_pm->s_skill[3].name, enemy_pm->s_skill[3].damage, own_now_bleed);
	//释放完技能结束敌方回合
	*fight_turn = 1;
	// 清除键盘缓冲区, 防止切换过场动画时候按到其他按键
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
}





void own_fight_turn(int *choose_x, int *choose_y, int *fight_choose, int *is_over)
{
	//startup_font();
	outtextxy(WINDOWS_WIDTH * 3 / 5 + 50, WINDOWS_HIGH * 3 / 4 + 30, _T("战斗"));
	outtextxy(WINDOWS_WIDTH * 3 / 5 + 200, WINDOWS_HIGH * 3 / 4 + 30, _T("物品"));
	outtextxy(WINDOWS_WIDTH * 3 / 5 + 50, WINDOWS_HIGH * 3 / 4 + 90, _T("宝可梦"));
	outtextxy(WINDOWS_WIDTH * 3 / 5 + 200, WINDOWS_HIGH * 3 / 4 + 90, _T("逃跑"));
	//准备界面选择光标
	setfillcolor(RGB(49, 49, 49));
	POINT pts[] = { { *choose_x, *choose_y },{ *choose_x, *choose_y + 20 },
					{ *choose_x + 15, *choose_y + 10 } };
	solidpolygon(pts, 3);

	char input;
	if (_kbhit())
	{
		input = _getch();
		if (input == 'w' && *choose_y - 60 >= WINDOWS_HIGH * 3 / 4 + 30)
		{
			music_choose();
			*choose_y -= 60;
		}
			
		if (input == 's' && *choose_y + 60 <= WINDOWS_HIGH * 3 / 4 + 90)
		{
			music_choose();
			*choose_y += 60;
		}
			
		if (input == 'a' && *choose_x - 150 >= WINDOWS_WIDTH * 3 / 5 + 30)
		{
			music_choose();
			*choose_x -= 150;
		}
			
		if (input == 'd' && *choose_x + 150 <= WINDOWS_WIDTH * 3 / 5 + 180)
		{
			music_choose();
			*choose_x += 150;
		}
			
		if (input == 'j')
		{
			music_next();
			if (*choose_x == WINDOWS_WIDTH * 3 / 5 + 30 && *choose_y == WINDOWS_HIGH * 3 / 4 + 30)
				*fight_choose = 1;	//战斗
			if (*choose_x == WINDOWS_WIDTH * 3 / 5 + 180 && *choose_y == WINDOWS_HIGH * 3 / 4 + 30)
				*fight_choose = 2;	//物品
			if (*choose_x == WINDOWS_WIDTH * 3 / 5 + 30 && *choose_y == WINDOWS_HIGH * 3 / 4 + 90)
				*fight_choose = 3;	//替换宝可梦
			if (*choose_x == WINDOWS_WIDTH * 3 / 5 + 180 && *choose_y == WINDOWS_HIGH * 3 / 4 + 90)
			{
				*fight_choose = 4;	//逃跑
				music_defeat();
				*is_over = 1;
			}
		}
	}
}





void show_fight(pokemon *own_pm, pokemon *enemy_pm)
{
	//初始化战斗结果为loss
	g_is_win = 0;
	//初始化我方宝可梦的is_change，保证每回合战斗开始前拥有的宝可梦没有被替换过
	for (int i = 1; i < 4; i++)
	{
		struct_player.s_pokemons[i].is_change = 1;
	}
	struct_player.s_pokemons[0].is_change = 0;//刚出场的宝可梦设置为已经上场

	//战斗切换画面
	interface_change_animatio(WINDOWS_WIDTH, WINDOWS_HIGH);

	//判断战斗是否结束,1为结束
	int is_over = 0;
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
	miss_enemy_words(enemy_pm);
	
	while (is_over != 1)
	{
		//战斗开始界面绘画封住函数
		fight_interface(own_pm, &own_now_bleed, own_bleed_width, 
						enemy_pm, &enemy_now_bleed, enemy_bleed_width);

		//生命值是否为0
		if (own_now_bleed <= 0)
		{
			music_defeat();
			outtextxy(50, WINDOWS_HIGH * 3 / 4 + 30, _T("you loss!"));
			g_is_win = 0;
			FlushBatchDraw();
			Sleep(1000);
			break;
		}
		if (enemy_now_bleed <= 0)
		{
			music_victory();
			outtextxy(50, WINDOWS_HIGH * 3 / 4 + 30, _T("winner"));
			g_is_win = 1;
			FlushBatchDraw();
			Sleep(1000);
			break;
		}

		//准备界面
		if (fight_choose == 0)
		{
			//敌方回合
			if (fight_turn == 0)	enemy_fight_turn(enemy_pm, own_pm, &fight_turn, &own_now_bleed);
			//我方回合
			if (fight_turn == 1)	own_fight_turn(&choose_x, &choose_y, &fight_choose, &is_over);
		}

		//技能选择界面
		if (fight_choose == 1)
		{
			TCHAR show_str[4][20];
			for (int i = 0; i < 4; i++)
				CharToTchar(own_pm->s_skill[i].name, show_str[i]);
			fight_operation_interface(&skill_x, &skill_y, &fight_choose, show_str, own_pm, enemy_pm, 
										&fight_turn, &own_now_bleed, &enemy_now_bleed);
		}

		//物品选择界面
		if (fight_choose == 2)
		{
			TCHAR show_str[4][20];
			for (int i = 0; i < 4; i++)
				CharToTchar(struct_player.s_good[i].name, show_str[i]);
			fight_operation_interface(&good_x, &good_y, &fight_choose, show_str, own_pm, enemy_pm, 
										&fight_turn, &own_now_bleed, &enemy_now_bleed);
		}

		//替换宝可梦选择界面
		if (fight_choose == 3)
		{
			TCHAR show_str[4][20];
			for (int i = 0; i < 4; i++)
			{
				if (struct_player.s_pokemons[i].is_own == 1)
					CharToTchar(struct_player.s_pokemons[i].name, show_str[i]);
				else lstrcpyW(show_str[i], _T("-"));
			}
			fight_operation_interface(&pokemon_x, &pokemon_y, &fight_choose, show_str, own_pm, enemy_pm, 
										&fight_turn, &own_now_bleed, &enemy_now_bleed);
		}

		FlushBatchDraw();
	}
	//战斗结束切换画面
	interface_change_animatio(WINDOWS_WIDTH, WINDOWS_HIGH);
}




void starup_canvas()
{
	for (int i = 0; i < 1281; i++)
	{
		for (int j = 0; j < 881; j++)
		{
			if (i < 140 || j >= 737 || j < 205)							//外围
				g_canvas[i][j] = 1;
			else if ((i >= 140 && i < 278) && (j >= 205 && j < 394))			//左上侧空地
				g_canvas[i][j] = 1;
			else if ((i >= 278 && i < 786) && (j >= 205 && j < 240))			//左上侧黄色小房子
				g_canvas[i][j] = 1;
			else if ((i >= 918 && i < 1070) && (j >= 205 && j < 285))		//右边蓝色房子附近小石块
				g_canvas[i][j] = 1;
			else if ((i >= 1135 && i < 1242) && (j >= 227) && j < 305)		//右边地洞
				g_canvas[i][j] = 1;
			else if ((i >= 458 && i < 711) && (j >= 285) && j < 365)			//中间红色房子
				g_canvas[i][j] = 1;
			else if ((i >= 140 && i < 218) && (j >= 463 && j < 737))			//左下的树木
				g_canvas[i][j] = 1;
			else if ((i >= 284 && i < 508) && (j >= 485 && j < 635))			//左下黄色房子
				g_canvas[i][j] = 1;
			else if ((i >= 743 && i < 927) && (j >= 450 && j < 517))			//右下黄色房子
				g_canvas[i][j] = 1;
			else if (i >= 743 && (j >= 640 && j < 737))						//右下横着的树木
				g_canvas[i][j] = 1;
			else if ((i >= 927 && i < 1070) && (j >= 445 && j < 737))		//右下的树木	
				g_canvas[i][j] = 1;
			else if ((i >= 435 && i < 482) && (j >= 310 && j < 360))		//绿色npc 
				g_canvas[i][j] = 1;
			else if ((i >= 717 && i < 790) && (j >= 691 && j < 750))		//粉色npc 
				g_canvas[i][j] = 1;
			else if ((i >= 970 && i < 1000) && (j >= 260 && j < 320))		//勇士npc 
				g_canvas[i][j] = 1;
			else if ((i >= 1000 && i < 1100) && (j >= 563 && j < 663))		//卡卡西npc 
				g_canvas[i][j] = 1;
		}
	}
}



void starup_map_and_player_position()
{
	loadimage(&g_img_city_map, _T("资源文件\\maps\\school2.png"));
	g_map_x = 0;
	g_map_y = 0;
	starup_canvas();

	loadimage(&g_img_player_walk, _T("资源文件\\player.png"));
	g_player_picture_i = g_player_picture_j = 0;
	g_player_x = 150;
	g_player_y = 395;
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
	FlushBatchDraw();
}




void keyboard_operation()
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
			if (g_canvas[g_map_x + g_player_x][g_map_y + g_player_y + PLAYER_HIGH + step] == 0 
				&& g_canvas[g_map_x + g_player_x + PLAYER_WIDTH][g_map_y + g_player_y + PLAYER_HIGH + step] == 0 
				&& g_map_y + g_player_y + PLAYER_HIGH + step < 880)
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
			if (g_canvas[g_map_x + g_player_x - step][g_map_y + g_player_y] == 0 
				&& g_canvas[g_map_x + g_player_x - step][g_map_y + g_player_y + PLAYER_HIGH] == 0 
				&& g_map_x + g_player_x - step > 0)
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
			if (g_canvas[g_map_x + g_player_x + PLAYER_WIDTH + step][g_map_y + g_player_y] == 0 
				&& g_canvas[g_map_x + g_player_x + PLAYER_WIDTH + step][g_map_y + g_player_y + PLAYER_HIGH] == 0 
				&& g_map_x + g_player_x + PLAYER_WIDTH + step < 1280)
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
			if (g_canvas[g_map_x + g_player_x][g_map_y + g_player_y - step] == 0 
				&& g_canvas[g_map_x + g_player_x + PLAYER_WIDTH][g_map_y + g_player_y - step] == 0 
				&& g_map_y + g_player_y - step > 0)
			{
				if (g_player_y > WINDOWS_HIGH / 2 || g_map_y < 10)
					g_player_y -= step;
				else
					g_map_y -= step;
			}
		}
		if (input == 'j')
		{
			g_player_picture_i--;
			judge_plot_and_talk(g_map_x + g_player_x, g_map_y + g_player_y, SCHOOL);
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





void startup_struct_skill()
{
	strcpy(g_s_pm_skills[0].name, "scream");
	g_s_pm_skills[0].damage = 60;
	g_s_pm_skills[0].init_pp = 5;
	g_s_pm_skills[0].left_pp = 5;

	strcpy(g_s_pm_skills[1].name, "lighting");
	g_s_pm_skills[1].damage = 120;
	g_s_pm_skills[1].init_pp = 2;
	g_s_pm_skills[1].left_pp = 2;

	strcpy(g_s_pm_skills[2].name, "flash");
	g_s_pm_skills[2].damage = 70;
	g_s_pm_skills[2].init_pp = 4;
	g_s_pm_skills[2].left_pp = 4;

	strcpy(g_s_pm_skills[3].name, "thunder");
	g_s_pm_skills[3].damage = 80;
	g_s_pm_skills[3].init_pp = 3;
	g_s_pm_skills[3].left_pp = 3;


	strcpy(g_s_pm_skills[4].name, "catchit");
	g_s_pm_skills[4].damage = 85;
	g_s_pm_skills[4].init_pp = 3;
	g_s_pm_skills[4].left_pp = 3;

	strcpy(g_s_pm_skills[5].name, "eat");
	g_s_pm_skills[5].damage = 70;
	g_s_pm_skills[5].init_pp = 4;
	g_s_pm_skills[5].left_pp = 4;

	strcpy(g_s_pm_skills[6].name, "wind");
	g_s_pm_skills[6].damage = 65;
	g_s_pm_skills[6].init_pp = 4;
	g_s_pm_skills[6].left_pp = 4;


	//敌人技能
	strcpy(g_s_pm_skills[7].name, "boom");
	g_s_pm_skills[7].damage = 60;
	g_s_pm_skills[7].init_pp = 2;
	g_s_pm_skills[7].left_pp = 2;

	strcpy(g_s_pm_skills[8].name, "waterattack");
	g_s_pm_skills[8].damage = 70;
	g_s_pm_skills[8].init_pp = 5;
	g_s_pm_skills[8].left_pp = 5;

	strcpy(g_s_pm_skills[9].name, "sword");
	g_s_pm_skills[9].damage = 80;
	g_s_pm_skills[9].init_pp = 5;
	g_s_pm_skills[9].left_pp = 5;

	strcpy(g_s_pm_skills[10].name, "firehigh");
	g_s_pm_skills[10].damage = 90;
	g_s_pm_skills[10].init_pp = 5;
	g_s_pm_skills[10].left_pp = 5;

	strcpy(g_s_pm_skills[11].name, "musicattack");
	g_s_pm_skills[11].damage = 65;
	g_s_pm_skills[11].init_pp = 5;
	g_s_pm_skills[11].left_pp = 5;

	strcpy(g_s_pm_skills[12].name, "lighton");
	g_s_pm_skills[12].damage = 100;
	g_s_pm_skills[12].init_pp = 5;
	g_s_pm_skills[12].left_pp = 5;

	strcpy(g_s_pm_skills[13].name, "star");
	g_s_pm_skills[13].damage = 85;
	g_s_pm_skills[13].init_pp = 5;
	g_s_pm_skills[13].left_pp = 5;

	strcpy(g_s_pm_skills[14].name, "windattack");
	g_s_pm_skills[14].damage = 75;
	g_s_pm_skills[14].init_pp = 5;
	g_s_pm_skills[14].left_pp = 5;

}




void startup_pokemon()
{
	PM[0].x = 0;
	PM[0].y = 0;
	PM[0].number = 0;
	strcpy(PM[0].name, "Pikachu");
	PM[0].level = 12;
	PM[0].experience = 0;
	PM[0].bleed = 70;
	PM[0].attack = 15;
	PM[0].defence = 15;
	PM[0].judge = 0;
	PM[0].time = 0;
	PM[0].is_own = 1;
	PM[0].is_change = 0;
	PM[0].s_skill[0] = g_s_pm_skills[0];
	PM[0].s_skill[1] = g_s_pm_skills[1];
	PM[0].s_skill[2] = g_s_pm_skills[2];
	PM[0].s_skill[3] = g_s_pm_skills[3];


	//敌方宝可梦
	PM[1].x = 0;  //367
	PM[1].y = 420;
	PM[1].number = 1;
	strcpy(PM[1].name, "Entei");
	PM[1].level = 10;
	PM[1].experience = 0;
	PM[1].bleed = 70;
	PM[1].attack = 10;
	PM[1].defence = 15;
	PM[1].judge = 1;
	PM[1].time = 0;
	PM[1].s_skill[0] = g_s_pm_skills[7];
	PM[1].s_skill[1] = g_s_pm_skills[8];
	PM[1].s_skill[2] = g_s_pm_skills[9];
	PM[1].s_skill[3] = g_s_pm_skills[10];


	//我方宝可梦
	PM[2].x = 0;
	PM[2].y = 0;
	PM[2].number = 2;
	strcpy(PM[2].name, "Charizard");
	PM[2].level = 10;
	PM[2].experience = 0;
	PM[2].bleed = 90;
	PM[2].attack = 20;
	PM[2].defence = 20;
	PM[2].judge = 1;
	PM[2].time = 0;
	PM[2].is_own = 1;
	PM[2].s_skill[0] = g_s_pm_skills[0];
	PM[2].s_skill[1] = g_s_pm_skills[4];
	PM[2].s_skill[2] = g_s_pm_skills[5];
	PM[2].s_skill[3] = g_s_pm_skills[6];



	//敌方宝可梦
	PM[3].x = 0;	//700
	PM[3].y = 420;
	PM[3].number = 3;
	strcpy(PM[3].name, "自来也");
	PM[3].level = 17;
	PM[3].experience = 0;
	PM[3].bleed = 100;
	PM[3].attack = 18;
	PM[3].defence = 13;
	PM[3].judge = 1;
	PM[3].time = 0;
	PM[3].s_skill[0] = g_s_pm_skills[8];
	PM[3].s_skill[1] = g_s_pm_skills[9];
	PM[3].s_skill[2] = g_s_pm_skills[12];
	PM[3].s_skill[3] = g_s_pm_skills[14];




	PM[4].x = 0;
	PM[4].y = 0;
	PM[4].number = 4;
	strcpy(PM[4].name, "Snorlax");
	PM[4].level = 15;
	PM[4].experience = 0;
	PM[4].bleed = 80;
	PM[4].attack = 20;
	PM[4].defence = 20;
	PM[4].judge = 0;
	PM[4].time = 0;
	PM[4].is_own = 0;
	PM[4].is_change = 0;
	PM[4].s_skill[0] = g_s_pm_skills[7];
	PM[4].s_skill[1] = g_s_pm_skills[11];
	PM[4].s_skill[2] = g_s_pm_skills[13];
	PM[4].s_skill[3] = g_s_pm_skills[10];

}


