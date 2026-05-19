#include <DHT.h>
#include <LiquidCrystal.h>

#define DHTPIN 2
#define DHTTYPE DHT11
#define ledPin 3
#define LDR_PIN A0

DHT dht(DHTPIN, DHTTYPE);
LiquidCrystal lcd(8, 9, 10, 11, 12, 13);

const int sicaklikUyariEsigi = 30;  
const int sicaklikDusukEsigi = 10; 
const int nemUyariEsigi = 80;        
const int nemDusukEsigi = 30;        

void setup() {
  Serial.begin(9600);
  pinMode(ledPin, OUTPUT);
  lcd.begin(16, 2);
  lcd.print("Hava Durumu");
  delay(1000);
  dht.begin();
}

void loop() {
  int isikDegeri = analogRead(LDR_PIN);
  Serial.print("Isik Degeri: ");
  Serial.println(isikDegeri);
  delay(1000);

  bool gece = (isikDegeri < 50);
  bool gunduz = (isikDegeri > 50);

  float sicaklik = dht.readTemperature();
  int nem = dht.readHumidity();
  String havaDurumu = "";

  if (sicaklik > sicaklikUyariEsigi) {
    Serial.println("Uyari: Sicaklik cok yuksek!");
    ledBlink();
  } else if (sicaklik < sicaklikDusukEsigi) {
    Serial.println("Uyari: Sicaklik cok dusuk!");
    ledBlink();
  }

  if (nem > nemUyariEsigi) {
    Serial.println("Uyari: Nem cok yuksek!");
    ledBlink();
  } else if (nem < nemDusukEsigi) {
    Serial.println("Uyari: Nem cok dusuk!");
    ledBlink();
  }

  if (gece) {
    digitalWrite(ledPin, HIGH);
    havaDurumu = "Gece";
    Serial.println("Durum: Gece");
  } else if (gunduz) {
    digitalWrite(ledPin, LOW);
    if (isikDegeri > 150 && sicaklik > 20) {
      havaDurumu = "Gunesli";
      Serial.println("Durum: Gunesli");
    } else {
      if (nem > 50) {
        havaDurumu = "Yagmurlu";
        Serial.println("Durum: Yagmurlu");
      } else {
        havaDurumu = "Bulutlu";
        Serial.println("Durum: Bulutlu");
      }
    }
  }

  lcd.clear();
  lcd.setCursor(0, 0);
  lcd.print("Sicaklik: ");
  lcd.print(sicaklik, 1);
  lcd.print(" C");

  lcd.setCursor(0, 1);
  lcd.print("Nem: ");
  lcd.print(nem);
  lcd.print(" %");

  lcd.setCursor(0, 2);
  lcd.print("Durum: ");
  lcd.print(havaDurumu);

  Serial.print("Sicaklik: ");
  Serial.print(sicaklik);
  Serial.print(" C ");
  Serial.print("Nem: ");
  Serial.print(nem);
  Serial.print(" % ");
  Serial.print("Durum: ");
  Serial.println(havaDurumu);

  delay(1000);
}

void ledBlink() {
  for (int i = 0; i < 5; i++) {
    digitalWrite(ledPin, HIGH);
    delay(500);
    digitalWrite(ledPin, LOW);
    delay(500);
  }
}