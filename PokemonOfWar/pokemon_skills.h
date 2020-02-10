//十万伏特，尖叫，电磁波，打雷是皮卡丘的skill目前，直接调用会显示再对方区域，其他则相反

void boom()
{
	IMAGE im9, im10;
	loadimage(&im9, _T("资源文件\\skill\\爆炸.jpg"));
	loadimage(&im10, _T("资源文件\\skill\\爆炸bk.jpg"));
	for (int y = 0; y <= 140; y++)
	{
		putimage(30, 200, 250, y, &im10, 0, 0, NOTSRCERASE);
		putimage(30, 200, 250, y, &im9, 0, 0, SRCINVERT);
		FlushBatchDraw();
		Sleep(10);
	}
}
void scream()//尖叫skill
{
	IMAGE im7, im8;
	loadimage(&im7, _T("资源文件\\skill\\尖叫.jpg"), 70, 220);
	loadimage(&im8, _T("资源文件\\skill\\尖叫bk.jpg"), 70, 220);
	putimage(200, 130, &im8, NOTSRCERASE);
	putimage(200, 130, &im7, SRCINVERT);
	FlushBatchDraw();
	Sleep(500);
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

void lighting()
{
	IMAGE im3, im4;
	loadimage(&im3, _T("资源文件\\skill\\闪电1.jpg"), 70, 200);
	loadimage(&im4, _T("资源文件\\skill\\闪电1bk.jpg"), 70, 200);
	putimage(450, 0, &im4, NOTSRCERASE);
	putimage(450, 0, &im3, SRCINVERT);
	FlushBatchDraw();
	Sleep(300);
	IMAGE im5, im6;
	loadimage(&im5, _T("资源文件\\skill\\闪电2.jpg"), 70, 200);
	loadimage(&im6, _T("资源文件\\skill\\闪电2bk.jpg"), 70, 200);
	putimage(450, 0, &im6, NOTSRCERASE);
	putimage(450, 0, &im5, SRCINVERT);
	FlushBatchDraw();
	Sleep(300);
	IMAGE im7, im8;
	loadimage(&im7, _T("资源文件\\skill\\闪电3.jpg"), 70, 220);
	loadimage(&im8, _T("资源文件\\skill\\闪电3bk.jpg"), 70, 220);
	putimage(450, -10, &im8, NOTSRCERASE);
	putimage(450, -10, &im7, SRCINVERT);
	FlushBatchDraw();
	Sleep(500);
	// mciSendString(_T("close sk1"), NULL, 0, NULL);
}

void flash()
{
	IMAGE im7, im8;
	loadimage(&im7, _T("资源文件\\skill\\电磁波.jpg"));
	loadimage(&im8, _T("资源文件\\skill\\电磁波bk.jpg"));
	putimage(350, 30, &im8, NOTSRCERASE);
	putimage(350, 30, &im7, SRCINVERT);
	FlushBatchDraw();
	Sleep(100);
	IMAGE im9, im10;
	loadimage(&im9, _T("资源文件\\skill\\电磁波2.jpg"));
	loadimage(&im10, _T("资源文件\\skill\\电磁波2bk.jpg"));
	putimage(350, 80, &im10, NOTSRCERASE);
	putimage(350, 80, &im9, SRCINVERT);
	FlushBatchDraw();
	Sleep(100);
	IMAGE im11, im12;
	loadimage(&im11, _T("资源文件\\skill\\电磁波3.jpg"));
	loadimage(&im12, _T("资源文件\\skill\\电磁波3bk.jpg"));
	putimage(350, 105, &im12, NOTSRCERASE);
	putimage(350, 105, &im11, SRCINVERT);
	FlushBatchDraw();
	Sleep(100);
	IMAGE im13, im14;
	loadimage(&im13, _T("资源文件\\skill\\电磁波4.jpg"));
	loadimage(&im14, _T("资源文件\\skill\\电磁波4bk.jpg"));
	putimage(350, 135, &im14, NOTSRCERASE);
	putimage(350, 135, &im13, SRCINVERT);
	FlushBatchDraw();
	Sleep(100);
	IMAGE im15, im16;
	loadimage(&im15, _T("资源文件\\skill\\电磁波5.jpg"));
	loadimage(&im16, _T("资源文件\\skill\\电磁波5bk.jpg"));
	putimage(350, 165, &im16, NOTSRCERASE);
	putimage(350, 165, &im15, SRCINVERT);
	FlushBatchDraw();
	Sleep(500);
	//mciSendString(_T("close sk3"), NULL, 0, NULL);
}


//打雷skill的展示
void thunder()
{
	IMAGE im3, im4;
	loadimage(&im3, _T("资源文件\\skill\\打雷1.jpg"), 70, 200);
	loadimage(&im4, _T("资源文件\\skill\\打雷1bk.jpg"), 70, 200);
	putimage(450, 0, &im4, NOTSRCERASE);
	putimage(450, 0, &im3, SRCINVERT);
	FlushBatchDraw();
	Sleep(400);
	IMAGE im5, im6;
	loadimage(&im5, _T("资源文件\\skill\\打雷2.jpg"), 70, 200);
	loadimage(&im6, _T("资源文件\\skill\\打雷2bk.jpg"), 70, 200);
	putimage(400, 0, &im6, NOTSRCERASE);
	putimage(400, 0, &im5, SRCINVERT);
	FlushBatchDraw();
	Sleep(500);
	//	mciSendString(_T("close sk4"), NULL, 0, NULL);
}


//水柱
void waterattack()
{
	IMAGE im9, im10;
	loadimage(&im9, _T("资源文件\\skill\\水珠2.jpg"));
	loadimage(&im10, _T("资源文件\\skill\\水珠2bk.jpg"));
	putimage(100, 130, &im10, NOTSRCERASE);
	putimage(100, 130, &im9, SRCINVERT);
	FlushBatchDraw();
	Sleep(200);
	IMAGE im11, im12;
	loadimage(&im11, _T("资源文件\\skill\\水珠3.jpg"));
	loadimage(&im12, _T("资源文件\\skill\\水珠3bk.jpg"));
	putimage(95, 130, &im12, NOTSRCERASE);
	putimage(95, 130, &im11, SRCINVERT);
	FlushBatchDraw();
	Sleep(500);
	//	mciSendString(_T("close enemy_3"), NULL, 0, NULL);
}


//剑雨
void sword()
{
	IMAGE im11;
	int x = 240, y = 90;
	for (x = 240, y = 90; x >= 140 && y <= 140; x = x - 2, y = y + 1)
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
	putimage(70, 140, &im10, NOTSRCERASE);
	putimage(70, 140, &im9, SRCINVERT);
	FlushBatchDraw();
	Sleep(500);
	putimage(70, 140, &im10, NOTSRCERASE);
	putimage(70, 140, &im9, SRCINVERT);
	FlushBatchDraw();
	putimage(70, 140, &im10, NOTSRCERASE);
	putimage(70, 140, &im9, SRCINVERT);
	Sleep(1000);
	FlushBatchDraw();
	putimage(70, 140, &im10, NOTSRCERASE);
	putimage(70, 140, &im9, SRCINVERT);
	Sleep(1000);
	FlushBatchDraw();
	Sleep(500);
	//Sleep(500);
	//mciSendString(_T("close enemy_5"), NULL, 0, NULL);
}