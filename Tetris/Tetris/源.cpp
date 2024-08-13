/*�����С��Ϸ�����У�ͳһ�涨�����x�йص��������row��y����column*/
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

// ˳ʱ����ת��
int rotate(int px, int py, int r) {
	switch (r % 4) {
	case 0: return py * 4 + px;
	case 1: return 12 + py - 4 * px;
	case 2: return 15 - px - 4 * py;
	case 3: return 3 - py + 4 * px;
	}
}

// �����ʲ�
void create_assests(wstring* tetromino) {
	tetromino[0].append(L"..X...X...X...X."); // Tetronimos 4x4
	tetromino[1].append(L"..X..XX...X.....");
	tetromino[2].append(L".....XX..XX.....");
	tetromino[3].append(L"..X..XX..X......");
	tetromino[4].append(L".X...XX...X.....");
	tetromino[5].append(L".X...X...XX.....");
	tetromino[6].append(L"..X...X..XX.....");
}

// ��ʼ����ͼ, ��ǽ����Ϊ9�����ƶ��ռ���Ϊ0
void initialize_pField() {
	pField = new unsigned char[BHEIGHT * BWIDTH];
	for (int y = 0; y < BHEIGHT; y++) {
		for (int x = 0; x < BWIDTH; x++) {
			pField[x + y * BWIDTH] = (x == 0 || x == BWIDTH - 1 || y == BHEIGHT - 1) ? 9 : 0;
		}
	}
}

// �жϷ���λ���Ƿ�Ϸ�������nPosX��nPosY�����˵�ǰ4*4��������Ͻ�λ��
bool DosePieceFit(int nTetromino, int nrotate, int nPosX, int nPosY) {
	for (int py = 0; py < 4; py++) {
		for (int px = 0; px < 4; px++) {

			// ��ȡ��ת�󷽿������
			int pi = rotate(px, py, nrotate);

			// ��ȡ��ͼ������
			int fi = (px + nPosX) + (py + nPosY) * BWIDTH;

			//�߽���
			if ((px + nPosX) >= 0 && (px + nPosX) < BWIDTH) {
				if ((py + nPosY) >= 0 && (py + nPosY), BHEIGHT) {
					// �������������Ϊ�յ�ͬʱ����Ҳ��Ϊ�գ���÷���������λ���ǷǷ���
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
	bool zRotateHold = true; //ȷ�����µ�ʱ�򲻻�������ת

	int nSpeed = 20; // ÿ��һ�뷽������һ��
	int nSpeedCounter = 0;
	bool bForceDown = false;
	int nPiecesCount = 0;// ͳ�����䷽������
	int nScoreCount = 0;

	vector<int> vLines;// ���ڴ���Ҫ�����line������

	// ���ڼ���Ӧ���Ƿ��£�������£���Ϊ1��û����Ϊ0
	bool bKey[4];

	while (!bGameOver) {
		// ��Ϸÿ�̵�ʱ��
		this_thread::sleep_for(50ms);// �������߳���ͣ50����
		nSpeedCounter++;
		bForceDown = (nSpeedCounter == nSpeed);
		/*
		����ض������Ƿ񱻰���
		GetAsyncKeyState()���ڼ���ض����Ƿ񱻰��£�����һ��16λ�Ķ���������
		��ָ�����̰������15λ�᷵��1��û���·���0����0λ���ڴ洢����״̬�Ƿ�ı�
		������ 1000 0000 0000 0000�ͷ���ֵ���а�λ��(&)�����Ļ���
		��������˾���1000��0000��0000��0000����ô!=0֮�����1
		���û���¾���0000 0000 0000 0000��!=0֮��λ0���Ϳ��Ա��水��״̬��
		�൱����һ�������ڣ������ж�/x27,/x25,/x28,z�Ƿ񱻰��£��������������bKey��
		*/
		for (int k = 0; k < 4; k++) {											//left right down rotate
			bKey[k] = (0x8000 & GetAsyncKeyState((unsigned char)("ADSR"[k]))) != 0;
		}

		// A�����£����ƶ�1����λ
		nCurrentX -= (bKey[0] && DosePieceFit(nCurrentPiece, nCurrentRotation, nCurrentX - 1, nCurrentY)) ? 1 : 0;

		// D�����£�����һ����λ
		nCurrentX += (bKey[1] && DosePieceFit(nCurrentPiece, nCurrentRotation, nCurrentX + 1, nCurrentY)) ? 1 : 0;

		//S�����£����ƶ�һ����λ
		nCurrentY += (bKey[2] && DosePieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) ? 1 : 0;

		//Z�����£���ת��ͬʱȷ��һֱ���µ�ʱ�򲻻�������ת
		if (bKey[3]) {
			if (zRotateHold && DosePieceFit(nCurrentPiece, nCurrentRotation + 1, nCurrentX, nCurrentY)) {
				nCurrentRotation += 1; zRotateHold = false;
			}
		}
		else {
			zRotateHold = true;
		}

		//������������
		if (bForceDown) {
			nSpeedCounter = 0;
			if (DosePieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY + 1)) {
				nCurrentY++;
			}
			else {
				//������ǰ���飬��Ҫ����ǰ�ķ���̶�������pfield�г�Ϊ����
				for (int y = 0; y < 4; y++) {
					for (int x = 0; x < 4; x++) {
						if (tetromino[nCurrentPiece][rotate(x, y, nCurrentRotation)] == L'X') {
							pField[nCurrentX + x + (nCurrentY + y) * BWIDTH] = nCurrentPiece + 1;
						}
					}
				}

				//����Ƿ����������ˮƽ�ߣ����ڷ���ĳ���ֻ��4�����أ����ÿ��ֻ��Ҫ����4������֮���������
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
							vLines.push_back(y + nCurrentY);//�ѵ�ǰ�е�λ�ô�����飬����ͳһɾ��
						}
					}
				}

				//ѡ����һ������
				nCurrentX = BWIDTH / 2;
				nCurrentY = 0;
				nCurrentRotation = 0;
				nCurrentPiece = rand() % 7;
				nPiecesCount++;
				if (nPiecesCount % 10 == 0) {
					if (nSpeed > 10)nSpeed--;
				}

				nScoreCount += 25;//ÿ����һ������+25��
				//��Ϸ����
				bGameOver = !DosePieceFit(nCurrentPiece, nCurrentRotation, nCurrentX, nCurrentY);
			}
		}
		//���Ƶ�ͼ
		for (int y = 0; y < BHEIGHT; y++) {
			for (int x = 0; x < BWIDTH; x++) {
				screen[(x + 2) + (y + 2) * nScreenWidth] = L" ABCDEFG=#"[pField[x + y * BWIDTH]];
			}
		}

		// ���Ƶ�ǰ�Ķ���˹����
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
		//���ƻ���
		WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWritten);
	}
	// ������Ϸ
	CloseHandle(hConsole);
	cout << "Game Over!! Score:" << nScoreCount << endl;
	system("pause");
	return 0;
}