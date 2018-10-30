#include "Tetris.h"
#pragma comment( linker, "/subsystem:\"windows\" /entry:\"mainCRTStartup\"" )

//初始化所有方块
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

//根据方块信息绘制所有方块
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

//绘制下一个方块提示
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

//删除从下往上数第i+1行
void DeleteBlock(GLint i)
{
	for (GLint k = 0; k < COL; k++)
	{
		for (GLint j = i; j < ROW; j++)
		{
			//block[][i]被清理
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

//检查是否有可消除的，若有则消除
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

//将方块变回背景
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

//将背景改成方块
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

//与已有方块或者边界碰撞检测
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
				//有方块超左右边界
				return true;
			}
			return false;
		}
		else
		{
			if (!block[x][y].isBackBlock)
			{
				//当前位置已有方块
				return true;
			}
			if (x<0 || x>COL)
			{
				//有方块超左右边界
				return true;
			}
			if (y < 0)
			{
				//有方块超下边界
				return true;
			}
		}
	}
	return false;
}

//改变方块类型,逆时针
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

//方块下落一格
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
		//下落结束,生成新的方块
		BackBlockToBlocks();
		DeleteCheck();

		//游戏结束判断
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

//方块迅速下降
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
		//下落结束,生成新的方块
		BackBlockToBlocks();
		DeleteCheck();

		//游戏结束判断
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

//方块左移一格
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

//方块右移一格
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

//生成新的方块
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

//选择字体
void SelectFont(int size, int charset, const char* face) 
{
	HFONT hFont = CreateFontA(size, 0, 0, 0, FW_MEDIUM, 0, 0, 0,
		charset, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS,
		DEFAULT_QUALITY, DEFAULT_PITCH | FF_SWISS, face);
	HFONT hOldFont = (HFONT)SelectObject(wglGetCurrentDC(), hFont);
	DeleteObject(hOldFont);
}

//绘制英文字
void DrawENString(const char* str) 
{
	static int isFirstCall = 1;
	static GLuint lists;

	if (isFirstCall) { // 如果是第一次调用，执行初始化
					   // 为每一个ASCII字符产生一个显示列表
		isFirstCall = 0;

		// 申请MAX_CHAR个连续的显示列表编号
		lists = glGenLists(MAX_CHAR);

		// 把每个字符的绘制命令都装到对应的显示列表中
		wglUseFontBitmaps(wglGetCurrentDC(), 0, MAX_CHAR, lists);
	}
	// 调用每个字符对应的显示列表，绘制每个字符
	for (; *str != '\0'; ++str)
		glCallList(lists + *str);
}

//绘制中文字
void DrawCNString(const char* str) 
{
	int len, i;
	wchar_t* wstring;
	HDC hDC = wglGetCurrentDC();
	GLuint list = glGenLists(1);

	// 计算字符的个数
	// 如果是双字节字符的（比如中文字符），两个字节才算一个字符
	// 否则一个字节算一个字符
	len = 0;
	for (i = 0; str[i] != '\0'; ++i)
	{
		if (IsDBCSLeadByte(str[i]))
			++i;
		++len;
	}

	// 将混合字符转化为宽字符
	wstring = (wchar_t*)malloc((len + 1) * sizeof(wchar_t));
	MultiByteToWideChar(CP_ACP, MB_PRECOMPOSED, str, -1, wstring, len);
	wstring[len] = L'\0';

	// 逐个输出字符
	for (i = 0; i < len; ++i)
	{
		wglUseFontBitmapsW(hDC, wstring[i], 1, list);
		glCallList(list);
	}

	// 回收所有临时资源
	free(wstring);
	glDeleteLists(list, 1);
}

//初始化绘图界面
void SetupRC()
{
	//背景色
	glClearColor(0.0f, 0.0f, 0.0f, 1.0f);

	//调视口
	glMatrixMode(GL_PROJECTION);
	gluOrtho2D(0, COL, 0, ROW);

	//初始化背景方块
	InitBackBlock();
	//初始化随机种子
	srand(time(NULL));
	currentBlocks = CreateBlocks();
	nextBlocks = CreateBlocks();
}

//时间函数
void TimerFunc(int value)
{
	DownBlocks();
	glutPostRedisplay();
	glutTimerFunc(timeDownMilliSec, TimerFunc, 1);
}

//整体绘制函数
void RenderScene()
{
	GLint gapSize = 5;
	GLint boudaryX = winWidth * 2 / 3;
	GLint boudaryY = winHeight / 2;
	char strToShow[MAX_CHAR];
	
	glClear(GL_COLOR_BUFFER_BIT);
	
#pragma region 游戏区域
	//游戏界面相对程序界面位置
	glViewport(0, 0, boudaryX, winHeight);
		
	DrawBlock();
#pragma endregion 游戏区域

#pragma region 信息区域
	//右侧信息区背景色
	glColor3ub(192, 192, 192);	//银色
	glViewport(boudaryX, 0, winWidth, winHeight);
	glRectd(0, 0, COL, ROW);

	//下一个方块提示界面位置
	GLint size = winWidth - boudaryX - gapSize * 2;
	glColor3f(0, 0, 0);
	glViewport(boudaryX + gapSize, boudaryY + gapSize, size, boudaryY - 2 * gapSize);
	glRectd(0, 0, COL, ROW);

	//下一个方块的示意图
	SelectFont(18, GB2312_CHARSET, "楷体_GB2312");
	glColor3f(1, 1, 1);
	glRasterPos2i(0, 17);
	DrawCNString("下一个方块：");
	//显示位置进行调整
	DrawNextBlocks();


	//积分界面及一些参数
	glColor3f(0, 0, 0);
	glViewport(boudaryX + gapSize, gapSize, size, boudaryY - 2 * gapSize);
	glRectd(0, 0, COL, ROW);

	//积分、参数变化
	glColor3f(1, 1, 1);
	glRasterPos2i(0, 8);
	DrawCNString("当前等级：");
	_itoa_s(currentLevel, strToShow, 10);
	DrawCNString(strToShow);
	glRasterPos2i(0, 7);
	DrawCNString("当前分数：");
	_itoa_s(sumScore, strToShow, 10);
	DrawCNString(strToShow);

	//显示帮助
	glRasterPos2i(0, 18);
	DrawCNString("按键提示：");
	glRasterPos2i(0, 17);
	DrawCNString("逆时针转动：w键或↑键");
	glRasterPos2i(0, 16);
	DrawCNString("向左移动：a键或←键");
	glRasterPos2i(0, 15);
	DrawCNString("向右移动：d键或→键");
	glRasterPos2i(0, 14);
	DrawCNString("迅速下降：s键或↓键");
#pragma endregion 信息区域

	glutSwapBuffers();
}

//对方向键的响应
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

//对wasd键的响应
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

//改变窗口大小
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
	glutCreateWindow("俄罗斯方块");
	
	SetupRC();
	glutReshapeFunc(ChangeSize);
	glutSpecialFunc(SpecialKeys);
	glutKeyboardFunc(KeyboardKeys);
	glutDisplayFunc(RenderScene);
	glutTimerFunc(timeDownMilliSec, TimerFunc, 1);

	glutMainLoop();

	return 0;
}