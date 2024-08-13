/*在这个小游戏代码中，统一规定任意和x有关的坐标代表row，y代表column*/
# include<iostream>
#include<string>
#include<Windows.h>
#include<thread>
#include<vector>

#define BHEIGHT 18
#define BWIDTH 12
#define nScreenHeight 30
#define nScreenWidth 120
using namespace std;

wstring tetromino[7];

unsigned char* pField = nullptr;

// 顺时针旋转°
int rotate(int px, int py, int r) {
	switch (r % 4) {
	case 0: return py * 4 + px;
	case 1: return 12 + py - 4 * px;
	case 2: return 15 - px - 4 * py;
	case 3: return 3 - py + 4 * px;
	}
}

// 创建资产
void create_assests(wstring* tetromino) {
	tetromino[0].append(L"..X...X...X...X."); // Tetronimos 4x4
	tetromino[1].append(L"..X..XX...X.....");
	tetromino[2].append(L".....XX..XX.....");
	tetromino[3].append(L"..X..XX..X......");
	tetromino[4].append(L".X...XX...X.....");
	tetromino[5].append(L".X...X...XX.....");
	tetromino[6].append(L"..X...X..XX.....");
}

// 初始化地图, 将墙壁设为9，可移动空间设为0
void initialize_pField() {
	pField = new unsigned char[BHEIGHT * BWIDTH];
	for (int y = 0; y < BHEIGHT; y++) {
		for (int x = 0; x < BWIDTH; x++) {
			pField[x + y * BWIDTH] = (x == 0 || x == BWIDTH - 1 || y == BHEIGHT - 1) ? 9 : 0;
		}
	}
}

// 判断方块位置是否合法，其中nPosX，nPosY代表了当前4*4方块的左上角位置
bool DosePieceFit(int nTetromino, int nrotate, int nPosX, int nPosY) {
	for (int py = 0; py < 4; py++) {
		for (int px = 0; px < 4; px++) {

			// 获取旋转后方块的索引
			int pi = rotate(px, py, nrotate);

			// 获取地图的索引
			int fi = (px + nPosX) + (py + nPosY) * BWIDTH;

			//边界检查
			if ((px + nPosX) >= 0 && (px + nPosX) < BWIDTH) {
				if ((py + nPosY) >= 0 && (py + nPosY), BHEIGHT) {
					// 如果方块索引不为空的同时场地也不为空，则该方块所处的位置是非法的
					if (tetromino[nTetromino][pi] != L'.' && pField[fi] != 0) {
						return false;
					}
				}
			}
		}
	}

	return true;
}

int main() {
	create_assests(tetromino);

	wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
	for (int i = 0; i < nScreenWidth * nScreenHeight; i++) { screen[i] = L' '; }
	HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleActiveScreenBuffer(hConsole);
	DWORD dwBytesWritten = 0;

	initialize_pField();

	bool bGameOver = false;

	int nCurrentPiece = 1;
	int nCurrentRotation = 0;
	int nCurrentX = BWIDTH / 2;
	int nCurrentY = 0;
	bool zRotateHold = true; //确保按下的时候不会连续旋转

	int nSpeed = 20; // 每隔一秒方块落下一格
	int nSpeedCounter = 0;
	bool bForceDown = false;
	int nPiecesCount = 0;// 统计下落方块数量
	int nScoreCount = 0;

	vector<int> vLines;// 用于储存要清除的line行索引

	// 用于检测对应键是否按下，如果按下，则为1，没按下为0
	bool bKey[4];

	while (!bGameOver) {
		// 游戏每刻的时长
		this_thread::sleep_for(50ms);// 让整个线程暂停50毫秒
		nSpeedCounter++;
		bForceDown = (nSpeedCounter == nSpeed);
		/*
		检查特定按键是否被按下
		GetAsyncKeyState()用于检测特定键是否被按下，返回一个16位的二进制数，
		若指定键盘按下则第15位会返回1，没按下返回0，第0位用于存储按下状态是否改变
		用掩码 1000 0000 0000 0000和返回值进行按位与(&)操作的话，
		如果按下了就是1000，0000，0000，0000，那么!=0之后就是1
		如果没按下就是0000 0000 0000 0000，!=0之后位0，就可以保存按键状态了
		相当于在一个周期内，连续判断/x27,/x25,/x28,z是否被按下，并将结果保存在bKey中
		*/
		for (int k = 0; k < 4; k++) {											//left right down rotate
			bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("ADSR"[k]))) != 0;
		}

		// A被按下，左移动1个单位
		nCurrentX -= (bKey[0] && DosePieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY)) ? 1 : 0;

		// D被按下，右移一个单位
		nCurrentX += (bKey[1] && DosePieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY)) ? 1 : 0;

		//S被按下，下移动一个单位
		nCurrentY += (bKey[2] && DosePieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) ? 1 : 0;

		//Z被按下，旋转，同时确保一直按下的时候不会连续旋转
		if (bKey[3]) {
			if (zRotateHold && DosePieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY)) {
				nCurrentRotation += 1; zRotateHold = false;
			}
		}
		else {
			zRotateHold = true;
		}

		//方块自由下落
		if (bForceDown) {
			nSpeedCounter = 0;
			if (DosePieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) {
				nCurrentY++;
			}
			else {
				//锁定当前方块，需要将当前的方块固定绘制在pfield中成为背景
				for (int y = 0; y < 4; y++) {
					for (int x = 0; x < 4; x++) {
						if (tetromino[nCurrentPiece][rotate(x, y, nCurrentRotation)] == L'X') {
							pField[nCurrentX + x + (nCurrentY + y) * BWIDTH] = nCurrentPiece + 1;
						}
					}
				}

				//检查是否创造出完整的水平线，由于方块的长度只有4个像素，因此每次只需要在这4个像素之间检查就行了
				for (int y = 0; y < 4; y++) {
					if ((nCurrentY + y) < BHEIGHT - 1) {
						BOOL bLine = true;
						for (int x = 1; x < BWIDTH - 1; x++) {
							if (pField[x + BWIDTH * (nCurrentY + y)] == 0) {
								bLine = false;
							}
						}
						if (bLine == true) {
							for (int x = 1; x < BWIDTH - 1; x++) {
								pField[x + BWIDTH * (nCurrentY + y)] = 8;
							}
							vLines.push_back(y + nCurrentY);//把当前行的位置存进数组，后续统一删除
						}
					}
				}

				//选择下一个方块
				nCurrentX = BWIDTH / 2;
				nCurrentY = 0;
				nCurrentRotation = 0;
				nCurrentPiece = rand() % 7;
				nPiecesCount++;
				if (nPiecesCount % 10 == 0) {
					if (nSpeed > 10)nSpeed--;
				}

				nScoreCount += 25;//每更新一个方块+25分
				//游戏结束
				bGameOver = !DosePieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
			}
		}
		//绘制地图
		for (int y = 0; y < BHEIGHT; y++) {
			for (int x = 0; x < BWIDTH; x++) {
				screen[(x + 2) + (y + 2) * nScreenWidth] = L" ABCDEFG=#"[pField[x + y * BWIDTH]];
			}
		}

		// 绘制当前的俄罗斯方块
		for (int py = 0; py < 4; py++) {
			for (int px = 0; px < 4; px++) {
				if (tetromino[nCurrentPiece][rotate(px, py, nCurrentRotation)] == L'X') {
					screen[(nCurrentY + py + 2) * nScreenWidth + (nCurrentX + px + 2)] = nCurrentPiece + 65;
				}
			}
		}
		if (!vLines.empty()) {
			WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
			this_thread::sleep_for(400ms);
			for (auto& v : vLines) {
				for (int px = 1; px < BWIDTH - 1; px++) {
					for (int py = v; py >= 0; py--) {
						pField[px + BWIDTH * py] = pField[px + BWIDTH * (py - 1)];
					}
					pField[px] = 0;
				}
			}
			nScoreCount += (1 << vLines.size()) * 100;
			vLines.clear();
		}
		swprintf_s(&screen[2 * nScreenWidth + BWIDTH + 6], 16, L"SCORE: %8d", nScoreCount);
		//绘制画布
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}
	// 结束游戏
	CloseHandle(hConsole);
	cout << "Game Over!! Score:" << nScoreCount << endl;
	system("pause");
	return 0;
}