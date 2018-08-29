#include <stdio.h>
#include <stdlib.h>
#include <termios.h>
#include <unistd.h>
static struct termios initial_settings, new_settings;
static int peek_character = -1;
void init_keyboard();
void close_keyboard();
int main()
{
init_keyboard();
 int c;
 c=getchar();
 c=getchar();
 c=getchar();
 switch(c)
 {
 	case 'A':
 	c=0x0105;
 	break;
 	case 'B':
 	c=0x0106;
 	break;
 	case 'C':
 	c=0x0107;
 	break;
 	case 'D':
 	c=0x0108;
 	break;
 }
close_keyboard();
exit(0);
}
void init_keyboard()
{
tcgetattr(0,&initial_settings);
new_settings = initial_settings;
new_settings.c_lflag &= ~ICANON;
new_settings.c_lflag &= ~ECHO;
new_settings.c_lflag &= ~ISIG;
new_settings.c_cc[VMIN] = 1;
new_settings.c_cc[VTIME] = 0;
tcsetattr(0, TCSANOW, &new_settings);
}
void close_keyboard()
{
tcsetattr(0, TCSANOW, &initial_settings);
}