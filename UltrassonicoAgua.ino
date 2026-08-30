#include <esp_now.h>
#include <WiFi.h>

// Endereço MAC formatado do seu ESP32 Receptor (LED/Buzzer)
uint8_t macAtuador[] = {0x80, 0xF3, 0xDA, 0x4B, 0x7C, 0x9C};

const int pinoTrig = 5;
const int pinoEcho = 18;

// Estrutura de mensagem enviada
typedef struct struct_mensagem {
  bool alertaAtivo;
} struct_mensagem;

struct_mensagem dadosEnvio;
esp_now_peer_info_t peerInfo;

long lerDistanciaCm() {
  digitalWrite(pinoTrig, LOW);
  delayMicroseconds(2);
  digitalWrite(pinoTrig, HIGH);
  delayMicroseconds(10);
  digitalWrite(pinoTrig, LOW);
  
  long duracao = pulseIn(pinoEcho, HIGH, 30000); // Timeout ~5 metros
  if (duracao == 0) return 999; // Sem retorno do eco
  return (duracao * 0.0343) / 2;
}

void setup() {
  Serial.begin(115200);
  pinMode(pinoTrig, OUTPUT);
  pinMode(pinoEcho, INPUT);

  WiFi.mode(WIFI_STA);

  if (esp_now_init() != ESP_OK) {
    Serial.println("Erro ao inicializar ESP-NOW");
    return;
  }

  memcpy(peerInfo.peer_addr, macAtuador, 6);
  peerInfo.channel = 0;  
  peerInfo.encrypt = false;
  
  if (esp_now_add_peer(&peerInfo) != ESP_OK) {
    Serial.println("Erro ao adicionar o receptor");
    return;
  }
}

void loop() {
  long distancia = lerDistanciaCm();
  Serial.print("Distância: ");
  Serial.print(distancia);
  Serial.println(" cm");

  // Ativa o alerta se a distância for <= 20 cm
  dadosEnvio.alertaAtivo = (distancia <= 20);

  esp_now_send(macAtuador, (uint8_t *) &dadosEnvio, sizeof(dadosEnvio));

  delay(200); // Leitura/Envio a cada 200 ms
}