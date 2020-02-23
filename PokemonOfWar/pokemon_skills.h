//十万伏特，尖叫，电磁波，打雷是皮卡丘的skill目前，直接调用会显示再对方区域，其他则相反


void scream()//尖叫skill,在己方宝可梦前显示
{
	mciSendString(_T("Seek sk2 to start"), NULL, 0, NULL);
	mciSendString(_T("play sk2"), NULL, 0, NULL);

	IMAGE im7, im8;
	loadimage(&im7, _T("资源文件\\skill\\尖叫.jpg"), 70, 220);
	loadimage(&im8, _T("资源文件\\skill\\尖叫bk.jpg"), 70, 220);
	putimage(250, 250, &im8, NOTSRCERASE);
	putimage(250, 250, &im7, SRCINVERT);
	FlushBatchDraw();
	Sleep(500);
}


void lighting()
{
	mciSendString(_T("Seek sk1 to start"), NULL, 0, NULL);
	mciSendString(_T("play sk1"), NULL, 0, NULL);

	IMAGE im3, im4;
	loadimage(&im3, _T("资源文件\\skill\\闪电1.jpg"), 70, 200);
	loadimage(&im4, _T("资源文件\\skill\\闪电1bk.jpg"), 70, 200);
	putimage(550, 0, &im4, NOTSRCERASE);
	putimage(550, 0, &im3, SRCINVERT);
	FlushBatchDraw();
	Sleep(300);
	IMAGE im5, im6;
	loadimage(&im5, _T("资源文件\\skill\\闪电2.jpg"), 70, 200);
	loadimage(&im6, _T("资源文件\\skill\\闪电2bk.jpg"), 70, 200);
	putimage(550, 0, &im6, NOTSRCERASE);
	putimage(550, 0, &im5, SRCINVERT);
	FlushBatchDraw();
	Sleep(300);
	IMAGE im7, im8;
	loadimage(&im7, _T("资源文件\\skill\\闪电3.jpg"), 70, 220);
	loadimage(&im8, _T("资源文件\\skill\\闪电3bk.jpg"), 70, 220);
	putimage(550, -10, &im8, NOTSRCERASE);
	putimage(550, -10, &im7, SRCINVERT);
	FlushBatchDraw();
	Sleep(500);
	// mciSendString(_T("close sk1"), NULL, 0, NULL);
}

void flash()
{
	mciSendString(_T("Seek sk3 to start"), NULL, 0, NULL);
	mciSendString(_T("play sk3"), NULL, 0, NULL);

	IMAGE im7, im8;
	loadimage(&im7, _T("资源文件\\skill\\电磁波.jpg"));
	loadimage(&im8, _T("资源文件\\skill\\电磁波bk.jpg"));
	putimage(500, 70, &im8, NOTSRCERASE);
	putimage(500, 70, &im7, SRCINVERT);
	FlushBatchDraw();
	Sleep(100);
	IMAGE im9, im10;
	loadimage(&im9, _T("资源文件\\skill\\电磁波2.jpg"));
	loadimage(&im10, _T("资源文件\\skill\\电磁波2bk.jpg"));
	putimage(500, 120, &im10, NOTSRCERASE);
	putimage(500, 120, &im9, SRCINVERT);
	FlushBatchDraw();
	Sleep(100);
	IMAGE im11, im12;
	loadimage(&im11, _T("资源文件\\skill\\电磁波3.jpg"));
	loadimage(&im12, _T("资源文件\\skill\\电磁波3bk.jpg"));
	putimage(500, 145, &im12, NOTSRCERASE);
	putimage(500, 145, &im11, SRCINVERT);
	FlushBatchDraw();
	Sleep(100);
	IMAGE im13, im14;
	loadimage(&im13, _T("资源文件\\skill\\电磁波4.jpg"));
	loadimage(&im14, _T("资源文件\\skill\\电磁波4bk.jpg"));
	putimage(500, 175, &im14, NOTSRCERASE);
	putimage(500, 175, &im13, SRCINVERT);
	FlushBatchDraw();
	Sleep(100);
	IMAGE im15, im16;
	loadimage(&im15, _T("资源文件\\skill\\电磁波5.jpg"));
	loadimage(&im16, _T("资源文件\\skill\\电磁波5bk.jpg"));
	putimage(500, 205, &im16, NOTSRCERASE);
	putimage(500, 205, &im15, SRCINVERT);
	FlushBatchDraw();
	Sleep(500);
	//mciSendString(_T("close sk3"), NULL, 0, NULL);
}


//打雷skill的展示
void thunder()
{
	mciSendString(_T("Seek sk4 to start"), NULL, 0, NULL);
	mciSendString(_T("play sk4"), NULL, 0, NULL);

	IMAGE im3, im4;
	loadimage(&im3, _T("资源文件\\skill\\打雷1.jpg"), 70, 200);
	loadimage(&im4, _T("资源文件\\skill\\打雷1bk.jpg"), 70, 200);
	putimage(560, 50, &im4, NOTSRCERASE);
	putimage(560, 50, &im3, SRCINVERT);
	FlushBatchDraw();
	Sleep(400);
	IMAGE im5, im6;
	loadimage(&im5, _T("资源文件\\skill\\打雷2.jpg"), 70, 200);
	loadimage(&im6, _T("资源文件\\skill\\打雷2bk.jpg"), 70, 200);
	putimage(510, 50, &im6, NOTSRCERASE);
	putimage(510, 50, &im5, SRCINVERT);
	FlushBatchDraw();
	Sleep(500);
	//	mciSendString(_T("close sk4"), NULL, 0, NULL);
}


void catchit() //电牢
{
	IMAGE im15, im16;
	loadimage(&im15, _T("资源文件\\skill\\电牢.jpg"));
	loadimage(&im16, _T("资源文件\\skill\\电牢bk.jpg"));
	putimage(500, 120, &im16, NOTSRCERASE);
	putimage(500, 120, &im15, SRCINVERT);
	FlushBatchDraw();
	Sleep(500);
	//mciSendString(_T("close enemy_1"), NULL, 0, NULL);
}


void boom()
{
	IMAGE im9, im10;
	loadimage(&im9, _T("资源文件\\skill\\爆炸.jpg"));
	loadimage(&im10, _T("资源文件\\skill\\爆炸bk.jpg"));
	for (int y = 0; y <= 140; y++)
	{
		putimage(70, 300, 250, y, &im10, 0, 0, NOTSRCERASE);
		putimage(70, 300, 250, y, &im9, 0, 0, SRCINVERT);
		FlushBatchDraw();
		Sleep(10);
	}
}


//水柱
void waterattack()
{
	IMAGE im9, im10;
	loadimage(&im9, _T("资源文件\\skill\\水珠2.jpg"));
	loadimage(&im10, _T("资源文件\\skill\\水珠2bk.jpg"));
	putimage(170, 220, &im10, NOTSRCERASE);
	putimage(170, 220, &im9, SRCINVERT);
	FlushBatchDraw();
	Sleep(200);
	IMAGE im11, im12;
	loadimage(&im11, _T("资源文件\\skill\\水珠3.jpg"));
	loadimage(&im12, _T("资源文件\\skill\\水珠3bk.jpg"));
	putimage(165, 220, &im12, NOTSRCERASE);
	putimage(165, 220, &im11, SRCINVERT);
	FlushBatchDraw();
	Sleep(500);
	//	mciSendString(_T("close enemy_3"), NULL, 0, NULL);
}


//剑雨
void sword()
{
	mciSendString(_T("Seek enemy_1 to start"), NULL, 0, NULL);
	mciSendString(_T("play enemy_1"), NULL, 0, NULL);
	int x = 240;
	int y = 240;
	IMAGE im11;
	for (x = 240, y = 240; x >= 140 && y <= 290; x = x - 2, y = y + 1)
	{
		getimage(&im11, x, y, 180, 99);
		IMAGE im9, im10;
		loadimage(&im9, _T("资源文件\\skill\\万剑.jpg"));
		loadimage(&im10, _T("资源文件\\skill\\万剑bk.jpg"));
		putimage(x, y, &im10, NOTSRCERASE);
		putimage(x, y, &im9, SRCINVERT);
		FlushBatchDraw();
		putimage(x, y, &im11);
	}
	Sleep(500);
	//mciSendString(_T("close enemy_4"), NULL, 0, NULL);
}



//火柱
void firehigh()
{
	IMAGE im9, im10;
	loadimage(&im9, _T("资源文件\\skill\\火柱.jpg"));
	loadimage(&im10, _T("资源文件\\skill\\火柱bk.jpg"));
	putimage(150, 260, &im10, NOTSRCERASE);
	putimage(150, 260, &im9, SRCINVERT);
	FlushBatchDraw();
	Sleep(200);
	putimage(150, 260, &im10, NOTSRCERASE);
	putimage(150, 260, &im9, SRCINVERT);
	FlushBatchDraw();
	putimage(150, 260, &im10, NOTSRCERASE);
	putimage(150, 260, &im9, SRCINVERT);
	Sleep(200);
	FlushBatchDraw();
	putimage(150, 260, &im10, NOTSRCERASE);
	putimage(150, 260, &im9, SRCINVERT);
	Sleep(200);
	FlushBatchDraw();
	Sleep(200);
	//mciSendString(_T("close enemy_5"), NULL, 0, NULL);
}


void eat()
{
	IMAGE im9, im10;
	mciSendString(_T("Seek enemy_7 to start"), NULL, 0, NULL);
	mciSendString(_T("play enemy_7"), NULL, 0, NULL);
	loadimage(&im9, _T("资源文件\\skill\\撕咬.jpg"));
	loadimage(&im10, _T("资源文件\\skill\\撕咬bk.jpg"));
	putimage(500, 100, &im10, NOTSRCERASE);
	putimage(500, 100, &im9, SRCINVERT);
	FlushBatchDraw();
	Sleep(500);
	//mciSendString(_T("close enemy_7"), NULL, 0, NULL);
}


void wind()
{
	IMAGE im9, im10;
	mciSendString(_T("Seek enemy_3 to start"), NULL, 0, NULL);
	mciSendString(_T("play enemy_3"), NULL, 0, NULL);
	loadimage(&im9, _T("资源文件\\skill\\风刃.jpg"));
	loadimage(&im10, _T("资源文件\\skill\\风刃bk.jpg"));
	int y;
	for (y = 250; y > 100; y -= 15)
	{
		putimage(550, y, &im10, NOTSRCERASE);
		putimage(550, y, &im9, SRCINVERT);
		putimage(500, y, &im10, NOTSRCERASE);
		putimage(500, y, &im9, SRCINVERT);
		putimage(600, y, &im10, NOTSRCERASE);
		putimage(600, y, &im9, SRCINVERT);
		FlushBatchDraw();
		Sleep(50);
	}
}