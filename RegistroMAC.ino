#include <WiFi.h>

void setup() {
  Serial.begin(115200);
  delay(1000); // Aguarda a estabilização da comunicação Serial

  // Força o desligamento prévio da Wi-Fi
  WiFi.mode(WIFI_OFF);
  delay(100);

  // Inicializa o modo Station obrigatoriamente
  WiFi.mode(WIFI_STA);
  delay(100);

  // Imprime o endereço MAC real gravado na eFuse do chip
  Serial.println("\n------------------------------------");
  Serial.print("Endereço MAC do ESP32: ");
  Serial.println(WiFi.macAddress());
  Serial.println("------------------------------------\n");
}

void loop() {
  // Nada a executar
}