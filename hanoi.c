#include <stdio.h>
#include <stdlib.h>
#if _WIN32
	#include <conio.h>
	#define CLEAR() system("cls")
#else
	#include <termios.h>
	#define CLEAR() system("clear")

	int getch()
	{
    		int cr;
    		struct termios nts, ots;

    		if (tcgetattr(0, &ots) < 0) // �õ���ǰ�ն�(0��ʾ��׼����)������
        		return EOF;
    		nts = ots;
    		cfmakeraw(&nts); // �����ն�ΪRawԭʼģʽ����ģʽ�����е������������ֽ�Ϊ��λ������
    		if (tcsetattr(0, TCSANOW, &nts) < 0) // �����ϸ���֮�������
        		return EOF;

    		cr = getchar();
    		if (tcsetattr(0, TCSANOW, &ots) < 0) // ���û�ԭ���ϵ�ģʽ
        		return EOF;

    		return cr;
	}
#endif

#define BLANK ' '
#define COLUMN '|'
#define PLATE '#'
#define OPT 'v'

#define KEY_UP 'w' : case 'W'
#define KEY_DOWN 's' : case 'S'
#define KEY_LEFT 'a' : case 'A'
#define KEY_RIGHT 'd' : case 'D'
#define KEY_QUIT 'q' : case 'Q'

#define MAX_SIZE 10

struct column
{
	int col[MAX_SIZE];
	int size;
}column[5][3] = { 

{
	{ { 4, 3, 2, 1, 0 }, 4 },
	{ { 0 }, 0 },
	{ { 0 }, 0 }
},
{
	{ { 5, 4, 3, 2, 1, 0 }, 5 },
	{ { 0 }, 0 },
	{ { 0 }, 0 }
},
{
	{ { 6, 5, 4, 3, 2, 1, 0 }, 6 },
	{ { 0 }, 0 },
	{ { 0 }, 0 }
},
{
	{ { 7, 6, 5, 4, 3, 2, 1, 0 }, 7 },
	{ { 0 }, 0 },
	{ { 0 }, 0 }
},
{
	{ { 8, 7, 6, 5, 4, 3, 2, 1, 0 }, 8 },
	{ { 0 }, 0 },
	{ { 0 }, 0 }
}

};

const char *diff_str[5] = {"�ǳ���", "��", "����", "����", "�ǳ�����"};

enum diff
{
	VERY_EASY,
	EASY,
	NORMAL,
	HARD,
	VERY_HARD
};

enum opt
{
	LEFT,
	CENTRAL,
	RIGHT
};

int column_temp = 0;
int step_count = 0;
int opt = LEFT;
int diff;


int isEmpty(int opt)
{
	return !column[diff][opt].size;
}

int isCompleted()
{
	return isEmpty(LEFT) && isEmpty(CENTRAL) && !column_temp;
}

void pop()
{
	column_temp = column[diff][opt].col[column[diff][opt].size - 1];
	column[diff][opt].col[--column[diff][opt].size] = 0;
}

void push()
{
	column[diff][opt].col[column[diff][opt].size++] = column_temp;
	column_temp = 0;
}

void menu()
{
	CLEAR();
	printf("==============================  �� ŵ ��  ==============================\n\n");
	printf("�밴��Ӧ�����ּ�ѡ����Ϸ�Ѷȣ��� Q ���˳���Ϸ: \n\n");
	printf("1. �ǳ���\n2. ��\n3. ����\n4. ����\n5. �ǳ�����\n");
	
	switch(getch())
	{
		case '1': diff = VERY_EASY; break;
		case '2': diff = EASY; break;
		case '3': diff = NORMAL; break;
		case '4': diff = HARD; break;
		case '5': diff = VERY_HARD; break;
		case KEY_QUIT: CLEAR(); exit(0);
		default:
		menu();
	}
}

void display()
{
	CLEAR();
	printf("==============================  �� ŵ ��  ==============================\n");
	printf("����: W,A,S,D        �˳�: Q        �Ѷ�: %s        ����: %d\n\n", diff_str[diff], step_count);
	int i, j;

	//opt
	if(opt == CENTRAL)
		//25 ��
		printf("                         ");
	else if(opt == RIGHT)
		//50 ��
		printf("                                                  ");

	for(i = 0; i < 12 - column_temp; i++)
	{
		putchar(BLANK);
	}

	for(i = 0; i < column_temp; i++)
	{
		putchar(PLATE);
	}

	putchar(column_temp ? PLATE : OPT);

	for(i = 0; i < column_temp; i++)
	{
		putchar(PLATE);
	}

	printf("\n\n");

	for(i = MAX_SIZE - 1; i >= 0; i--)
	{
		//left
		for(j = 0; j < 12 - column[diff][LEFT].col[i]; j++)
		{
			putchar(BLANK);
		}

		for(j = 0; j < column[diff][LEFT].col[i]; j++)
		{
			putchar(PLATE);
		}

		putchar(column[diff][LEFT].col[i] ? PLATE : COLUMN);

		for(j = 0; j < column[diff][LEFT].col[i]; j++)
		{
			putchar(PLATE);
		}

		//central
		for(j = 0; j < 24 - column[diff][LEFT].col[i] - column[diff][CENTRAL].col[i]; j++)
		{
			putchar(BLANK);
		}

		for(j = 0; j < column[diff][CENTRAL].col[i]; j++)
		{
			putchar(PLATE);
		}

		putchar(column[diff][CENTRAL].col[i] ? PLATE : COLUMN);

		for(j = 0; j < column[diff][CENTRAL].col[i]; j++)
		{
			putchar(PLATE);
		}

		//right
		for(j = 0; j < 24 - column[diff][CENTRAL].col[i] - column[diff][RIGHT].col[i]; j++)
		{
			putchar(BLANK);
		}

		for(j = 0; j < column[diff][RIGHT].col[i]; j++)
		{
			putchar(PLATE);
		}

		putchar(column[diff][RIGHT].col[i] ? PLATE : COLUMN);

		for(j = 0; j < column[diff][RIGHT].col[i]; j++)
		{
			putchar(PLATE);
		}

		putchar('\n');
	}
}

void mainLoop()
{
	while(1)
	{
		display();

		switch(getch())
		{
			case KEY_UP:
				if(!column_temp && !isEmpty(opt))
				{
					step_count++;
					pop();
				}
				break;
				
			case KEY_DOWN:
			
				if(isEmpty(opt) || (column_temp && column[diff][opt].col[column[diff][opt].size - 1] > column_temp))
				{
					step_count++;
					push();
				}
				break;
				
			case KEY_LEFT:
				if(opt == LEFT)
					break;
				step_count++;
				opt--;
				break;
				
			case KEY_RIGHT:
				if(opt == RIGHT)
					break;
				step_count++;
				opt++;
				break;
				
			case KEY_QUIT:
				CLEAR();
				exit(0);
		}

		if(isCompleted())
		{
			//���һ����ʾˢ��
			display();
			printf("\n��ϲ���أ���һ��ʹ�� %d ��������������\n", step_count);
			printf("��������˳�...\n");
			getch();
			break;
		}
	}
}

int main(int argc, char **argv)
{
	menu();
	mainLoop();
	return 0;
}
