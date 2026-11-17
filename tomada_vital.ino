// Tomada Vital - prototipo IoT (Arduino UNO no Tinkercad)
// Detecta a queda de energia, calcula quanto a bateria do aparelho ainda aguenta
// e acende a cor da prioridade. A mensagem da Serial e o que iria para a nuvem.

const int PINO_REDE = 2;        // chave deslizante: 5V = tem energia, GND = queda
const int PINO_BATERIA = A0;    // potenciometro: simula a bateria do aparelho (0 a 100%)
const int LED_VERDE = 9;        // energia normal
const int LED_AMARELO = 10;     // queda com prioridade ALTA ou MODERADA
const int LED_VERMELHO = 11;    // queda CRITICA
const int BUZZER = 8;           // apita na queda CRITICA

const String DISPOSITIVO = "TV-001";
const long AUTONOMIA_TOTAL_MIN = 480;   // 8 h de bateria cheia (valor de exemplo)
const long LIMITE_CRITICA = 120;        // ate 2 h = CRITICA
const long LIMITE_ALTA = 360;           // ate 6 h = ALTA; acima disso = MODERADA

bool emQueda = false;
unsigned long inicioQueda = 0;
String ultimaPrioridade = "";

void setup() {
  pinMode(PINO_REDE, INPUT);
  pinMode(LED_VERDE, OUTPUT);
  pinMode(LED_AMARELO, OUTPUT);
  pinMode(LED_VERMELHO, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  Serial.begin(9600);
  Serial.println(DISPOSITIVO + ";INICIO;monitorando a energia");
}

String classificar(long restantes) {
  if (AUTONOMIA_TOTAL_MIN == 0 || restantes <= LIMITE_CRITICA) {
    return "CRITICA";
  }
  if (restantes <= LIMITE_ALTA) {
    return "ALTA";
  }
  return "MODERADA";
}

void acenderLeds(bool verde, bool amarelo, bool vermelho) {
  digitalWrite(LED_VERDE, verde ? HIGH : LOW);
  digitalWrite(LED_AMARELO, amarelo ? HIGH : LOW);
  digitalWrite(LED_VERMELHO, vermelho ? HIGH : LOW);
}

void loop() {
  bool temEnergia = digitalRead(PINO_REDE) == HIGH;

  if (temEnergia) {
    if (emQueda) {
      emQueda = false;
      noTone(BUZZER);
      Serial.println(DISPOSITIVO + ";RESTABELECIDO");
    }
    acenderLeds(true, false, false);
  } else {
    int leitura = analogRead(PINO_BATERIA);
    long bateria = map(leitura, 0, 1023, 0, 100);

    if (!emQueda) {
      emQueda = true;
      inicioQueda = millis();
      ultimaPrioridade = "";
    }

    // Na simulacao, 1 segundo real vale 1 minuto.
    long passados = (millis() - inicioQueda) / 1000;
    long restantes = AUTONOMIA_TOTAL_MIN * bateria / 100 - passados;
    if (restantes < 0) {
      restantes = 0;
    }
    String prioridade = classificar(restantes);

    // Envia quando a prioridade muda e, depois, a cada 15 "minutos".
    if (prioridade != ultimaPrioridade || passados % 15 == 0) {
      Serial.println(DISPOSITIVO + ";QUEDA;bateria=" + String(bateria) + ";restante="
                     + String(restantes) + ";prioridade=" + prioridade);
      ultimaPrioridade = prioridade;
    }

    if (prioridade == "CRITICA") {
      acenderLeds(false, false, true);
      tone(BUZZER, 1000, 200);
    } else {
      acenderLeds(false, true, false);
      noTone(BUZZER);
    }
  }

  delay(1000);
}
