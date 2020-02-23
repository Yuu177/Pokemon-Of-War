//音乐
void load_music();
void play_music_one(char *);
void music_choose();
void music_fight();
void music_next();
void music_open_door();
void music_bk1();
void music_start();
void music_victory();
void music_defeat();
void music_get();



void music_choose()
{
	play_music_one("资源文件\\music\\选择.mp3");
}


void music_next()
{
	play_music_one("资源文件\\music\\确认.wav");
}



void music_open_door()
{
	play_music_one("资源文件\\music\\开门声.wav");
}


void music_bk1()
{
	mciSendString(_T("close startmusic"), NULL, 0, NULL);
	mciSendString(_T("close fgmusic"), NULL, 0, NULL);
	mciSendString(_T("open 资源文件\\music\\背景音乐.mp3 alias bkmusic1"), NULL, 0, NULL);
	mciSendString(_T("play bkmusic1 repeat"), NULL, 0, NULL);
}


void music_fight()
{
	mciSendString(_T("close bkmusic1"), NULL, 0, NULL);
	mciSendString(_T("open 资源文件\\music\\战斗2.mp3 alias fgmusic"), NULL, 0, NULL);
	mciSendString(_T("play fgmusic repeat"), NULL, 0, NULL);
}

void music_start()
{
	mciSendString(_T("open 资源文件\\music\\开始音乐.mp3 alias startmusic"), NULL, 0, NULL);
	mciSendString(_T("play startmusic repeat"), NULL, 0, NULL);
}




void play_music_one(char *filename)
{
	char cmdString[50] = "open ";
	strcat(cmdString, filename);
	strcat(cmdString, " alias tmpmusic");
	TCHAR w_cmdString[50];
	CharToTchar(cmdString, w_cmdString);
	mciSendString(_T("close tmpmusic"), NULL, 0, NULL);
	mciSendString(w_cmdString, NULL, 0, NULL);
	mciSendString(_T("play tmpmusic"), NULL, 0, NULL);
}


void music_victory()
{
	mciSendString(_T("close fgmusic"), NULL, 0, NULL);
	play_music_one("资源文件\\music\\胜利.mp3");
}


void music_defeat()
{
	mciSendString(_T("close fgmusic"), NULL, 0, NULL);
	play_music_one("资源文件\\music\\失败.mp3");
}



void music_get()
{
	//mciSendString(_T("close fgmusic"), NULL, 0, NULL);
	play_music_one("资源文件\\music\\获得物品.mp3");
}



/*void load_music()
{
	mciSendString(_T("open 资源文件\\music\\开始音乐.mp3 alias startmusic"), NULL, 0, NULL);
	mciSendString(_T("open 资源文件\\music\\背景音乐.mp3 alias bkmusic1"), NULL, 0, NULL);
	mciSendString(_T("open 资源文件\\music\\选择.wav alias chmusic"), NULL, 0, NULL);
	mciSendString(_T("open 资源文件\\music\\下一页2.wav alias nemusic"), NULL, 0, NULL);
	mciSendString(_T("open 资源文件\\music\\战斗2.mp3 alias fgmusic"), NULL, 0, NULL);
	mciSendString(_T("open 资源文件\\music\\开门声.wav alias opmusic"), NULL, 0, NULL);
}*/

