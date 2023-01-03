#include "heltec.h"

#define BAND 915E6
#define PINO 2

bool bloqueado = false;

int watchdog = 2;

void setup() {
  pinMode(PINO, INPUT_PULLUP);

  Heltec.begin(true, true, true, true, BAND);

  Heltec.display->init();
  Heltec.display->setFont(ArialMT_Plain_10);

  tela("SEGURSAT TECH");
}

void loop() {

  if (watchdog > 0) {
    watchdog--;

  } else {
    tela("SEGURSAT TECH");
  }

  String texto = receber();

  if (!texto.isEmpty()) {
    tela(texto);

  } else {
    texto = Serial.readString();

    if (!texto.isEmpty()) {
      tela(texto);
    }
  }

  int leitura = digitalRead(PINO);

  // tela(String(leitura));

  if (leitura) {
    bloqueado = false;

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

  watchdog = 2;
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