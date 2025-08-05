#include <inttypes.h>

void init_edu(unsigned int baud = 9600);

unsigned int dist_frente(void);
float dist_esq(void);
float dist_dir(void);

void mover(int16_t esq, int16_t dir);
