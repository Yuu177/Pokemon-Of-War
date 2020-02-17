//char 转换为 tchar
void CharToTchar(const char * _char, TCHAR * tchar)
{
	int iLength;
	iLength = MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, NULL, 0);
	MultiByteToWideChar(CP_ACP, 0, _char, strlen(_char) + 1, tchar, iLength);
}


//界面切换动画
void interface_change_animatio(int width, int high)
{
	int i, j;
	i = width / 2;
	j = 0;
	setlinecolor(BLACK);
	while (!(i == width / 2 - 1 && j == 0))
	{
		line(width / 2, high / 2, i, j);
		if (i < width && j == 0)			i++;
		else if (i == width && j < high)	j++;
		else if (i > 0 && j == high)		i--;
		else if (i == 0 && j > 0)			j--;
		FlushBatchDraw();
	}
	line(width / 2, high / 2, i, j);
	FlushBatchDraw();
	Sleep(500);
}


//指定透明色贴图
//背景颜色需要和图案上的颜色没有相同才行
void transparentimage(IMAGE *dstimg, int x, int y, IMAGE *srcimg, UINT transparentcolor)
{
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = srcimg->getwidth();
	int h = srcimg->getheight();
	// 使用 Windows GDI 函数实现透明位图
	TransparentBlt(dstDC, x, y, w, h, srcDC, 0, 0, w, h, transparentcolor);
}


void next_step()
{
	FlushBatchDraw();
	//清除键盘缓冲区,防止切换过场动画时候按到其他按键
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	while (1)
	{
		char input = _getch();
		if (input == 'j') break;
	}
}
