// ==============================
// Robô Sumo — Versão Otimizada (Busca 180°)
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
// Leitura sensores
// ==============================

int lerDistancia() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);
  long d = pulseIn(echo, HIGH, 30000);
  return (d == 0 ? 0 : d * 0.034 / 2);
}

int lerQRE(int pino) {
  long soma = 0;
  for (int i = 0; i < 5; i++) { 
    soma += analogRead(pino); delay(2); 
  }
  return soma / 5;
}

// ==============================
// Lógica principal
// ==============================

void comportamento() {

  int dist = lerDistancia();
  int valE = lerQRE(sensorE);
  int valD = lerQRE(sensorD);

  bool linhaE = valE <= limite_reflex;
  bool linhaD = valD <= limite_reflex;

  // Print monitor serial
  Serial.print("Dist: "); Serial.print(dist);
  Serial.print(" | E: "); Serial.print(valE);
  Serial.print(" | D: "); Serial.println(valD);

  // ---- Fuga da borda ----
  if (linhaE || linhaD) {
    Serial.println("⚠ Borda detectada! Recuando!");

    re(vel_re);
    delay(250);

    if (linhaE && !linhaD) 
      girarDireita(vel_re);
    else if (linhaD && !linhaE) 
      girarEsquerda(vel_re);
    else 
      girarDireita(vel_re);

    delay(250);
    parar();
    return;
  }

  // ---- Ataque ----
  if (dist > 0 && dist <= distancia_ataque) {
    Serial.println("🔥 ATAQUE MÁXIMO!");
    frente(vel_max);
    return;
  }

  // ---- Aproximação ----
  if (dist > distancia_ataque && dist <= distancia_max) {
    Serial.println("➡ Inimigo longe — avançando...");
    frente(vel_min);
    return;
  }

  // ---- Busca Inteligente (180°) ----
  Serial.println("🔍 Procurando inimigo... busca 180°");

  if (!buscaLado) {  
    girarEsquerda(vel_min);        // DIREITA
    delay(160);
    buscaLado = true;
  } else {                   // ESQUERDA
    girarDireita(vel_min);
    delay(160);
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
  Serial.println("Ready. Press button.");
}

// ==============================
// Loop
// ==============================

void loop() {
  if (!combateAtivo && digitalRead(botao) == LOW) {
    Serial.println("Start in 3...");
    delay(3000);
    Serial.println("GO!");
    combateAtivo = true;
  }

  if (combateAtivo) comportamento();
}
