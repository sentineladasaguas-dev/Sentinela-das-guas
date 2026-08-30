#include <esp_now.h>
#include <WiFi.h>

const int pinoLED = 13;
const int pinoBuzzer = 15;

// Estado global do alerta recebido via ESP-NOW
volatile bool estadoAlerta = false;

// Estrutura idêntica à do emissor
typedef struct struct_mensagem {
  bool alertaAtivo;
} struct_mensagem;

struct_mensagem dadosRecebidos;

// Função chamada automaticamente ao receber dados
void aoReceberDados(const esp_now_recv_info *info, const uint8_t *dados, int tamanho) {
  memcpy(&dadosRecebidos, dados, sizeof(dadosRecebidos));
  estadoAlerta = dadosRecebidos.alertaAtivo;
}

void setup() {
  Serial.begin(115200);

  pinMode(pinoLED, OUTPUT);
  pinMode(pinoBuzzer, OUTPUT);

  // Inicializa o buzzer desligado (agora em LOW) e LED desligado
  digitalWrite(pinoBuzzer, LOW);
  digitalWrite(pinoLED, LOW);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Erro ao inicializar ESP-NOW");
    return;
  }

  esp_now_register_recv_cb(aoReceberDados);
}

void loop() {
  if (estadoAlerta) {
    // Fase 1 da Sirene: LED acende e Buzzer liga (HIGH)
    digitalWrite(pinoLED, HIGH);
    digitalWrite(pinoBuzzer, HIGH); 
    delay(500);

    // Fase 2 da Sirene: LED apaga e Buzzer desliga (LOW)
    digitalWrite(pinoLED, LOW);
    digitalWrite(pinoBuzzer, LOW);  
    delay(500);
  } else {
    // Garantia de estado inativo caso nada seja detectado
    digitalWrite(pinoLED, LOW);
    digitalWrite(pinoBuzzer, LOW);
    delay(100);
  }
}