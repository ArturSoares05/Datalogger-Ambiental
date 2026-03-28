#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <RTClib.h>
#include <DHT.h>

// --- Definições de Pinos ---
#define DHTPIN 2
#define DHTTYPE DHT22  
#define BUZZER 3
#define LED_V 4
#define LED_R 5
#define BTN_1 6        
#define BTN_2 7        
#define LDR_PIN A0

// --- Objetos ---
LiquidCrystal_I2C lcd(0x27, 16, 2);
RTC_DS1307 rtc;
DHT dht(DHTPIN, DHTTYPE);

// --- Variáveis de Controle ---
bool ligado = false;
int menuOpcao = 0;     
bool emMedicao = false;
unsigned long tempoBotoesPressionados = 0;
unsigned long ultimoBipe = 0;
unsigned long tempoBloqueioReligamento = 0; 

bool b1Anterior = false;
bool b2Anterior = false;
bool cancelaComando = false;

void setup() {
  pinMode(BTN_1, INPUT); 
  pinMode(BTN_2, INPUT);
  pinMode(LED_V, OUTPUT);
  pinMode(LED_R, OUTPUT);
  pinMode(BUZZER, OUTPUT);
  
  dht.begin();
  rtc.begin();
  lcd.init();
  lcd.off(); 
}

void animacaoLigando() {
  lcd.on();
  lcd.backlight();
  lcd.clear();
  char msg[] = "DATALOGGER...";
  for (int i = 0; i < strlen(msg); i++) {
    lcd.setCursor(i + 2, 0);
    lcd.print(msg[i]);
    delay(150);
  }
  delay(800);
  lcd.clear();
}

void desligarDispositivo() {
  ligado = false;
  
  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Desligando...");
  delay(1500); 
  
  lcd.clear();       
  lcd.noBacklight(); 
  lcd.off();    
  
  digitalWrite(LED_V, LOW);
  digitalWrite(LED_R, LOW);
  noTone(BUZZER);
  cancelaComando = true;
  
  tempoBloqueioReligamento = millis(); 
}

void loop() {

  bool b1Atual = (digitalRead(BTN_1) == HIGH); 
  bool b2Atual = (digitalRead(BTN_2) == HIGH);

  // 1. Lógica de Desligamento (Segurar ambos por 5 segundos)
  if (b1Atual && b2Atual && ligado) {
    if (tempoBotoesPressionados == 0) tempoBotoesPressionados = millis();
    if (millis() - tempoBotoesPressionados > 5000) {
      desligarDispositivo();
      tempoBotoesPressionados = 0;
    }
  } else {
    tempoBotoesPressionados = 0;
    if (!b1Atual && !b2Atual) cancelaComando = false; 
  }

  // 2. Ligar Dispositivo (Com trava de 5 segundos)
  if (!ligado) {
    
    if (b1Anterior && !b1Atual) { 
      if (millis() - tempoBloqueioReligamento > 5000) {
        animacaoLigando();
        ligado = true;
        menuOpcao = 0;
        emMedicao = false;
      }
    }
    b1Anterior = b1Atual;
    return; 
  }

  // 3. Monitoramento de Sensores (Sempre Ativo se ligado)
  float t = dht.readTemperature();
  float h = dht.readHumidity();
  int luzPerc = map(analogRead(LDR_PIN), 1023, 0, 0, 100);
  bool alerta = (t <= 15 || t >= 25 || h <= 30 || h >= 50 || luzPerc > 30);

  if (alerta) {
    digitalWrite(LED_R, HIGH);
    digitalWrite(LED_V, LOW);
    if (millis() - ultimoBipe > 3000) {
      tone(BUZZER, 1000, 100);
      ultimoBipe = millis();
    }
  } else {
    digitalWrite(LED_R, LOW);
    digitalWrite(LED_V, HIGH);
    noTone(BUZZER);
  }

  // 4. Lógica do Menu (Apenas ao SOLTAR)
  if (!cancelaComando) {
    if (b1Anterior && !b1Atual) {
      if (!emMedicao) menuOpcao = (menuOpcao + 1) % 3;
      else emMedicao = false; 
      lcd.clear();
    }
    if (b2Anterior && !b2Atual) {
      if (!emMedicao) {
        emMedicao = true;
        lcd.clear();
      }
    }
  }

  // 5. Desenho das Telas
  if (ligado) {
    if (!emMedicao) {
      lcd.setCursor(0, 0);
      lcd.print("SELECIONE:");
      lcd.setCursor(0, 1);
      if (menuOpcao == 0) lcd.print("> TEMPERATURA  ");
      else if (menuOpcao == 1) lcd.print("> UMIDADE      ");
      else if (menuOpcao == 2) lcd.print("> LUMINOSIDADE ");
    } else {
      lcd.setCursor(0, 0);
      lcd.print("VALOR ATUAL:");
      lcd.setCursor(0, 1);
      if (menuOpcao == 0) { lcd.print("Temp: "); lcd.print(t, 1); lcd.print(" C  "); }
      else if (menuOpcao == 1) { lcd.print("Umid: "); lcd.print(h, 1); lcd.print(" %  "); }
      else if (menuOpcao == 2) { lcd.print("Luz:  "); lcd.print(luzPerc); lcd.print(" %  "); }
    }
  }

  b1Anterior = b1Atual;
  b2Anterior = b2Atual;
  delay(10); 
}
