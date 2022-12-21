#include "heltec.h"

#define BAND 915E6
#define PINO 2

bool bloqueado = false;

void setup() {
  pinMode(PINO, INPUT_PULLUP);

  Heltec.begin(true, true, true, true, BAND);

  Heltec.display->init();
  Heltec.display->setFont(ArialMT_Plain_10);

  tela("SEGURSAT");
}

void loop() {
  int leitura = digitalRead(PINO);

  // tela(String(leitura));

  if (leitura) {
    // standby
    bloqueado = false;

    String texto = receber();

    if (!texto.isEmpty()) {
      tela(texto);
    } else {

      texto = Serial.readString();

      if (!texto.isEmpty()) {
        tela(texto);
      }
    }


  } else {

    if (!bloqueado) {
      bloqueado = true;

      tela("Enviando comando...");

      enviar("1");

      tela("Comando enviado!");
    }
  }
}

void tela(String mensagem) {
  Serial.println(mensagem);

  Heltec.display->clear();
  Heltec.display->drawString(0, 16, mensagem);
  Heltec.display->display();
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