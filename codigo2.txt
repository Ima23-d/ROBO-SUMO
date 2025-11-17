// ==============================
// Robô Sumo — Versão Corrigida
// ==============================

// ---- Pinos dos motores ----
int motorA_frente = 9;
int motorA_tras   = 6;
int motorB_frente = 5;
int motorB_tras   = 3;

// ---- Sensor ultrassônico ----
int trig = 11;
int echo = 10;

// ---- Sensores de refletância QRE ----
int sensorE = A0;   // Esquerda
int sensorD = A1;   // Direita

// ---- Botão de início ----
int botao = 12;

// ---- Parâmetros ----
int limite_reflex = 600;     // Branco acima disso
int distancia_ataque = 30;   // Inimigo perto
int distancia_max = 80;      // Máximo de busca
int vel_max = 255;
int vel_min = 140;
int vel_re = 140;

bool combateAtivo = false;

// ==============================
// Funções básicas
// ==============================

void parar() {
  analogWrite(motorA_frente, 0);
  analogWrite(motorA_tras, 0);
  analogWrite(motorB_frente, 0);
  analogWrite(motorB_tras, 0);
}

void frente(int vel) {
  analogWrite(motorA_frente, vel);
  analogWrite(motorA_tras, 0);
  analogWrite(motorB_frente, vel);
  analogWrite(motorB_tras, 0);
}

void re(int vel) {
  analogWrite(motorA_frente, 0);
  analogWrite(motorA_tras, vel);
  analogWrite(motorB_frente, 0);
  analogWrite(motorB_tras, vel);
}

void girarDireita(int vel) {
  analogWrite(motorA_frente, vel);
  analogWrite(motorA_tras, 0);
  analogWrite(motorB_frente, 0);
  analogWrite(motorB_tras, vel);
}

void girarEsquerda(int vel) {
  analogWrite(motorA_frente, 0);
  analogWrite(motorA_tras, vel);
  analogWrite(motorB_frente, vel);
  analogWrite(motorB_tras, 0);
}

// ==============================
// Leitura dos sensores
// ==============================

int lerDistancia() {
  digitalWrite(trig, LOW);
  delayMicroseconds(2);
  digitalWrite(trig, HIGH);
  delayMicroseconds(10);
  digitalWrite(trig, LOW);

  long duracao = pulseIn(echo, HIGH, 30000);
  if (duracao == 0) return 0;

  int distancia = duracao * 0.034 / 2;
  return distancia;
}

int lerRefletancia(int pino) {
  long soma = 0;
  for (int i = 0; i < 5; i++) {
    soma += analogRead(pino);
    delay(2);
  }
  return soma / 5;
}

// ==============================
// Comportamento principal
// ==============================

void comportamento() {
  int dist = lerDistancia();
  int valE = lerRefletancia(sensorE);
  int valD = lerRefletancia(sensorD);

  bool linhaE = valE <= limite_reflex;
  bool linhaD = valD <= limite_reflex;

  // ---- PRINTS DOS SENSORES ----
  Serial.print("Distância: "); Serial.print(dist); Serial.print(" cm | ");
  Serial.print("QRE E: "); Serial.print(valE); Serial.print(" | ");
  Serial.print("QRE D: "); Serial.println(valD);

  // ---- FUGA DA BORDA ----
  if (linhaE || linhaD) {
    Serial.println("⚠️ LINHA DETECTADA! Recuando...");

    re(vel_re);
    delay(300);

    if (linhaE && !linhaD) {
      Serial.println("Virando para a direita...");
      girarDireita(vel_re);
      delay(350);
    }
    else if (linhaD && !linhaE) {
      Serial.println("Virando para a esquerda...");
      girarEsquerda(vel_re);
      delay(350);
    }
    else {  
      Serial.println("Linha nos dois sensores — virando direita");
      girarDireita(vel_re);
      delay(350);
    }

    parar();
    delay(80);
    return;
  }

  // ---- ATAQUE ----
  if (dist > 0 && dist <= distancia_ataque) {
    Serial.print("🎯 ATAQUE! Inimigo a ");
    Serial.print(dist);
    Serial.println(" cm!");
    frente(vel_max);
    return;
  }

  // ---- BUSCA DO INIMIGO ----
  if (dist > distancia_ataque && dist <= distancia_max) {
    Serial.println("➡ Inimigo detectado longe — avançando devagar...");
    frente(vel_min);
    return;
  }

  Serial.println("🔍 Procurando inimigo...");
  girarDireita(vel_min);
  delay(200);
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
  Serial.println("✅ Pronto! Aperte o botão para iniciar...");
}

// ==============================
// Loop principal
// ==============================

void loop() {
  if (!combateAtivo && digitalRead(botao) == LOW) {
    Serial.println("🕒 Botão pressionado! Iniciando em 3 segundos...");
    parar();

    for (int i = 3; i > 0; i--) {
      Serial.print("Começando em ");
      Serial.println(i);
      delay(1000);
    }

    Serial.println("🚀 COMBATE INICIADO!");
    combateAtivo = true;
  }

  if (combateAtivo) comportamento();
  else parar();

  delay(40);
}
