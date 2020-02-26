//音乐
void play_music_one(char *);
void music_choose();
void music_fight1();
void music_fight2();
void music_next();
void music_open_door();
void music_bk1();
void music_bk2();
void music_bk3();
void music_bk4();
void music_start();
void music_victory();
void music_defeat();
void music_get();
void startup_skill_music();	//初始化宝可梦技能的音效
void music_end();


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
	mciSendString(_T("open 资源文件\\music\\背景音乐1.mp3 alias bkmusic1"), NULL, 0, NULL);
	mciSendString(_T("play bkmusic1 repeat"), NULL, 0, NULL);
}

void music_bk2()
{
	mciSendString(_T("open 资源文件\\music\\背景音乐2.mp3 alias bkmusic2"), NULL, 0, NULL);
	mciSendString(_T("play bkmusic2 repeat"), NULL, 0, NULL);
}


void music_bk3()
{
	mciSendString(_T("open 资源文件\\music\\背景音乐3.mp3 alias bkmusic3"), NULL, 0, NULL);
	mciSendString(_T("play bkmusic3 repeat"), NULL, 0, NULL);
}

void music_bk4()
{
	mciSendString(_T("open 资源文件\\music\\背景音乐4.mp3 alias bkmusic4"), NULL, 0, NULL);
	mciSendString(_T("play bkmusic4 repeat"), NULL, 0, NULL);
}


void music_fight1()
{
	mciSendString(_T("close bkmusic1"), NULL, 0, NULL);
	mciSendString(_T("close bkmusic2"), NULL, 0, NULL);
	mciSendString(_T("close bkmusic3"), NULL, 0, NULL);
	mciSendString(_T("close bkmusic4"), NULL, 0, NULL);
	mciSendString(_T("open 资源文件\\music\\战斗1.mp3 alias fgmusic1"), NULL, 0, NULL);
	mciSendString(_T("play fgmusic1 repeat"), NULL, 0, NULL);
}


void music_fight2()
{
	mciSendString(_T("close bkmusic1"), NULL, 0, NULL);
	mciSendString(_T("close bkmusic2"), NULL, 0, NULL);
	mciSendString(_T("close bkmusic3"), NULL, 0, NULL);
	mciSendString(_T("close bkmusic3"), NULL, 0, NULL);
	mciSendString(_T("open 资源文件\\music\\战斗2.mp3 alias fgmusic2"), NULL, 0, NULL);
	mciSendString(_T("play fgmusic2 repeat"), NULL, 0, NULL);
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
	mciSendString(_T("close fgmusic1"), NULL, 0, NULL);
	mciSendString(_T("close fgmusic2"), NULL, 0, NULL);
	play_music_one("资源文件\\music\\胜利.mp3");
}


void music_defeat()
{
	mciSendString(_T("close fgmusic1"), NULL, 0, NULL);
	mciSendString(_T("close fgmusic2"), NULL, 0, NULL);
	play_music_one("资源文件\\music\\失败.mp3");
}



void music_get()
{
	mciSendString(_T("close getmusic"), NULL, 0, NULL);
	mciSendString(_T("open 资源文件\\music\\获得物品.mp3 alias getmusic"), NULL, 0, NULL);
	mciSendString(_T("play getmusic"), NULL, 0, NULL);
}


void startup_skill_music()
{
	mciSendString(_T("open 资源文件\\music\\十万伏特.mp3 alias sk1"), NULL, 0, NULL);
	mciSendString(_T("open 资源文件\\music\\尖叫.mp3 alias sk2"), NULL, 0, NULL);
	mciSendString(_T("open 资源文件\\music\\电磁波.mp3 alias sk3"), NULL, 0, NULL);
	mciSendString(_T("open 资源文件\\music\\打雷.mp3 alias sk4"), NULL, 0, NULL);
	mciSendString(_T("open 资源文件\\music\\剑雨.mp3 alias enemy_1"), NULL, 0, NULL);
}


void music_end()
{
	mciSendString(_T("close bkmusic1"), NULL, 0, NULL);
	mciSendString(_T("close bkmusic2"), NULL, 0, NULL);
	mciSendString(_T("close bkmusic3"), NULL, 0, NULL);
	mciSendString(_T("close bkmusic3"), NULL, 0, NULL);
	mciSendString(_T("open 资源文件\\music\\结束音乐.mp3 alias endmusic"), NULL, 0, NULL);
	mciSendString(_T("play endmusic"), NULL, 0, NULL);
}