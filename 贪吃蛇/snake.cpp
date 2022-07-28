#include <stdio.h>
#include<Windows.h>
#include<conio.h>


enum direction//定义枚举
{
	UP = 1,
	DOWN,
	LEFT,
	RIGHT

};

struct Snake //Snake结构体
{
	int x;    //每节蛇的x坐标
	int y;    //每节蛇的y坐标
}snake[800];

int map[20][40];    //二维数组map[][]，用来表示游戏地图
			//为0时，表示空白，什么都没有
			//为1时，表示上下围墙
			//为2时，表示左右围墙
			//为3时，表示蛇
			//为4时，表示食物

struct States //结构体States存放游戏的参数
{
	int LastDirection = UP;//上一次的位置

	int direction = UP; //位置
	int food_x = 15, food_y = 6; //食物初始位置
	int score = 0;  //游戏得分
}SnakeStates;

void clear(void);	//屏幕初始化
void show(void);    //显示界面
void init(void);    //初始化地图
void move(void);    //响应键盘输入，控制蛇的移动
void ishit(void);	//判断是否判断吃到食物、撞墙和撞到自己
void fail(void);//结束画面

void clear()
{
	HANDLE handle = GetStdHandle(STD_OUTPUT_HANDLE);//获取标准输出设备句柄
	COORD coord; //定义光标位置结构体，其中包含x y坐标两个变量
	coord.X = 0;   //x=0屏幕最上方
	coord.Y = 0;   //y=0屏幕最左边
	SetConsoleCursorPosition(handle, coord);//将光标位置移至屏幕左上角
}


void show()      //显示界面
{
	for (int i = 0; i < 20; i++)              //打印地图
	{
		for (int j = 0; j < 40; j++)
			switch (map[i][j])
			{
			case 0: printf(" "); break;  //0代表空格，什么也不打印
			case 1: printf("-"); break;  //1代表上下围墙
			case 2: printf("|"); break;  //2代表左右围墙
			case 3: printf("$"); break;  //3代表蛇身用@表示
			case 4: printf("*"); break;  //4代表食物用$表示
			}
		printf("\n");
	}
	printf("游戏得分: %d", SnakeStates.score);
}


void init()    //初始化地图
{
	int i, j;
	for (i = 0; i < 20; i++)
		for (j = 0; j < 40; j++)
			map[i][j] = 0;        //初始化二维数组，全部置为0


	for (i = 0; i < 20; i++)
		for (j = 0; j < 40; j++)
		{
			if (i == 0 || i == 19)
				map[i][j] = 1;        //赋值上下围墙 ----------
			if (j == 0 || j == 39)
				map[i][j] = 2;        //赋值左右围墙|||||||||||
		}


	snake[0].x = 10;
	snake[0].y = 20;
	map[snake[0].x][snake[0].y] = 3;         //利用变量在地图中间初始化蛇身
	map[SnakeStates.food_x][SnakeStates.food_y] = 4;                 //初始化食物位置,随便定义了一个位置
	show();
}




void move()     //响应键盘输入，控制蛇的移动
{
	map[snake[SnakeStates.score].x][snake[SnakeStates.score].y] = 0;//将最后一节赋值为0


	if (_kbhit())
	{
		char c;
		c = _getch();
		switch (c)
		{
		case 'w':SnakeStates.direction = 1; break;   //按w键，蛇向上移动
		case 's':SnakeStates.direction = 2; break;   //按s键，蛇向下移动
		case 'a':SnakeStates.direction = 3; break;   //按a键，蛇向左移动
		case 'd':SnakeStates.direction = 4; break;   //按d键，蛇向右移动
		default: break;
		}
	}


	for (int i = SnakeStates.score; i > 0; i--)
	{
		snake[i].x = snake[i - 1].x;
		snake[i].y = snake[i - 1].y;
		map[snake[i].x][snake[i].y] = 3;
	}


	switch (SnakeStates.direction)    //根据direction值，改变蛇的坐标
	{
		case UP:
		{
			if (SnakeStates.LastDirection == DOWN) {
				SnakeStates.direction = DOWN;
				snake[0].x++;
				break;
			}
			snake[0].x--;
			SnakeStates.LastDirection = SnakeStates.direction;
			break;
		}
		case DOWN:
		{
			if (SnakeStates.LastDirection == UP) {
				SnakeStates.direction = UP;
				snake[0].x--;
				break;
			}
			snake[0].x++;
			SnakeStates.LastDirection = SnakeStates.direction;
			break;
		}
		case LEFT:
		{
			if (SnakeStates.LastDirection == RIGHT) {
				SnakeStates.direction = RIGHT;
				snake[0].y++;
				break;
			}
			snake[0].y--;

			SnakeStates.LastDirection = SnakeStates.direction;
			break;
		}
		case RIGHT:
		{
			if (SnakeStates.LastDirection == LEFT) {
				SnakeStates.direction = LEFT;
				snake[0].y--;
				break;
			}
			snake[0].y++;
			SnakeStates.LastDirection = SnakeStates.direction;
			break;
		}
	default:break;
	}


	map[snake[0].x][snake[0].y] = 3;  //更新地图，将蛇的新位置赋值为3
}


void ishit()
{
	if (snake[0].x == SnakeStates.food_x && snake[0].y == SnakeStates.food_y)//判断蛇头位置是否与食物位置相重合
	{
		SnakeStates.score++;    //得分加1
		//如果随机生成的食物位置与围墙或蛇体或墙体重合，则循环再次随机生成食物位置
		while (map[SnakeStates.food_x][SnakeStates.food_y] != 0)
		{
			SnakeStates.food_x = rand() % 19 + 1;      //随机在1-19的位置产生食物
			SnakeStates.food_y = rand() % 39 + 1;      //随机在1-39的位置产生食物
		}
		map[SnakeStates.food_x][SnakeStates.food_y] = 4;   //在地图上显示食物
	}
	//判断蛇头位置是否与墙的位置重合
	if (snake[0].x == 0 || snake[0].x == 19 || snake[0].y == 0 || snake[0].y == 39)
	{
		fail();          //退出程序
		system("pause");  //暂停程序
	}
	//判断蛇头位置是否与身体的位置重合
	for (int i=0;i<800;i++) 
	{
		if (snake[i+1].y == 0 && snake[i+1].x == 0)
			break;
		if (snake[i+1].x == snake[0].x && snake[i+1].y == snake[0].y)
		{
			fail();          //退出程序
			system("pause");  //暂停程序
		}
	}
	//if (map[snake[0].x + 1][snake[0].y + 1] == 3)
	//{
	//	system("pause");  //暂停程序
	//	exit(0);          //退出程序
	//}
}
void fail(void)
{
	Sleep(400);
	system("cls");//清屏
	
		printf("\n\n\n\n\n\n\n");

	printf("你死了，得分: %d\n\n\n", SnakeStates.score);
	printf("  生的终止不过一场死亡，死的意义不过在于重生或永眠，死亡不是失去生命，而是走出时间。\n               --余华 《活着》");
	while (true);
}

int main()
{
	init();
	while (1)
	{
		clear();        //调用自定义的清屏函数
		move();
		show();
		ishit();
		Sleep(150);     //延时函数，调整程序运行速度(蛇的移动速度)
	}
}
