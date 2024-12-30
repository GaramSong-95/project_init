#include "device_driver.h"

#define LCDW			(320)
#define LCDH			(240)
#define X_MIN	 		(0)
#define X_MAX	 		(LCDW - 1)
#define Y_MIN	 		(0)
#define Y_MAX	 		(LCDH - 1)

#define TIMER_PERIOD	(10)
#define RIGHT			(1)
#define LEFT			(-1)
#define HOME			(0)
#define SCHOOL			(1)

#define CAR_STEP		(10)
#define CAR_SIZE_X		(20)
#define CAR_SIZE_Y		(20)
#define FROG_STEP		(10)
#define FROG_SIZE_X		(20)
#define FROG_SIZE_Y		(20)
#define FROG_LOCATION_X (150)
#define FROG_LOCATION_Y	(220)



#define BACK_COLOR		(5)
#define CAR_COLOR		(0)
#define FROG_COLOR		(1)
#define Bullet_Step     (20)

#define GAME_OVER		(1)
#define Destroyed       (1)


class Object
{
	public:

	int x;
	int y;
	unsigned short color;
	int size_w;
	int size_h;

	void Draw()
	{
		Lcd_Draw_Box(x, y, size_w, size_h, color);
	}

};

class C_frog : public Object
{
	public:

	int dir;
	int score;
	unsigned short Collision_state;
	int POWER_STEP;
	int Count_Step;
	int Power_state;
	int num;

	void Move(int key)
	{
		if(Count_Step==3) 
		{
			Power_state=0;
			Count_Step=0;
			POWER_STEP=0;
		}
		switch (key)
		{
		case 0:
		if((y<FROG_STEP+POWER_STEP)&&(y>Y_MIN)) y = Y_MIN;
		else if(y > Y_MIN) y -= FROG_STEP+POWER_STEP;
		if(Power_state) Count_Step+=1;
		break;
		case 1:
		if((y+size_h<Y_MAX)&&(y+size_h>Y_MAX-(FROG_STEP+POWER_STEP))) y = FROG_LOCATION_Y;
		else if(y + size_h < Y_MAX) y += FROG_STEP+POWER_STEP;
		if(Power_state) Count_Step+=1;
		break;
		case 2:
		if((x>X_MIN)&&(x<FROG_STEP+POWER_STEP)) x = X_MIN;
		else if(x > X_MIN) x -= FROG_STEP+POWER_STEP;
		if(Power_state) Count_Step+=1;
		break;
		case 3:
		if((x+size_w<X_MAX)&&(x+size_w>X_MAX-(FROG_STEP+POWER_STEP))) x = X_MAX - FROG_SIZE_X;
		else if(x + size_w < X_MAX) x += FROG_STEP+POWER_STEP;
		if(Power_state) Count_Step+=1;
		break;
		case 4:
		if(num==3) break;
		Power_state=1;
		POWER_STEP = 10;
		num++;
		break;
		case 5:
		break;

		
		default:
		break;
		}
	}
	int Check_Collision(int Car_x, int Car_y)
	{
		if((Car_x >= x) && ((x + FROG_SIZE_X) >= Car_x)) Collision_state |= 1<<0;
	 	else if((Car_x <= x) && ((Car_x + FROG_SIZE_X) >= x)) Collision_state |= 1<<0;
	
		if((Car_y >= y) && ((y + FROG_SIZE_X) >= Car_y)) Collision_state |= 1<<1;
	 	else if((Car_y <= y) && ((Car_y + FROG_SIZE_X) >= y)) Collision_state |= 1<<1;

		if(Collision_state==3) 
		{
			Collision_state = 0;
			Uart_Printf("SCORE = %d\n", score);
			return GAME_OVER;
		}
		if((dir==SCHOOL)&&(y==Y_MIN)) 
		{
			dir=HOME;
			Uart_Printf("SCHOOL\n");
			
		}
		else if((dir==HOME)&&(y>=FROG_LOCATION_Y)) 
		{
			score++;
			Uart_Printf("HOME %d\n",score);
			dir=SCHOOL;
			
		}

		Collision_state = 0;
		return 0;
	}
};




class C_car : public Object
{
	public:
	
	int dir;
	
	void Move(int speed=0)
	{
		if(x>=LCDW-CAR_SIZE_X) dir=LEFT;
		else if(x<=X_MIN) dir=RIGHT;

		if((x<LCDW-CAR_SIZE_X)&&(dir==RIGHT)) x += CAR_STEP+speed;
		else if((x>X_MIN)&&(dir==LEFT)) x -= CAR_STEP+speed;
	}
	
};

static unsigned short color[] = {RED, YELLOW, GREEN, BLUE, WHITE, BLACK};
static C_car Car[4];
static C_frog frog1;
static unsigned int Stage;
static int Car_Speed[3] = {0,3,5};


static void Game_Init()
{
	Lcd_Clr_Screen();
	frog1.x=FROG_LOCATION_X;
	frog1.y=FROG_LOCATION_Y;
	frog1.size_h=FROG_SIZE_Y;
	frog1.size_w=FROG_SIZE_X;
	frog1.color=color[3];
	frog1.score=0;
	frog1.dir=SCHOOL;
	frog1.Collision_state = 0;
	frog1.Power_state=0;
	frog1.POWER_STEP=0;
	frog1.Count_Step=0;
	frog1.num=0;
	Car[0].x=0;
	Car[0].y=Y_MAX/2;
	Car[0].color=color[0];
	Car[0].size_h=CAR_SIZE_X;
	Car[0].size_w=CAR_SIZE_Y;
	Car[0].dir = RIGHT;
	Stage = 0;
	frog1.Draw();
	Car[0].Draw();


}
static void Car_Creation(int i,int y=0,int x=0)
{
	Car[i].x=x;
	Car[i].y=y;
	Car[i].color=color[i];
	Car[i].size_h=CAR_SIZE_X;
	Car[i].size_w=CAR_SIZE_Y;
	Car[i].dir = RIGHT;
}



extern volatile int TIM4_expired;
extern volatile int USART1_rx_ready;
extern volatile int USART1_rx_data;
extern volatile int Jog_key_in;
extern volatile int Jog_key;

extern "C" void abort(void)
{
  while (1);
}

static void Sys_Init(void)
{
	Clock_Init();
	LED_Init();
	Uart_Init(115200);

	SCB->VTOR = 0x08003000;
	SCB->SHCSR = 7<<16;	
}

extern "C" void Main()
{
	Sys_Init();
	Uart_Printf("Game Example\n");

	Lcd_Init();
	Jog_Poll_Init();
	Jog_ISR_Enable(1);
	Uart1_RX_Interrupt_Enable(1);

	for(;;)
	{
		Game_Init();
		TIM4_Repeat_Interrupt_Enable(1, TIMER_PERIOD*10);
		int game_state = 0;
		int game_over = 0;

		for(;;)
		{
			
			
			

			switch(Stage)
			{
				case 0:
				game_state=0;
				if(frog1.score==1) 
				{
					Car_Creation(1,Y_MAX/2-30);
					Stage = 1;
					
				}
				break;
				case 1:
				game_state=1;
				if(frog1.score==2)
				{
					Car_Creation(2,Y_MAX/2-60);
					Stage = 2;
					
				}
				break;
				case 2:
				game_state=2;
				if(frog1.score==3)
				{
					Stage = 3;
					Car_Speed[0]++;
					Car_Speed[1]++;
					Car_Speed[2]++;
				}
				break;
				case 3:
				game_state=2;
				if(frog1.score==4)
				{
					Stage = 4;
					Car_Speed[0]++;
					Car_Speed[1]++;
					Car_Speed[2]++;
				}
				break;
				case 4:
				game_state=2;
				if(frog1.score==5)
				{
					Stage = 5;
					Car_Speed[0]+=2;
					Car_Speed[1]+=2;
					Car_Speed[2]+=2;
				}
				break;
				case 5:
				game_state=2;
				
				break;
			}

			if(Jog_key_in) 
			{
				Uart_Printf("KEY %d\n",Jog_key);
				frog1.color=color[5];
				frog1.Draw();
				frog1.Move(Jog_key);
				for(int i=0;i<game_state+1;i++)
				{
				   	game_over = frog1.Check_Collision(Car[i].x,Car[i].y);
					if(game_over) break;
				}
				frog1.color=color[3];
				frog1.Draw();
				Jog_key_in = 0;				
			}

			if(TIM4_expired) 
			{
				
				for(int i=0;i<game_state+1;i++)
				{
				Car[i].color=color[5];
				Car[i].Draw();
				Car[i].Move(Car_Speed[i]);
				game_over = frog1.Check_Collision(Car[i].x,Car[i].y);
				Car[i].color = color[i];
				Car[i].Draw();
				if(game_over) break;
				}
				TIM4_expired = 0;
			}

			if(game_over)
			{
				TIM4_Repeat_Interrupt_Enable(0, 0);
				Uart_Printf("Game Over, Please press any key to continue.\n");
				Jog_Wait_Key_Pressed();
				Jog_Wait_Key_Released();
				Uart_Printf("Game Start\n");
				Stage = 0;
				break;
			}



			// if(Jog_key_in) 
			// {
			// 	Uart_Printf("KEY\n");
			// 	frog.ci = BACK_COLOR;
			// 	Draw_Object(&frog);
			// 	game_over = Frog_Move(Jog_key);
			// 	frog.ci = FROG_COLOR;
			// 	Draw_Object(&frog);
			// 	Jog_key_in = 0;				
			// }

			// if(TIM4_expired) 
			// {
			// 	car.ci = BACK_COLOR;
			// 	Draw_Object(&car);
			// 	game_over = Car_Move();
			// 	car.ci = CAR_COLOR;
			// 	Draw_Object(&car);
			// 	TIM4_expired = 0;
			// }

			// if(game_over)
			// {
			// 	TIM4_Repeat_Interrupt_Enable(0, 0);
			// 	Uart_Printf("Game Over, Please press any key to continue.\n");
			// 	Jog_Wait_Key_Pressed();
			// 	Jog_Wait_Key_Released();
			// 	Uart_Printf("Game Start\n");
			// 	break;
			// }
		}
	}
}



// typedef struct
// {
// 	int x,y;
// 	int w,h;
// 	int ci;
// 	int dir;
// }QUERY_DRAW;

// static QUERY_DRAW car;
// static QUERY_DRAW frog;

// static int score;
// static unsigned short color[] = {RED, YELLOW, GREEN, BLUE, WHITE, BLACK};

// static int Check_Collision(void)
// {
// 	int col = 0;

// 	if((car.x >= frog.x) && ((frog.x + FROG_STEP) >= car.x)) col |= 1<<0;
// 	else if((car.x < frog.x) && ((car.x + CAR_STEP) >= frog.x)) col |= 1<<0;
	
// 	if((car.y >= frog.y) && ((frog.y + FROG_STEP) >= car.y)) col |= 1<<1;
// 	else if((car.y < frog.y) && ((car.y + CAR_STEP) >= frog.y)) col |= 1<<1;

// 	if(col == 3)
// 	{
// 		Uart_Printf("SCORE = %d\n", score);	
// 		return GAME_OVER;
// 	}

// 	if((frog.dir == SCHOOL) && (frog.y == Y_MIN)) 
// 	{
// 		Uart_Printf("SCHOOL\n");		
// 		frog.dir = HOME;
// 	}

// 	if((frog.dir == HOME) && (frog.y == LCDH - frog.h))
// 	{
// 		frog.dir = SCHOOL;
// 		score++;
// 		Uart_Printf("HOME, %d\n", score);
// 	}

// 	return 0;
// }

// static int Car_Move(void)
// {
// 	car.x += CAR_STEP * car.dir;
// 	if((car.x + car.w >= X_MAX) || (car.x <= X_MIN)) car.dir = -car.dir;
// 	return Check_Collision();
// }

// static void k0(void)
// {
// 	if(frog.y > Y_MIN) frog.y -= FROG_STEP;
// }

// static void k1(void)
// {
// 	if(frog.y + frog.h < Y_MAX) frog.y += FROG_STEP;
// }

// static void k2(void)
// {
// 	if(frog.x > X_MIN) frog.x -= FROG_STEP;
// }

// static void k3(void)
// {
// 	if(frog.x + frog.w < X_MAX) frog.x += FROG_STEP;
// }

// static int Frog_Move(int k)
// {
// 	// UP(0), DOWN(1), LEFT(2), RIGHT(3)
// 	static void (*key_func[])(void) = {k0, k1, k2, k3};
// 	if(k <= 3) key_func[k]();
// 	return Check_Collision();
// }

// static void Game_Init(void)
// {
// 	score = 0;
// 	Lcd_Clr_Screen();
// 	frog.x = 150; frog.y = 220; frog.w = FROG_SIZE_X; frog.h = FROG_SIZE_Y; frog.ci = FROG_COLOR; frog.dir = SCHOOL;
// 	car.x = 0; car.y = 110; car.w = CAR_SIZE_X; car.h = CAR_SIZE_Y; car.ci = CAR_COLOR; car.dir = RIGHT;
// 	Lcd_Draw_Box(frog.x, frog.y, frog.w, frog.h, color[frog.ci]);
// 	Lcd_Draw_Box(car.x, car.y, car.w, car.h, color[car.ci]);
// }

// static void Draw_Object(QUERY_DRAW * obj)
// {
// 	Lcd_Draw_Box(obj->x, obj->y, obj->w, obj->h, color[obj->ci]);
// }




