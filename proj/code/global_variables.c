#include "global_variables.h"

char OPTION = MAIN_MENU;

int radius = 10;

char tool = PENCIL;

int colour = 0x001F;

int RTC_COUNTER = 60; //começa a 60

char name[11];

char word[11]; //nao tenho a certeza se isto ja nao e o mesmo que name[11], se for nao esquecer de mudar int score()

int tries = 0; //começa a 0 ||| MAX = 4 (incluindo)

scores_t top_highscores;

int screen_abs = 0; //começa a 0 ||| MAX = 9 (incluindo)

int screen_current = 0; //começa a 0 ||| MAX = 9 (incluindo)
int espaco = 0;
int length = 0;
int length_word = 0;
int contador_high = 0;
int tentativas = 0;
int ai_or_pvp = 0; // 0 = ai; 1 = pvp

/*
 * NAO ESTA FEITO!! O SCORE NAO TEM O CASO QUE ACERTA (TEM QUE SER FEITO NO FINAL)
 * score = 4 tentativas * 100 (perde)
 * score = numero de letras * 150 (ganha)
 */

/*
 * Kengas ve o undo na parte do drawBitmap
 */
