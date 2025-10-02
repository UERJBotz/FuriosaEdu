#include <inttypes.h>

void init_edu(unsigned int baud);

bool dist_frente_esq(void);
bool dist_frente_dir(void);
bool dist_esq(void);
bool dist_dir(void);

void mover(int16_t esq, int16_t dir);