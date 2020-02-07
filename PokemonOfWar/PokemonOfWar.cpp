/******************************************
*	游戏名称：Pokemon Of War
*	编译环境：vc2017 + EasyX_20200109(beta)
*	Maker：	  panyu.tan
*	最初版本：2020/2/7
*	最后修改：2020/2/7
*******************************************/

#include <graphics.h>
#include <conio.h>
#pragma comment( lib, "MSIMG32.LIB")	// 引用该库才能使用 TransparentBlt 函数


#define WINDOWS_WIDTH 800
#define WINDOWS_HIGH 600
#define PLAYER_WIDTH 32
#define PLAYER_HIGH 50					//这个高度人物的头顶会多出5个像素


IMAGE g_city_map;						//1280*880
int canvas[1281][881] = { 0 };			//定义地图画布坐标,使每个像素点坐标为0，0为无障碍，1为有障碍不能通过
IMAGE g_player_walk;
int g_player_picture_i;					//player图片,i为整张人物图的横坐标，j为纵坐标
int g_player_picture_j;
int g_player_x;							//player初始位置
int g_player_y;
int g_map_x;							//map截取的位置
int g_map_y;



void starup_map_player();
void show_map();
void gameover();
void transparentimage(IMAGE *, int, int, int, int, IMAGE *, UINT);	//指定透明色贴图



void transparentimage(IMAGE *dstimg, int x, int y, int w, int h, IMAGE *srcimg, UINT transparentcolor)
{
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int srcX = g_player_picture_i * PLAYER_WIDTH;
	int srcY = g_player_picture_j * PLAYER_HIGH;
	// 使用 Windows GDI 函数实现透明位图
	TransparentBlt(dstDC, x, y, w, h, srcDC, srcX, srcY, w, h, transparentcolor);
}

void starup_map_player()
{
	initgraph(WINDOWS_WIDTH, WINDOWS_HIGH);

	loadimage(&g_city_map, _T("资源文件\\city_map.png"));
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

	loadimage(&g_player_walk, _T("资源文件\\player1.png"));
	g_player_picture_i = g_player_picture_j = 0;
	g_player_x = 150;
	g_player_y = 395;

	BeginBatchDraw();
}

void show_map()
{
	putimage(0, 0, WINDOWS_WIDTH, WINDOWS_HIGH, &g_city_map, g_map_x, g_map_y);
	//putimage(g_player_x, g_player_y, PLAYER_WIDTH, PLAYER_HIGH, &g_player_walk, g_player_picture_i*PLAYER_WIDTH, g_player_picture_j*PLAYER_HIGH);
	transparentimage(NULL, g_player_x, g_player_y, PLAYER_WIDTH, PLAYER_HIGH, &g_player_walk, RGB(29,248,6));
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
	starup_map_player();
	while (1)
	{
		show_map();
		operate();
	}
	gameover();
	return 0;
}