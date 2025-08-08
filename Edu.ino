#include "edu.h" // biblioteca: https://github.com/UerjBotz/Edu/tree/master
#include <SumoIR.h>

enum estado {
  HORARIO = 0,
  ANTI_HORARIO,
  ATAQUE_E,
  ATAQUE_D,
};

enum simbolo {
  SENSOR_F = 0,
  SENSOR_E,
  SENSOR_D,
  NADA,
};

enum estado estado_atual = HORARIO;
enum simbolo simb;

SumoIR IR;
void setup() {
  init_edu(9600);
  IR.begin(A0);
  IR.setLed(LED_BUILTIN, HIGH, 180);
}

void loop() {
  IR.update();
  if (IR.on()) {
    simb = sensor();
    estado_atual = maquina_estado(estado_atual, simb);
    Serial.print("Simbolo: ");
    Serial.println(simb);
    Serial.print("Estado: ");
    Serial.println(estado_atual);
    acao(estado_atual);
  } else {
    mover(0,0);
  }
}

enum estado maquina_estado(enum estado e, enum simbolo s) { // TODO: Print para saber o estado atual do robô.
  switch(e) {
    case HORARIO: 
      switch(s) {
        case SENSOR_F: return ATAQUE_D;
        case SENSOR_E: return ANTI_HORARIO;
        case SENSOR_D: return HORARIO;
        case NADA:     return HORARIO;
      }
      break;
    case ANTI_HORARIO: 
      switch(s) {
        case SENSOR_F: return ATAQUE_E;
        case SENSOR_D: return HORARIO;
        case SENSOR_E: return ANTI_HORARIO;
        case NADA:     return ANTI_HORARIO;
      }
      break;
    case ATAQUE_D:
      switch(s) {
        case SENSOR_F: return ATAQUE_D;
        case SENSOR_E: return ANTI_HORARIO;
        case SENSOR_D: return HORARIO;
        case NADA:     return HORARIO;
      }
      break;
    case ATAQUE_E:
      switch(s) {
        case SENSOR_F: return ATAQUE_E;
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
    case HORARIO:      mover(600,-600);  break;
    case ANTI_HORARIO: mover(-600,600);  break;
    case ATAQUE_D:     mover(1023,1023); break;
    case ATAQUE_E:     mover(1023,1023); break;
  }
}

// Arena 75cm de diâmetro, sensores esquerdo e direito mandam números >800 ao não detectar
enum simbolo sensor() {
  if (dist_frente() < 20) {
    //Serial.println("EMPURRANDO");
    return SENSOR_F;
  }
  if (dist_esq() < 20) {
    //Serial.println("ESQUERDA");
    return SENSOR_E;
  }
  else if (dist_dir() < 20) {
    //Serial.println("DIREITA");
    return SENSOR_D;
  }
  return NADA;
}
