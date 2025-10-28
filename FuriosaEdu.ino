#include "edu.h"
#include "LEDFX.h"
#include <SumoIR.h>
#include <BluetoothSerial.h>

#define pino_ir 15
#define led     2

SumoIR IR;
BluetoothSerial SerialBT;


enum girar_ate {
  HORARIO = 0,
  ANTI_HORARIO,
  ATAQUE_E,
  ATAQUE_D,
  ATAQUE_F,
};

enum simbolo {
  SENSOR_FE = 0,
  SENSOR_FD,
  SENSOR_FEFD,
  SENSOR_E,
  SENSOR_D,
  NADA,
};

enum estrategia {
  GIRAR_ATE = 4,
  MAD_MAX = 9,
} estrategia = GIRAR_ATE;


void setup() {
  init_edu(9600);
  IR.begin(pino_ir);
  IR.setLed(led, HIGH, 180);
  SerialBT.begin("Furiosa está ligada");
}

void loop() {
  IR.update();

  struct leitura sensores = leitura_sensores();
  mostra_sensores_no_led(sensores);

  if (IR.available()) {/* quando o sensor estiver ativado */
    // salva o número lido pelo sensor, estando de 4 a 9
    int cmd = IR.read();
    if (cmd >= 4 && cmd <= 9) {
        estrategia = (enum estrategia)cmd;
    }
  }

  if (IR.prepare()) {
    mover(0,0);
    SerialBT.println("Prepare");
  } else if (IR.start()) {
    SerialBT.println("Start");
  } else if (IR.on()) {
    SerialBT.println("On");

    enum simbolo simb = prox_simbolo(sensores);
    switch (estrategia) {
      default:
      case GIRAR_ATE: {
        static enum girar_ate estado = HORARIO;

        estado = maquina_girar_ate(estado, simb);
        acao_girar_ate(estado);
      } break;

      case MAD_MAX: {
        mover(1023, 1023);
      } break;

    }
  } else if (IR.stop()) {
    SerialBT.println("Stop");
    mover(0,0);
  }
}

enum girar_ate maquina_girar_ate(enum girar_ate e,
                                 enum simbolo s) {
  switch(e) {
    case HORARIO:
      switch(s) {
        case SENSOR_FE:   return ATAQUE_E;
        case SENSOR_FD:   return ATAQUE_D;
        case SENSOR_FEFD: return ATAQUE_F;
        case SENSOR_E:    return ANTI_HORARIO;
        case SENSOR_D:    return HORARIO;
        case NADA:        return HORARIO;
      } break;
    case ANTI_HORARIO: 
      switch(s) {
        case SENSOR_FE:   return ATAQUE_E;
        case SENSOR_FD:   return ATAQUE_D;
        case SENSOR_FEFD: return ATAQUE_F;
        case SENSOR_D:    return HORARIO;
        case SENSOR_E:    return ANTI_HORARIO;
        case NADA:        return ANTI_HORARIO;
      } break;
    case ATAQUE_F:
      switch(s) {
        case SENSOR_FE:   return ATAQUE_E;
        case SENSOR_FD:   return ATAQUE_D;
        case SENSOR_FEFD: return ATAQUE_F;
        case SENSOR_E:    return ANTI_HORARIO;
        case SENSOR_D:    return HORARIO;
        case NADA:        return ATAQUE_F;
      } break;
    case ATAQUE_D:
      switch(s) {
        case SENSOR_FE:   return ATAQUE_E;
        case SENSOR_FD:   return ATAQUE_D;
        case SENSOR_FEFD: return ATAQUE_F;
        case SENSOR_E:    return ANTI_HORARIO;
        case SENSOR_D:    return HORARIO;
        case NADA:        return HORARIO;
      } break;
    case ATAQUE_E:
      switch(s) {
        case SENSOR_FE:   return ATAQUE_E;
        case SENSOR_FD:   return ATAQUE_D;
        case SENSOR_FEFD: return ATAQUE_F;
        case SENSOR_D:    return HORARIO;
        case SENSOR_E:    return ANTI_HORARIO;
        case NADA:        return ANTI_HORARIO;
      } break;
  }

  return e;
}

void acao_girar_ate(enum girar_ate estado) {
  switch(estado) {
    case HORARIO: {
      mover(700,-700); Serial.println("Girando horario");
    } break;
    case ANTI_HORARIO: {
      mover(-700,700); Serial.println("Girando anti-horario");
    } break;
    case ATAQUE_F: {
      mover(1023,1023); Serial.println("Atacando frente");
    } break;
    case ATAQUE_D: {
      mover(1023,1010); Serial.println("Atacando direita");
    } break;
    case ATAQUE_E: {
      mover(1010,1023); Serial.println("Atacando esquerda");
    } break;
  }
}

enum simbolo prox_simbolo(struct leitura sensores) {
  if (sensores.esq)        return SENSOR_E;  else
  if (sensores.dir)        return SENSOR_D;  else
  if (sensores.frente_esq) return SENSOR_FE; else
  if (sensores.frente_dir) return SENSOR_FD; else
  if (sensores.frente_esq &&
      sensores.frente_dir) return SENSOR_FEFD;

  return NADA;
}
