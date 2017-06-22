#include <termios.h>
#include <unistd.h>
#include <fcntl.h>
#include "kbhit.h"

/**
 * @file kbhit.c
 */



/**	@fn kbhit
 	@param void
	@brief recognition for keyboard hit
	@return 1 is returned when a keyboard input is reconized.
*/

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


/**	@fn getch
	@brief when user inputs some data through keyboard, the data is not saved in buffer.
	*/


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
