#include "edu.h" // biblioteca: https://github.com/UerjBotz/Edu/tree/master
#include <SumoIR.h>

#define pino_ir 15

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
  IR.begin(pino_ir);
  IR.setLed(2, HIGH, 180);
}

void loop() {
  // debug_e();
  // debug_d();
  // debug_f();
  IR.update();
  if (IR.prepare()) {
    mover(0,0);
  }
  else if (IR.start()) {
    Serial.println("Começo ativado");
  }
  if (IR.on()) {
    simb = sensor();
    estado_atual = maquina_estado(estado_atual, simb);
    Serial.print("Simbolo: ");
    Serial.println(simb);
    Serial.print("Estado: ");
    Serial.println(estado_atual);
    acao(estado_atual);
  } else if (IR.stop()) {
    mover(0,0);
  }
}

enum estado maquina_estado(enum estado e, enum simbolo s) {
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
<<<<<<< HEAD
    case HORARIO:      mover(600,-600);  break;
    case ANTI_HORARIO: mover(-600,600);  break;
    case ATAQUE_D:     mover(1023,1023);   break;
    case ATAQUE_E:     mover(1023,1023);   break;
=======
    case HORARIO:      mover(150,-150);  break;
    case ANTI_HORARIO: mover(-150,150);  break;
    case ATAQUE_D:     mover(255,255);   break;
    case ATAQUE_E:     mover(255,255);   break;
>>>>>>> refs/remotes/origin/master
  }
}

// Arena 75cm de diâmetro, sensores esquerdo e direito mandam números >800 ao não detectar
enum simbolo sensor() {
<<<<<<< HEAD
  if (dist_esq()) {
    // Serial.println("ESQUERDA");
    return SENSOR_E;
  }
  else if (dist_dir()) {
    // Serial.println("DIREITA");
    return SENSOR_D;
  }
  if (dist_frente_esq() || dist_frente_dir()) {
=======
  if (dist_esq() < 15) {
    // Serial.println("ESQUERDA");
    return SENSOR_E;
  }
  else if (dist_dir() < 15) {
    // Serial.println("DIREITA");
    return SENSOR_D;
  }
  if (dist_frente() < 20) {
>>>>>>> refs/remotes/origin/master
    // Serial.println("EMPURRANDO");
    return SENSOR_F;
  }
  return NADA;
<<<<<<< HEAD
=======
}

void debug_e() {
  Serial.println(dist_esq());
  Serial.print("Esquerda: ");
}

void debug_d() {
  Serial.println(dist_dir());
  Serial.print("Direita: ");
}

void debug_f() {
  Serial.println(dist_frente());
  Serial.print("Frente: ");
>>>>>>> refs/remotes/origin/master
}