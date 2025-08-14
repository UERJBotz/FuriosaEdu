#include "edu.h" // biblioteca: https://github.com/UerjBotz/Edu/tree/master
#include <SumoIR.h>

#define pino_ir 15
#define led     2

enum estado {
  HORARIO = 0,
  ANTI_HORARIO,
  ATAQUE_E,
  ATAQUE_D,
};

enum simbolo {
  SENSOR_FE = 0,
  SENSOR_FD,
  SENSOR_E,
  SENSOR_D,
  NADA,
};

enum estado  estado_atual = HORARIO;
enum simbolo simb;

SumoIR IR;

void setup() {
  init_edu(9600);
  IR.begin(pino_ir);
  IR.setLed(led, HIGH, 180);
}

void loop() {
  IR.update();

  if (IR.prepare()) {
    mover(0,0);
  }
  else if (IR.start()) {
    Serial.println("Começo");
  }
  else if (IR.on()) {
    simb = sensor();
    estado_atual = maquina_estado(estado_atual, simb);
    Serial.print("Simbolo: ");
    Serial.println(simb);
    Serial.print("Estado: ");
    Serial.println(estado_atual);

    #ifdef MADMAX
      mover(1023, 1023);
    #else
      acao(estado_atual);
    #endif
  } 
  else if (IR.stop()) {
    mover(0,0);
  }
}

enum estado maquina_estado(enum estado e, enum simbolo s) {
  switch(e) {
    case HORARIO: 
      switch(s) {
        case SENSOR_FE: return ATAQUE_E;
        case SENSOR_FD: return ATAQUE_D;
        case SENSOR_E: return ANTI_HORARIO;
        case SENSOR_D: return HORARIO;
        case NADA:     return HORARIO;
      }
      break;
    case ANTI_HORARIO: 
      switch(s) {
        case SENSOR_FE: return ATAQUE_E;
        case SENSOR_FD: return ATAQUE_D;
        case SENSOR_D: return HORARIO;
        case SENSOR_E: return ANTI_HORARIO;
        case NADA:     return ANTI_HORARIO;
      }
      break;
    case ATAQUE_D:
      switch(s) {
        case SENSOR_FE: return ATAQUE_E;
        case SENSOR_FD: return ATAQUE_D;
        case SENSOR_E: return ANTI_HORARIO;
        case SENSOR_D: return HORARIO;
        case NADA:     return HORARIO;
      }
      break;
    case ATAQUE_E:
      switch(s) {
        case SENSOR_FE: return ATAQUE_E;
        case SENSOR_FD: return ATAQUE_D;
        case SENSOR_D: return HORARIO;
        case SENSOR_E: return ANTI_HORARIO;
        case NADA:     return ANTI_HORARIO;
      }
      break;
  }
  return HORARIO;
}

void acao(enum estado estado_atual) {
  switch(estado_atual) {
    case HORARIO:      mover(1023,-1023);    break;
    case ANTI_HORARIO: mover(-1023,1023);    break;
    case ATAQUE_D:     mover(1023,1010);   break;
    case ATAQUE_E:     mover(1010,1023);   break;
  }
}

enum simbolo sensor() {
  if (dist_esq()) {
    // Serial.println("ESQUERDA");
    return SENSOR_E;
  }
  if (dist_dir()) {
    // Serial.println("DIREITA");
    return SENSOR_D;
  }
  if (dist_frente_esq()) {
    // Serial.println("ATACANDO ESQUERDA");
    return SENSOR_FE;
  }
  if (dist_frente_dir()) {
    // Serial.println("ATACANDO DIREITA")
    return SENSOR_FD;
  }
  return NADA;
}