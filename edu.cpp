#include <Arduino.h>

#define roda_esq_m1 19
#define roda_esq_m2 18
#define roda_dir_m1 23
#define roda_dir_m2 4
#include "motores.h"

#define sensor_esq 32
#define sensor_dir 26
#define sensor_frente_esq  27
#define sensor_frente_dir  26
#include "sensores.h"

void init_edu(unsigned int baud = 9600) {
    init_motores();
    init_sensores();

    Serial.begin(baud);
}