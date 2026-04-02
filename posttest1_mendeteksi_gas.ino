#include <ESP8266WiFi.h>
#include <ThingerESP8266.h>

// Konfigurasi Thinger.io
#define USERNAME "arghorb"
#define DEVICE_ID "IoT_7"
#define DEVICE_CREDENTIAL "oncJPX!TdRMfH&b5"

// Masukkan SSID dan Password WiFi
const char* ssid = "bebek goreng";
const char* password = "123bebek";

// Definisi Pin LED
const int led1 = D2; 
const int led2 = D3; 
const int led3 = D4; 
const int pinSensor = A0;

ThingerESP8266 thing(USERNAME, DEVICE_ID, DEVICE_CREDENTIAL);

void setup() {
  Serial.begin(115200);

  pinMode(led1, OUTPUT);
  pinMode(led2, OUTPUT);
  pinMode(led3, OUTPUT);

  digitalWrite(led1, LOW);
  digitalWrite(led2, LOW);
  digitalWrite(led3, LOW);

  // Koneksi WiFi melalui Thinger
  thing.add_wifi(ssid, password);

  // Resource untuk Dashboard
  thing["sensor_gas"] >> [](pson& out){
      out = analogRead(pinSensor);
  };

  Serial.println("Sistem Siap. Menghubungkan ke Thinger.io...");
}

void loop() {
  thing.handle();

  int nilaiGas = analogRead(pinSensor);
  Serial.print("Nilai Gas: ");
  Serial.println(nilaiGas);

  if (nilaiGas >= 0 && nilaiGas <= 200) {
    Serial.println("Status: Rendah");
    jalankanKedip(led1, 3);
  } 
  else if (nilaiGas >= 201 && nilaiGas <= 500) {
    Serial.println("Status: Sedang");
    jalankanKedip(led2, 5);
  } 
  else if (nilaiGas > 500) {
    Serial.println("Status: Bahaya");
    jalankanKedip(led3, 7);
  }

  delay(10); 
}

void jalankanKedip(int pin, int detik) {
  int siklus = detik * 2; // on + off = 1 detik (500ms)
  for (int i = 0; i < siklus; i++) {
    digitalWrite(pin, !digitalRead(pin));
    delay(250);
    
    thing.handle(); 
  }
  digitalWrite(pin, LOW);
}