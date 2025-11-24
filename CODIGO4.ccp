// ==============================
// Robô Sumo — Versão Otimizada (Busca 180° + Borda Correta)
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
bool buscaLado = false; // FALSE = direita, TRUE = esquerda

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
// Leitura sensores — CORRETAS
// ==============================

// Leitura rápida para detectar borda (melhor p/ sumô)
int lerQRE_fast(int pino) {
  return analogRead(pino);
}

// Leitura suave para monitoramento
int lerQRE_suave(int pino) {
  return (analogRead(pino) + analogRead(pino) + analogRead(pino)) / 3;
}

// Ultrassônico otimizado
int lerDistancia() {

  digitalWrite(trig, LOW);
  delayMicroseconds(4);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duracao = pulseIn(echo, HIGH, 10000);
  if (duracao == 0) return 0;

  int dist = duracao * 0.034 / 2;

  if (dist < 2 || dist > 300) return 0;

  return dist;
}

// ==============================
// Lógica principal
// ==============================

void comportamento() {

  int dist = lerDistancia();

  // Leitura rápida → borda
  int valE = lerQRE_fast(sensorE);
  int valD = lerQRE_fast(sensorD);

  bool linhaE = (valE <= limite_reflex);
  bool linhaD = (valD <= limite_reflex);

  // Monitor Serial
  Serial.print("Dist: ");
  Serial.print(dist);
  Serial.print(" | E: ");
  Serial.print(valE);
  Serial.print(" | D: ");
  Serial.println(valD);

  // ============================
  // Fugir da borda
  // ============================
  if (linhaE || linhaD) {

    Serial.println("⚠ BORDA DETECTADA! RECUANDO!");

    re(vel_re);
    delay(215);

    if (linhaE && !linhaD)
      girarDireita(vel_re);
    else if (linhaD && !linhaE)
      girarEsquerda(vel_re);
    else
      girarDireita(vel_re);

    delay(215);
    parar();
    return;
  }

  // ============================
  // ATAQUE
  // ============================
  if (dist > 0 && dist <= distancia_ataque) {

    // segurança contra borda durante ataque
    if (lerQRE_fast(sensorE) <= limite_reflex ||
        lerQRE_fast(sensorD) <= limite_reflex)  
        return;

    Serial.println("ATAQUE MÁXIMO!");
    frente(vel_max);
    return;
  }

  // ============================
  // Aproximação
  // ============================
  if (dist > distancia_ataque && dist <= distancia_max) {
    Serial.println("➡ Inimigo detectado — aproximando...");
    frente(vel_min);
    return;
  }

  // ============================
  // Busca 180° Alternada
  // ============================
  Serial.println("Procurando inimigo... BUSCA 180°");

  if (!buscaLado) {
    girarEsquerda(vel_min);
    delay(170);
    buscaLado = true;
  } else {
    girarDireita(vel_min);
    delay(170);
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

  Serial.println("Pronto. Pressione o botão.");
}

// ==============================
// Loop
// ==============================

void loop() {

  if (!combateAtivo && digitalRead(botao) == LOW) {
    Serial.println("Iniciando em 3...");
    delay(3000);
    Serial.println("VAI!");
    combateAtivo = true;
  }

  if (combateAtivo)
    comportamento();
}
