#define BLYNK_PRINT Serial

// Blynk config
#define BLYNK_TEMPLATE_ID "TMPL6XwiQ4E-G"
#define BLYNK_TEMPLATE_NAME "DHT11new"
#define BLYNK_AUTH_TOKEN "vIoDHwsJnz4IuUoTqOhmIdD8p3DoF3yk"

#include <WiFi.h>
#include <WiFiClient.h>
#include <BlynkSimpleEsp32.h>
#include <DHT.h>

// WiFi credentials
char ssid[] = "test";
char pass[] = "12345678";

// Pin Definitions
#define DHTPIN 2
#define DHTTYPE DHT11
#define BUZZER_PIN 18

DHT dht(DHTPIN, DHTTYPE);
BlynkTimer timer;

// Fungsi untuk membaca sensor dan mengirim ke Blynk
void sendSensorData() {
  float humi = dht.readHumidity();
  float tempC = dht.readTemperature();

  if (isnan(humi) || isnan(tempC)) {
    Serial.println("Gagal membaca sensor DHT11!");
    return;
  }

  // Kirim data ke Dashboard Blynk
  Blynk.virtualWrite(V0, tempC);
  Blynk.virtualWrite(V1, humi);

  // Log ke Serial Monitor
  Serial.print("Temp: ");
  Serial.print(tempC);
  Serial.print("°C | Humidity: ");
  Serial.print(humi);
  Serial.println("%");

  // Logika Aktuator Buzzer
  if (tempC > 33.0) {
    tone(BUZZER_PIN, 102, 1000); // Bunyi jika suhu > 33°C
    Serial.println("Suhu Tinggi! Buzzer ON");
  } else {
    noTone(BUZZER_PIN);
  }
}

void setup() {
  Serial.begin(9600);
  
  dht.begin();
  pinMode(BUZZER_PIN, OUTPUT);

  // Koneksi ke Blynk
  Blynk.begin(BLYNK_AUTH_TOKEN, ssid, pass);

  // Setup timer agar pembacaan sensor dilakukan setiap 2 detik
  // Ini lebih baik daripada menggunakan delay() di dalam loop
  timer.setInterval(2000L, sendSensorData);
}

void loop() {
  Blynk.run();
  timer.run();
}