#define _CRT_SECURE_NO_WARNINGS
#include "DxLib.h"
#include <math.h>

//int field(void);
void getColor(int& color1, int& color2, const int& mousePos);
void afterClick(const int& mousePos, int& symbol, int& win, int& scene);
void afterClick(int& symbol, int& win, int& scene); //CPU
void turnChange(int& symbol);
void reach(int& r);
void cpuChangeZahyou();
void Winer(int win, int max);
void Player(int symbol);
int getPos(int mouseX, int mouseY, int fiePix);

const int WIDTH = 960, HEIGHT = 640;
enum { TITLE, PLAY, CPU, OVER };
int scene = TITLE;

const int SYMBOL_O = 1, SYMBOL_X = -1;
//フィールドの定義
const int FIELD_PIXEL = 64 * 2;
const int marginX = 288;
const int marginY = 128;
int field[9];
int zahyouX[9];
int zahyouY[9];
int cpuZahyou;
const int pattern[8][3] = {
	{1,2,3},
	{4,5,6},
	{7,8,9},
	{1,4,7},
	{2,5,8},
	{3,6,9},
	{1,5,9},
	{3,5,7}
};
const int symbol_value[3] = { 10,0,100 };
const int fieldYuusenn[9] = { 5,1,3,7,9,2,4,6,8 };


int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{

	SetWindowText("三目並べ");
	SetGraphMode(WIDTH, HEIGHT, 32);
	ChangeWindowMode(TRUE);
	if (DxLib_Init() == -1)return -1;
	SetBackgroundColor(0, 0, 0);
	SetDrawScreen(DX_SCREEN_BACK);

	int timer = 0;
	int counter = 0;
	int game = -1;

	int x, y, sx, sy;
	for (int i = 0; i < 9; i++) {
		field[i] = -1;
		zahyouX[i] = i % 3 * FIELD_PIXEL + marginX;
		zahyouY[i] = i / 3 * FIELD_PIXEL + marginY;
	}

	int color1 = 0xffffff;
	int color2 = 0xffffff;

	//プレイ関係
	int win = 0;
	int playerReach = -1;
	//〇か×かを判別
	int symbol = SYMBOL_O;
	int max = 0;
	//×の長さ
	int crossRange = 25;
	//CPU
	int cpuReach = -1;

	//マウスカーソル
	int mouseX, mouseY;
	int mousePos = -1;
	int mouseInput;
	int mouseLeft = 0;

	while (1)
	{

		switch (scene) {
		case TITLE:
			//タイトル処理
			//初期化
			for (int i = 0; i < 9; i++) {
				field[i] = -1;
			}
			win = 0;
			playerReach = -1;
			cpuReach = -1;
			counter = 0;
			//マウス
			GetMousePoint(&mouseX, &mouseY);
			mouseInput = GetMouseInput();
			mousePos = getPos(mouseX, mouseY, FIELD_PIXEL);
			getColor(color1, color2, mousePos);
			if ((mouseInput & MOUSE_INPUT_LEFT) == 1) {
				if (mouseLeft == 0) {
					if (mousePos == 3) { scene = PLAY; game = 1; }
					else if (mousePos == 5) { scene = CPU; game = 2; }
				}
			}
			mouseLeft = mouseInput & MOUSE_INPUT_LEFT;
			break;

		case PLAY:

			max = 0;

			//マウス
			GetMousePoint(&mouseX, &mouseY);
			mouseInput = GetMouseInput();
			//左クリック後の処理
			if ((mouseInput & MOUSE_INPUT_LEFT) == 1) {
				if (mouseLeft == 0) {
					mousePos = getPos(mouseX, mouseY, FIELD_PIXEL);
					if (field[mousePos] == -1) {
						afterClick(mousePos, symbol, win, scene);
					}
				}
			}
			for (int j = 0; j < 9; j++) {
				if (field[j] != -1)max++;
			}
			mouseLeft = mouseInput & MOUSE_INPUT_LEFT;
			if (max == 9)scene = OVER;
			if (CheckHitKey(KEY_INPUT_BACK) == 1)scene = TITLE;
			break;

		case CPU:
			//CPUの処理
			max = 0;

			//マウス
			GetMousePoint(&mouseX, &mouseY);
			mouseInput = GetMouseInput();
			//左クリック後の処理
			if (symbol == 1) {
				if ((mouseInput & MOUSE_INPUT_LEFT) == 1) {
					if (mouseLeft == 0) {
						mousePos = getPos(mouseX, mouseY, FIELD_PIXEL);
						if (field[mousePos] == -1) {
							afterClick(mousePos, symbol, win, scene);
						}
					}
				}
			}
			else if (symbol == -1) {
				counter++;
				for (int i = 0; i < 8; i++) {
					if (field[pattern[i][0] - 1] + field[pattern[i][1] - 1] + field[pattern[i][2] - 1] == 19) {
						cpuReach = i;
					}
					else if (field[pattern[i][0] - 1] + field[pattern[i][1] - 1] + field[pattern[i][2] - 1] == 199) {
						playerReach = i;
					}
				}
				if (counter >= 60) {
					if (cpuReach != -1) {
						reach(cpuReach);
					}
					else if (playerReach != -1) {
						reach(playerReach);
					}
					else {
						cpuChangeZahyou();
					}
					afterClick(symbol, win, scene);
					counter = 0;
				}
			}
			for (int j = 0; j < 9; j++) {
				if (field[j] != -1)max++;
			}
			mouseLeft = mouseInput & MOUSE_INPUT_LEFT;
			if (max == 9)scene = OVER;
			if (CheckHitKey(KEY_INPUT_BACK) == 1)scene = TITLE;
			break;

		case OVER:

			//Winer(symbol,max);
			timer++;
			if (timer == 60 * 3) {
				if (game == 1)scene = PLAY;
				else if (game == 2)scene = CPU;
				timer = 0;
				for (int i = 0; i < 9; i++) {
					field[i] = -1;
				}
				win = 0;
				playerReach = -1;
				cpuReach = -1;
				counter = 0;
			}
			break;
		}

		//フィールド描画
		DrawBox(0, 0, WIDTH, HEIGHT, 0x008050, TRUE);
		if (scene != TITLE) {
			for (int i = 0; i < 9; i++) {
				int sx = zahyouX[i];
				int sy = zahyouY[i];
				DrawBox(sx, sy, sx + FIELD_PIXEL, sy + FIELD_PIXEL, 0xafaf00, FALSE);
				if (field[i] == 100)DrawCircle(sx + FIELD_PIXEL / 2, sy + FIELD_PIXEL / 2, FIELD_PIXEL / 2 - 20, 0xffffff, FALSE);
				else if (field[i] == 10) {
					DrawLine(sx + crossRange, sy + crossRange, sx + FIELD_PIXEL - crossRange, sy + FIELD_PIXEL - crossRange, 0x000000);
					DrawLine(sx + crossRange, sy + FIELD_PIXEL - crossRange, sx + FIELD_PIXEL - crossRange, sy + crossRange, 0x000000);
				}
			}
		}
		if (scene == PLAY || scene == CPU) {
			Player(symbol);
			DrawBox(5, 5, 67, 67, 0xffffff, FALSE);
			SetFontSize(10);
			DrawString(10, 20, "BackSpace", 0xffffff);
			DrawString(10, 40, "でタイトル", 0xffffff);
		}
		if (scene == OVER) {
			SetDrawBlendMode(DX_BLENDMODE_ALPHA, 128);
			DrawBox(0, 0, WIDTH, HEIGHT, 0x000000, TRUE);
			SetDrawBlendMode(DX_BLENDMODE_NOBLEND, 0);
			Winer(win, max);
		}
		if (scene == TITLE) {
			SetFontSize(80);
			DrawString(WIDTH / 2 - 80 * 2, 60, "三目並べ", 0xffffff);
			SetFontSize(40);
			DrawBox(zahyouX[3], zahyouY[3], zahyouX[3] + FIELD_PIXEL, zahyouY[3] + FIELD_PIXEL, color1, FALSE);
			DrawString(zahyouX[3] + 34, zahyouY[3] + 44, "PvP", color1);
			DrawBox(zahyouX[5], zahyouY[5], zahyouX[5] + FIELD_PIXEL, zahyouY[5] + FIELD_PIXEL, color2, FALSE);
			DrawString(zahyouX[5] + 34, zahyouY[5] + 44, "CPU", color2);
		}

		ScreenFlip();
		WaitTimer(16);
		if (ProcessMessage() == -1)break;
		if (CheckHitKey(KEY_INPUT_ESCAPE) == 1)break;
	}

	DxLib_End();
	return 0;
}

void getColor(int& color1, int& color2, const int& mousePos)
{
	if (mousePos == 3) {
		color1 = 0xffff00;
	}
	else if (mousePos == 5) {
		color2 = 0xffff00;
	}
	else {
		color1 = 0xffffff;
		color2 = 0xffffff;
	}
}

void afterClick(const int& mousePos, int& symbol, int& win, int& scene)
{
	field[mousePos] = symbol_value[symbol + 1];
	for (int i = 0; i < 8; i++) {
		if (field[pattern[i][0] - 1] == symbol_value[symbol + 1] &&
			field[pattern[i][1] - 1] == symbol_value[symbol + 1] &&
			field[pattern[i][2] - 1] == symbol_value[symbol + 1])
		{
			scene = OVER;
			win = symbol;
		}
	}
	if (scene == PLAY || scene == CPU)turnChange(symbol);
}

void afterClick(int& symbol, int& win, int& scene)
{
	for (int i = 0; i < 8; i++) {
		if (field[pattern[i][0] - 1] == symbol_value[symbol + 1] &&
			field[pattern[i][1] - 1] == symbol_value[symbol + 1] &&
			field[pattern[i][2] - 1] == symbol_value[symbol + 1])
		{
			scene = OVER;
			win = symbol;
		}
	}
	if (scene == CPU)turnChange(symbol);
}

void turnChange(int& symbol)
{
	if (symbol == SYMBOL_O)symbol = SYMBOL_X;
	else if (symbol == SYMBOL_X) symbol = SYMBOL_O;
}

void reach(int& r)
{
	for (int i = 0; i < 3; i++) {
		cpuZahyou = field[pattern[r][i] - 1];
		if (cpuZahyou == -1) {
			field[pattern[r][i] - 1] = 10;
			r = -1;
			break;
		}
	}
}

void cpuChangeZahyou()
{
	for (int i = 0; i < 9; i++) {
		cpuZahyou = field[fieldYuusenn[i] - 1];
		if (cpuZahyou == -1) {
			field[fieldYuusenn[i] - 1] = 10;
			break;
		}
	}
}

void Winer(int win, int max) {
	SetFontSize(60);
	if (win == 1)DrawString(WIDTH / 2 - (60 * 2), HEIGHT / 2 - 30, "〇の勝利", 0xffff00);
	else if (win == -1)DrawString(WIDTH / 2 - (60 * 2), HEIGHT / 2 - 30, "×の勝利", 0xffff00);
	else if (max == 9)DrawString(WIDTH / 2 - (60 * 1.5), HEIGHT / 2 - 30, "ドロー", 0xffff00);
}

void Player(int symbol)
{
	SetFontSize(60);
	if (symbol == 1)DrawString(WIDTH / 2 - (60 * 1.5), 30, "〇の番", 0xffffff);
	else if (symbol == -1)DrawString(WIDTH / 2 - (60 * 1.5), 30, "×の番", 0xffffff);
}

int getPos(int mouseX, int mouseY, int fiePix)
{
	int pos = -1;
	for (int i = 0; i < 9; i++) {
		if (zahyouX[i] <= mouseX && mouseX <= zahyouX[i] + fiePix) {
			if (zahyouY[i] <= mouseY && mouseY <= zahyouY[i] + fiePix) {
				pos = i;
				break;
			}
		}
	}
	return pos;
}
