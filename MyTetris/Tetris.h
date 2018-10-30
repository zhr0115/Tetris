#include <gl/glut.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

#define LEFT	'a'
#define RIGHT	'd'
#define UP		'w'
#define DOWN	's'

#define RED RGB(255,0,0)
#define ORANGE RGB(250,128,10)
#define YELLOW RGB(255,255,0)
#define GREEN RGB(0,255,0)
#define CYAN RGB(0,255,255)
#define LIGHT_BLUE RGB(0xA6,0xCA,0xF0) //天蓝色
#define PURPLE RGB(255,0,255)
#define BLACK RGB(0,0,0)

#define ROW	20
#define COL	10

#define MAX_CHAR 128

GLint winWidth = 600;
GLint winHeight = 800;

GLint timeDownMilliSec = 500;
GLint currentLevel = 1;
GLint sumScore = 0;

struct wcPt2D
{
	GLint x;
	GLint y;
};

struct BLOCK
{
	bool isBackBlock;
	GLint color;
};

//存储所有方块信息
struct BLOCK block[COL][ROW + 3];

struct BLOCKS
{
	wcPt2D currentCetnterLocation;
	wcPt2D blockRelativePos[4];
	GLint blockColor;
	GLint nextBlocksType;
};

//记录当前方块的信息
BLOCKS currentBlocks;
//记录下一个方块的信息
BLOCKS nextBlocks;

//存储所有19种类型方块的相对位置
BLOCKS blocksAllType[19] =
{  //初始化各个游戏方块
	{ 0,0,1,1,1,2,1,3,2,3,RED,1 },
	{ 0,0,0,2,1,2,2,2,0,3,RED,2 },
	{ 0,0,0,1,1,1,1,2,1,3,RED,3 },
	{ 0,0,2,1,0,2,1,2,2,2,RED,0 },
	{ 0,0,1,1,1,2,0,3,1,3,ORANGE,5 },
	{ 0,0,0,1,0,2,1,2,2,2,ORANGE,6 },
	{ 0,0,1,1,2,1,1,2,1,3,ORANGE,7 },
	{ 0,0,0,2,1,2,2,2,2,3,ORANGE,4 },
	{ 0,0,1,1,0,2,1,2,2,2,YELLOW,9 },
	{ 0,0,1,1,1,2,2,2,1,3,YELLOW,10 },
	{ 0,0,0,2,1,2,2,2,1,3,YELLOW,11 },
	{ 0,0,1,1,0,2,1,2,1,3,YELLOW,8 },
	{ 0,0,1,1,1,2,2,2,2,3,GREEN,13 },
	{ 0,0,1,2,2,2,0,3,1,3,GREEN,12 },
	{ 0,0,2,1,1,2,2,2,1,3,CYAN,15 },
	{ 0,0,0,2,1,2,1,3,2,3,CYAN,14 },
	{ 0,0,1,0,1,1,1,2,1,3,LIGHT_BLUE,17 },
	{ 0,0,0,2,1,2,2,2,3,2,LIGHT_BLUE,16 },
	{ 0,0,1,1,2,1,1,2,2,2,PURPLE,18 },
};

void InitBackBlock();
void DrawBlock();
void DrawNextBlocks();
void DeleteBlock(GLint i);
void BlocksToBackBlock();
void BackBlockToBlocks();
bool ConflictCheck(BLOCKS curBlocks);
void SwitchBlocks();
void DownBlocks();
void DownBlocksQuickly();
void LeftMoveBlocks();
void RightMoveBlocks();
BLOCKS CreateBlocks();

void SelectFont(int size, int charset, const char* face);
void DrawENString(const char* str);
void DrawCNString(const char* str);

void SetupRC();
void TimerFunc(int value);
void RenderScene();
void SpecialKeys(int key, int x, int z);
void KeyboardKeys(unsigned char key, int x, int z);
void ChangeSize(int w, int h);
int main(int argc, char* argv[]);
