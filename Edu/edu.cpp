#include <Arduino.h>

#define roda_esq_m1 9
#define roda_esq_m2 10
#define roda_dir_m1 5
#define roda_dir_m2 6
#include "motores.h"

#define sensor_esq A3
#define sensor_dir A4
#define TRIG A1
#define ECHO A2
#include "sensores.h"

void init_edu(unsigned int baud = 9600) {
    init_motores();
    init_sensores();

    Serial.begin(baud);
}