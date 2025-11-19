// ==============================
// Robô Sumo — Versão Otimizada (Busca 180° + Borda Rápida)
// ==============================

// ---- Pinos dos motores ----
int motorA_frente = 9;
int motorA_tras   = 6;
int motorB_frente = 5;
int motorB_tras   = 3;

// ---- Sensor ultrassônico ----
int trig = 11;
int echo = 10;

// ---- Sensores de refletância ----
int sensorE = A0;
int sensorD = A1;

// ---- Botão ----
int botao = 12;

// ---- Parâmetros ----
int limite_reflex = 600;
int distancia_ataque = 10;
int distancia_max = 80;
int vel_max = 255;
int vel_min = 140;
int vel_re = 140;

bool combateAtivo = false;
bool buscaLado = false;   // FALSE = direita, TRUE = esquerda

// ==============================
// Movimentos
// ==============================

void parar() {
  analogWrite(motorA_frente, 0);
  analogWrite(motorA_tras, 0);
  analogWrite(motorB_frente, 0);
  analogWrite(motorB_tras, 0);
}

void frente(int v) {
  analogWrite(motorA_frente, v);
  analogWrite(motorA_tras, 0);
  analogWrite(motorB_frente, v);
  analogWrite(motorB_tras, 0);
}

void re(int v) {
  analogWrite(motorA_frente, 0);
  analogWrite(motorA_tras, v);
  analogWrite(motorB_frente, 0);
  analogWrite(motorB_tras, v);
}

void girarDireita(int v) {
  analogWrite(motorA_frente, v);
  analogWrite(motorA_tras, 0);
  analogWrite(motorB_frente, 0);
  analogWrite(motorB_tras, v);
}

void girarEsquerda(int v) {
  analogWrite(motorA_frente, 0);
  analogWrite(motorA_tras, v);
  analogWrite(motorB_frente, v);
  analogWrite(motorB_tras, 0);
}

// ==============================
// Leitura sensores (OTIMIZADO)
// ==============================

// --- leitura rápida para borda ---
int lerQRE_fast(int pino) {
  return analogRead(pino);
}

// --- leitura suave quando não atacando ---
int lerQRE(int pino) {
  return (analogRead(pino) + analogRead(pino) + analogRead(pino)) / 3;
}

// --- ultrassônico ---
int lerDistancia() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long d = pulseIn(echo, HIGH, 30000);
  return (d == 0 ? 0 : d * 0.034 / 2);
}

// ==============================
// Lógica principal
// ==============================

void comportamento() {

  int dist = lerDistancia();

  // leitura rápida da borda
  int valE = lerQRE_fast(sensorE);
  int valD = lerQRE_fast(sensorD);

  bool linhaE = valE <= limite_reflex;
  bool linhaD = valD <= limite_reflex;

  // ------- PRINTS RESTAURADOS -------
  Serial.print("Dist: "); Serial.print(dist);
  Serial.print(" | E: "); Serial.print(valE);
  Serial.print(" | D: "); Serial.println(valD);

  // -------- FUGA DA BORDA ----------
  if (linhaE || linhaD) {
    Serial.println("⚠ Borda detectada! Recuando!");

    re(vel_re);
    delay(200);

    if (linhaE && !linhaD)
      girarDireita(vel_re);
    else if (linhaD && !linhaE)
      girarEsquerda(vel_re);
    else
      girarDireita(vel_re);

    delay(200);
    parar();
    return;
  }

  // -------- ATAQUE -----------
  if (dist > 0 && dist <= distancia_ataque) {

    // segurança contra borda durante ataque
    int bordaE = lerQRE_fast(sensorE);
    int bordaD = lerQRE_fast(sensorD);

    if (bordaE <= limite_reflex || bordaD <= limite_reflex)
      return;

    Serial.println("ATAQUE MÁXIMO!");
    frente(vel_max);
    return;
  }

  // -------- APROXIMAÇÃO -----------
  if (dist > distancia_ataque && dist <= distancia_max) {
    Serial.println("➡ Inimigo longe — avançando...");
    frente(vel_min);
    return;
  }

  // -------- BUSCA 180° -----------
  Serial.println("Procurando inimigo... busca 180°");

  if (!buscaLado) {
    girarEsquerda(vel_min);
    delay(120);
    buscaLado = true;
  } else {
    girarDireita(vel_min);
    delay(120);
    buscaLado = false;
  }

  parar();
}

// ==============================
// Setup
// ==============================

void setup() {
  Serial.begin(9600);
  pinMode(botao, INPUT_PULLUP);
  pinMode(trig, OUTPUT);
  pinMode(echo, INPUT);
  pinMode(motorA_frente, OUTPUT);
  pinMode(motorA_tras, OUTPUT);
  pinMode(motorB_frente, OUTPUT);
  pinMode(motorB_tras, OUTPUT);
  parar();
  Serial.println("Pronto. Precione o botão");
}

// ==============================
// Loop
// ==============================

void loop() {
  if (!combateAtivo && digitalRead(botao) == LOW) {
    Serial.println("Start em 3...");
    delay(3000);
    Serial.println("Vai!");
    combateAtivo = true;
  }

  if (combateAtivo)
    comportamento();
}
