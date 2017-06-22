#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "kbhit.h"

int kbhit(void){
   struct termios oldt, newt;
   int ch;
   int oldf;
   
   tcgetattr(STDIN_FILENO, &oldt);
   newt = oldt;
   newt.c_lflag &= ~(ICANON | ECHO);
   tcsetattr(STDIN_FILENO,TCSANOW, &newt);
   oldf = fcntl(STDIN_FILENO, F_GETFL, 0);
   fcntl(STDIN_FILENO, F_SETFL, oldf | O_NONBLOCK);

   ch = getchar();

   tcsetattr(STDIN_FILENO, TCSANOW, &oldt);
   fcntl(STDIN_FILENO, F_SETFL, oldf);

   if(ch!=EOF){
	ungetc(ch, stdin);
	return 1;
   }

   return 0;
}

int getch(void){
   int ch;

   struct termios old;
   struct termios new;

   tcgetattr(0, &old);

   new = old;
   new.c_lflag &= ~(ICANON|ECHO);
   new.c_cc[VMIN] = 1;
   new.c_cc[VTIME] = 0;

   tcsetattr(0, TCSAFLUSH, &new);
   ch = getchar();
   tcsetattr(0, TCSAFLUSH, &old);
   
   return ch;
}
