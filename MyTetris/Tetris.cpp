#include "Tetris.h"
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

//��ʼ�����з���
void InitBackBlock()
{
	for (GLint i = 0; i < COL; i++)
	{
		for (GLint j = 0; j < ROW; j++)
		{
			block[i][j].isBackBlock = true;
			block[i][j].color = BLACK;
		}
	}
}

//���ݷ�����Ϣ�������з���
void DrawBlock()
{
	for (GLint i = 0; i < COL; i++)
	{
		for (GLint j = 0; j < ROW; j++)
		{
			if (block[i][j].isBackBlock)
			{
				block[i][j].color = BLACK;
			}
			switch (block[i][j].color)
			{
			case RED:
				glColor3ub(255, 0, 0);
				break;
			case ORANGE:
				glColor3ub(250, 128, 10);
				break;
			case YELLOW:
				glColor3ub(255, 255, 0);
				break;
			case GREEN:
				glColor3ub(0, 255, 0);
				break;
			case CYAN:
				glColor3ub(0, 255, 255);
				break;
			case LIGHT_BLUE:
				glColor3ub(0xA6, 0xCA, 0xF0);
				break;
			case PURPLE:
				glColor3ub(255, 0, 255);
				break;
			case BLACK:
			default:
				glColor3ub(0, 0, 0);
				break;
			}
			glRecti(i, j, i + 1, j + 1);
			glLineWidth(2.0f);
			glBegin(GL_LINE_LOOP);
			glColor3f(0.0f, 0.0f, 0.0f);
			glVertex2i(i, j);
			glVertex2i(i + 1, j);
			glVertex2i(i + 1, j + 1);
			glVertex2i(i, j + 1);
			glEnd();
			glFlush();
		}
	}
}

//������һ��������ʾ
void DrawNextBlocks()
{
	GLint x, y;
	glPushMatrix();
	glScalef(3, 3, 3);
	for (GLint i = 0; i < 4; i++)
	{
		x = nextBlocks.blockRelativePos[i].x;
		y = nextBlocks.blockRelativePos[i].y + 1;
		switch (nextBlocks.blockColor)
		{
		case RED:
			glColor3ub(255, 0, 0);
			break;
		case ORANGE:
			glColor3ub(250, 128, 10);
			x = x + 1;
			break;
		case YELLOW:
			glColor3ub(255, 255, 0);
			break;
		case GREEN:
			glColor3ub(0, 255, 0);
			break;
		case CYAN:
			glColor3ub(0, 255, 255);
			break;
		case LIGHT_BLUE:
			glColor3ub(0xA6, 0xCA, 0xF0);
			break;
		case PURPLE:
			glColor3ub(255, 0, 255);
			break;
		case BLACK:
		default:
			glColor3ub(0, 0, 0);
			break;
		}
		glRecti(x, y, x + 1, y + 1);
		glLineWidth(2.0f);
		glBegin(GL_LINE_LOOP);
		glColor3f(0.0f, 0.0f, 0.0f);
		glVertex2i(x, y);
		glVertex2i(x + 1, y);
		glVertex2i(x + 1, y + 1);
		glVertex2i(x, y + 1);
		glEnd();
		glFlush();
	}
	
	glPopMatrix();
}

//ɾ��������������i+1��
void DeleteBlock(GLint i)
{
	for (GLint k = 0; k < COL; k++)
	{
		for (GLint j = i; j < ROW; j++)
		{
			//block[][i]������
			if (j == ROW - 1)
			{
				block[k][j].isBackBlock = true;
				block[k][j].color = BLACK;
			}
			else
			{
				block[k][j] = block[k][j + 1];
			}
		}
	}
}

//����Ƿ��п������ģ�����������
void DeleteCheck()
{
	bool isFull;
	GLint sumDeleteRow = 0;

	for (GLint i = 0; i < ROW; i++)
	{
		isFull = true;
		for (GLint j = 0; j < COL; j++)
		{
			if (block[j][i].isBackBlock)
			{
				isFull = false;
				break;
			}
		}
		if (isFull)
		{
			DeleteBlock(i);
			sumDeleteRow += 1;
			i--;
		}
	}

	sumScore += 100 * currentLevel*sumDeleteRow*sumDeleteRow;
	if (sumScore <= 2000)
	{
		currentLevel = 1;
	}
	else if (sumScore<=5000)
	{
		currentLevel = 2;
		timeDownMilliSec = 400;
	}
	else if (sumScore <= 10000)
	{
		currentLevel = 3;
		timeDownMilliSec = 400;
	}
	else if (sumScore <= 20000)
	{
		currentLevel = 4;
		timeDownMilliSec = 300;
	}
	else
	{
		currentLevel = 5;
		timeDownMilliSec = 200;
	}
}

//�������ر���
void BlocksToBackBlock()
{
	GLint x, y;
	for (GLint i = 0; i < 4; i++)
	{
		x = currentBlocks.currentCetnterLocation.x + currentBlocks.blockRelativePos[i].x;
		y = currentBlocks.currentCetnterLocation.y + currentBlocks.blockRelativePos[i].y;
		block[x][y].isBackBlock = true;
	}
}

//�������ĳɷ���
void BackBlockToBlocks()
{
	GLint x, y;
	for (GLint i = 0; i < 4; i++)
	{
		x = currentBlocks.currentCetnterLocation.x + currentBlocks.blockRelativePos[i].x;
		y = currentBlocks.currentCetnterLocation.y + currentBlocks.blockRelativePos[i].y;
		block[x][y].isBackBlock = false;
		block[x][y].color = currentBlocks.blockColor;
	}
}

//�����з�����߽߱���ײ���
bool ConflictCheck(BLOCKS curBlocks)
{
	GLint x, y;
	for (GLint i = 0; i < 4; i++)
	{
		x = curBlocks.currentCetnterLocation.x + curBlocks.blockRelativePos[i].x;
		y = curBlocks.currentCetnterLocation.y + curBlocks.blockRelativePos[i].y;
		if (y > ROW - 1)
		{
			if (x<0 || x>COL)
			{
				//�з��鳬���ұ߽�
				return true;
			}
			return false;
		}
		else
		{
			if (!block[x][y].isBackBlock)
			{
				//��ǰλ�����з���
				return true;
			}
			if (x<0 || x>COL)
			{
				//�з��鳬���ұ߽�
				return true;
			}
			if (y < 0)
			{
				//�з��鳬�±߽�
				return true;
			}
		}
	}
	return false;
}

//�ı䷽������,��ʱ��
void SwitchBlocks()
{
	BlocksToBackBlock();
	wcPt2D curPos = currentBlocks.currentCetnterLocation;
	BLOCKS newBlocks= blocksAllType[currentBlocks.nextBlocksType];
	newBlocks.currentCetnterLocation = curPos;
	if (!ConflictCheck(newBlocks))
	{
		currentBlocks = newBlocks;
	}
	BackBlockToBlocks();
}

//��������һ��
void DownBlocks()
{
	BlocksToBackBlock();
	BLOCKS newBlocks = currentBlocks;
	newBlocks.currentCetnterLocation.y = currentBlocks.currentCetnterLocation.y - 1;

	if (!ConflictCheck(newBlocks))
	{
		currentBlocks = newBlocks;
		BackBlockToBlocks();
	}
	else
	{
		//�������,�����µķ���
		BackBlockToBlocks();
		DeleteCheck();

		//��Ϸ�����ж�
		for (GLint i = 0; i < COL; i++)
		{
			if (!block[i][ROW - 1].isBackBlock&&ConflictCheck(newBlocks))
			{
				exit(1);
			}
		}
		currentBlocks = nextBlocks;
		nextBlocks = CreateBlocks();
	}
}

//����Ѹ���½�
void DownBlocksQuickly()
{
	BlocksToBackBlock();
	BLOCKS newBlocks = currentBlocks;
	newBlocks.currentCetnterLocation.y = currentBlocks.currentCetnterLocation.y - 1;

	if (!ConflictCheck(newBlocks))
	{
		do 
		{
			newBlocks.currentCetnterLocation.y = newBlocks.currentCetnterLocation.y - 1;
		} while (!ConflictCheck(newBlocks));

		currentBlocks = newBlocks;
		currentBlocks.currentCetnterLocation.y= newBlocks.currentCetnterLocation.y+1;
		BackBlockToBlocks();
	}
	else
	{
		//�������,�����µķ���
		BackBlockToBlocks();
		DeleteCheck();

		//��Ϸ�����ж�
		for (GLint i = 0; i < COL; i++)
		{
			if (!block[i][ROW - 1].isBackBlock&&ConflictCheck(newBlocks))
			{
				exit(1);
			}
		}
		currentBlocks = nextBlocks;
		nextBlocks = CreateBlocks();
	}
}

//��������һ��
void LeftMoveBlocks()
{
	BlocksToBackBlock();
	BLOCKS newBlocks = currentBlocks;
	newBlocks.currentCetnterLocation.x = currentBlocks.currentCetnterLocation.x - 1;
	if (!ConflictCheck(newBlocks))
	{
		currentBlocks = newBlocks;
	}
	BackBlockToBlocks();
}

//��������һ��
void RightMoveBlocks()
{
	BlocksToBackBlock();
	BLOCKS newBlocks = currentBlocks;
	newBlocks.currentCetnterLocation.x = currentBlocks.currentCetnterLocation.x + 1;
	if (!ConflictCheck(newBlocks))
	{
		currentBlocks = newBlocks;
	}
	BackBlockToBlocks();
}

//�����µķ���
BLOCKS CreateBlocks()
{
	BLOCKS newBlocks;
	GLint newBlockType = rand() % 7;
	switch (newBlockType)
	{
	default:
	case 0:
		newBlocks = blocksAllType[0];
		newBlocks.currentCetnterLocation.x = COL / 2 - 2;
		newBlocks.currentCetnterLocation.y = ROW - 1;
		break;
	case 1:
		newBlocks = blocksAllType[4];
		newBlocks.currentCetnterLocation.x = COL / 2 - 1;
		newBlocks.currentCetnterLocation.y = ROW - 1;
		break;
	case 2:
		newBlocks = blocksAllType[8];
		newBlocks.currentCetnterLocation.x = COL / 2 - 1;
		newBlocks.currentCetnterLocation.y = ROW - 1;
		break;
	case 3:
		newBlocks = blocksAllType[12];
		newBlocks.currentCetnterLocation.x = COL / 2 - 2;
		newBlocks.currentCetnterLocation.y = ROW - 1;
		break;
	case 4:
		newBlocks = blocksAllType[14];
		newBlocks.currentCetnterLocation.x = COL / 2 - 2;
		newBlocks.currentCetnterLocation.y = ROW - 1;
		break;
	case 5:
		newBlocks = blocksAllType[16];
		newBlocks.currentCetnterLocation.x = COL / 2 - 1;
		newBlocks.currentCetnterLocation.y = ROW - 1;
		break;
	case 6:
		newBlocks = blocksAllType[18];
		newBlocks.currentCetnterLocation.x = COL / 2 - 2;
		newBlocks.currentCetnterLocation.y = ROW - 1;
		break;
	}
	return newBlocks;
}

//ѡ������
void SelectFont(int size, int charset, const char* face) 
{
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}

//����Ӣ����
void DrawENString(const char* str) 
{
	static int isFirstCall = 1;
	static GLuint lists;

	if (isFirstCall) { // ����ǵ�һ�ε��ã�ִ�г�ʼ��
					   // Ϊÿһ��ASCII�ַ�����һ����ʾ�б�
		isFirstCall = 0;

		// ����MAX_CHAR����������ʾ�б���
		lists = glGenLists(MAX_CHAR);

		// ��ÿ���ַ��Ļ������װ����Ӧ����ʾ�б���
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}
	// ����ÿ���ַ���Ӧ����ʾ�б�����ÿ���ַ�
	for (; *str != '\0'; ++str)
		glCallList(lists + *str);
}

//����������
void DrawCNString(const char* str) 
{
	int len, i;
	wchar_t* wstring;
	HDC hDC = wglGetCurrentDC();
	GLuint list = glGenLists(1);

	// �����ַ��ĸ���
	// �����˫�ֽ��ַ��ģ����������ַ����������ֽڲ���һ���ַ�
	// ����һ���ֽ���һ���ַ�
	len = 0;
	for (i = 0; str[i] != '\0'; ++i)
	{
		if (IsDBCSLeadByte(str[i]))
			++i;
		++len;
	}

	// ������ַ�ת��Ϊ���ַ�
	wstring = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, -1, wstring, len);
	wstring[len] = L'\0';

	// �������ַ�
	for (i = 0; i < len; ++i)
	{
		wglUseFontBitmapsW(hDC, wstring[i], 1, list);
		glCallList(list);
	}

	// ����������ʱ��Դ
	free(wstring);
	glDeleteLists(list, 1);
}

//��ʼ����ͼ����
void SetupRC()
{
	//����ɫ
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//���ӿ�
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, COL, 0, ROW);

	//��ʼ����������
	InitBackBlock();
	//��ʼ���������
	srand(time(NULL));
	currentBlocks = CreateBlocks();
	nextBlocks = CreateBlocks();
}

//ʱ�亯��
void TimerFunc(int value)
{
	DownBlocks();
	glutPostRedisplay();
	glutTimerFunc(timeDownMilliSec, TimerFunc, 1);
}

//������ƺ���
void RenderScene()
{
	GLint gapSize = 5;
	GLint boudaryX = winWidth * 2 / 3;
	GLint boudaryY = winHeight / 2;
	char strToShow[MAX_CHAR];
	
	glClear(GL_COLOR_BUFFER_BIT);
	
#pragma region ��Ϸ����
	//��Ϸ������Գ������λ��
	glViewport(0, 0, boudaryX, winHeight);
		
	DrawBlock();
#pragma endregion ��Ϸ����

#pragma region ��Ϣ����
	//�Ҳ���Ϣ������ɫ
	glColor3ub(192, 192, 192);	//��ɫ
	glViewport(boudaryX, 0, winWidth, winHeight);
	glRectd(0, 0, COL, ROW);

	//��һ��������ʾ����λ��
	GLint size = winWidth - boudaryX - gapSize * 2;
	glColor3f(0, 0, 0);
	glViewport(boudaryX + gapSize, boudaryY + gapSize, size, boudaryY - 2 * gapSize);
	glRectd(0, 0, COL, ROW);

	//��һ�������ʾ��ͼ
	SelectFont(18, GB2312_CHARSET, "����_GB2312");
	glColor3f(1, 1, 1);
	glRasterPos2i(0, 17);
	DrawCNString("��һ�����飺");
	//��ʾλ�ý��е���
	DrawNextBlocks();


	//���ֽ��漰һЩ����
	glColor3f(0, 0, 0);
	glViewport(boudaryX + gapSize, gapSize, size, boudaryY - 2 * gapSize);
	glRectd(0, 0, COL, ROW);

	//���֡������仯
	glColor3f(1, 1, 1);
	glRasterPos2i(0, 8);
	DrawCNString("��ǰ�ȼ���");
	_itoa_s(currentLevel, strToShow, 10);
	DrawCNString(strToShow);
	glRasterPos2i(0, 7);
	DrawCNString("��ǰ������");
	_itoa_s(sumScore, strToShow, 10);
	DrawCNString(strToShow);

	//��ʾ����
	glRasterPos2i(0, 18);
	DrawCNString("������ʾ��");
	glRasterPos2i(0, 17);
	DrawCNString("��ʱ��ת����w�������");
	glRasterPos2i(0, 16);
	DrawCNString("�����ƶ���a�������");
	glRasterPos2i(0, 15);
	DrawCNString("�����ƶ���d�������");
	glRasterPos2i(0, 14);
	DrawCNString("Ѹ���½���s�������");
#pragma endregion ��Ϣ����

	glutSwapBuffers();
}

//�Է��������Ӧ
void SpecialKeys(int key, int x, int z)
{
	if (key == GLUT_KEY_UP)
	{
		SwitchBlocks();
		// Refresh the Window
		glutPostRedisplay();
		return;
	}

	if (key == GLUT_KEY_DOWN)
	{
		DownBlocksQuickly();
		// Refresh the Window
		glutPostRedisplay();
		return;
	}

	if (key == GLUT_KEY_LEFT)
	{
		LeftMoveBlocks();
		// Refresh the Window
		glutPostRedisplay();
		return;
	}

	if (key == GLUT_KEY_RIGHT)
	{
		RightMoveBlocks();
		// Refresh the Window
		glutPostRedisplay();
		return;
	}
}

//��wasd������Ӧ
void KeyboardKeys(unsigned char key, int x, int z)
{
	if (key == UP)
	{
		SwitchBlocks();
		// Refresh the Window
		glutPostRedisplay();
		return;
	}

	if (key == DOWN)
	{
		DownBlocksQuickly();
		// Refresh the Window
		glutPostRedisplay();
		return;
	}

	if (key == LEFT)
	{
		LeftMoveBlocks();
		// Refresh the Window
		glutPostRedisplay();
		return;
	}

	if (key == RIGHT)
	{
		RightMoveBlocks();
		// Refresh the Window
		glutPostRedisplay();
		return;
	}
}

//�ı䴰�ڴ�С
void ChangeSize(int w, int h)
{
	// Prevent a divide by zero
	if (h == 0)
		h = 1;

	winWidth = w;
	winHeight = h;
	
	// Set Viewport to window dimensions
	glViewport(0, 0, w, h);
}

int main(int argc, char* argv[])
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowPosition(50, 50);
	glutInitWindowSize(winWidth, winHeight);
	glutCreateWindow("����˹����");
	
	SetupRC();
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKeys);
	glutKeyboardFunc(KeyboardKeys);
	glutDisplayFunc(RenderScene);
	glutTimerFunc(timeDownMilliSec, TimerFunc, 1);

	glutMainLoop();

	return 0;
}