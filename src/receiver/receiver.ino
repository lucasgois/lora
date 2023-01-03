#include "heltec.h"

#define BAND 915E6
#define RELE 12

void setup() {
  Heltec.begin(true, true, true, true, BAND);
  pinMode(RELE, OUTPUT);

  // LoRa.receive();
}

void loop() {
  String texto = receber();

  if (texto.isEmpty()) {
    return;
  }

  bool pulso = texto.charAt(0) == '1';

  enviar("Executando comando");

  if (pulso) {
    digitalWrite(RELE, HIGH);
    delay(800);
    digitalWrite(RELE, LOW);
  }

  enviar("Comando executado");
}

void enviar(String texto) {
  Serial.print("enviar: ");
  Serial.println(texto);

  LoRa.beginPacket();
  LoRa.print(texto);
  LoRa.endPacket();

  piscar();
}

String receber() {
  String texto = "";

  int packetSize = LoRa.parsePacket();

  if (packetSize) {

    while (LoRa.available()) {
      texto = "";

      for (int i = 0; i < packetSize; i++) {
        texto += (char)LoRa.read();
      }
    }

    Serial.print("receber: ");
    Serial.println(texto);

    piscar();
  }

  return texto;
}

void piscar() {
  digitalWrite(25, HIGH);
  delay(50);
  digitalWrite(25, LOW);
  delay(50);
  digitalWrite(25, HIGH);
  delay(50);
  digitalWrite(25, LOW);
}