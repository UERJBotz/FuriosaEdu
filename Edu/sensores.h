#include <Arduino.h>
#include <inttypes.h>
#include <math.h>

unsigned long dist_sonar(uint8_t trig, uint8_t echo);
float         dist_sharp(uint8_t pino);

void init_sensores(void) {
    pinMode(sensor_esq, INPUT);
    pinMode(sensor_dir, INPUT);

    pinMode(TRIG, OUTPUT);
    pinMode(ECHO, INPUT);
}

unsigned int dist_frente(void) { return dist_sonar(TRIG, ECHO); }
float        dist_esq(void)    { return dist_sharp(sensor_esq); }
float        dist_dir(void)    { return dist_sharp(sensor_dir); }


float dist_sharp(uint8_t pino) {
    return 13*pow(analogRead(pino)*0.0048828125, -1);
}

unsigned long dist_sonar(uint8_t trig, uint8_t echo) {
    digitalWrite(trig, HIGH); delayMicroseconds(5);
    digitalWrite(trig, LOW);

    unsigned long dt = pulseIn(echo, HIGH, 30000); // microssegundos
    return dt*34/1000/2;
}
