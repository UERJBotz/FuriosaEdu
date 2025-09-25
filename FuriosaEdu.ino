#include "edu.h"
#include <SumoIR.h>
#include <BluetoothSerial.h>

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
BluetoothSerial SerialBT;

enum estrategia {
  GIRAR_ATE = 4,
  MAD_MAX = 9,
} estrategia = GIRAR_ATE;

void setup() {
  init_edu(9600);
  IR.begin(pino_ir);
  IR.setLed(led, HIGH, 180);
  SerialBT.begin("Furiosa");
}

void loop() {
  IR.update();

  if (IR.available()) { /* quando o sensor tiver ativado */
    // salva o número lido pelo sensor, estando de 4 a 9
    int cmd = IR.read();
    if (cmd >= 4 && cmd <= 9) estrategia = (enum estrategia)cmd;
  }

  if (IR.prepare()) {
    mover(0,0);
    SerialBT.println("Prepare");
  }
  else if (IR.start()) {
    SerialBT.println("Start");
  }
  else if (IR.on()) {
    SerialBT.println("On");

    simb = sensor();

    switch(estrategia) {
      default:
      case GIRAR_ATE:
        estado_atual = maquina_estado_girar_ate(estado_atual, simb);
        acao_andar_ate(estado_atual);
      break;

      case MAD_MAX: mover(1023, 1023); break;
    }
  } 
  else if (IR.stop()) {
    SerialBT.println("Stop");
    mover(0,0);
  }
}

enum estado maquina_estado_girar_ate(enum estado e, enum simbolo s) {
  switch(e) {
    case HORARIO: 
      switch(s) {
        case SENSOR_FE: return ATAQUE_E;
        case SENSOR_FD: return ATAQUE_D;
        case SENSOR_E:  return ANTI_HORARIO;
        case SENSOR_D:  return HORARIO;
        case NADA:      return HORARIO;
      }
      break;
    case ANTI_HORARIO: 
      switch(s) {
        case SENSOR_FE: return ATAQUE_E;
        case SENSOR_FD: return ATAQUE_D;
        case SENSOR_D:  return HORARIO;
        case SENSOR_E:  return ANTI_HORARIO;
        case NADA:      return ANTI_HORARIO;
      }
      break;
    case ATAQUE_D:
      switch(s) {
        case SENSOR_FE: return ATAQUE_E;
        case SENSOR_FD: return ATAQUE_D;
        case SENSOR_E:  return ANTI_HORARIO;
        case SENSOR_D:  return HORARIO;
        case NADA:      return HORARIO;
      }
      break;
    case ATAQUE_E:
      switch(s) {
        case SENSOR_FE: return ATAQUE_E;
        case SENSOR_FD: return ATAQUE_D;
        case SENSOR_D:  return HORARIO;
        case SENSOR_E:  return ANTI_HORARIO;
        case NADA:      return ANTI_HORARIO;
      }
      break;
  }
  return HORARIO;
}

void acao_andar_ate(enum estado estado_atual) {
  switch(estado_atual) {
    case HORARIO:      mover(1023,-1023);  break; // ALTERAR E TESTAR!!
    case ANTI_HORARIO: mover(-1023,1023);  break; // ALTERAR E TESTAR!!
    case ATAQUE_D:     mover(1023,1010);   break;
    case ATAQUE_E:     mover(1010,1023);   break;
  }
}

enum simbolo sensor() {
  if (dist_esq()) {
    return SENSOR_E;
  }
  if (dist_dir()) {
    return SENSOR_D;
  }
  if (dist_frente_esq()) {
    return SENSOR_FE;
  }
  if (dist_frente_dir()) {
    return SENSOR_FD;
  }
  return NADA;
}