//================ ESPNOW LIB&VARS ===============//
#include <ESP8266WiFi.h>                          // inisialisasi library WiFi ESP8266
#include <espnow.h>                               // inisialisasi library ESPNOW

typedef struct struct_message {                   // buat struct dengan nama 'struct_message'
  uint8_t DEV_ID;                                 // variabel tanda pengenal setiap pengirim data
  float hum, temp;                                // variabel float(desimal) untuk kelembaban dan suhu
} struct_message;                                 // pengenalan struct

struct_message myData;                            // buat objek struct dengan nama 'myData'

struct_message device1;                           // objek dengan nama 'device1' untuk data device 1
struct_message device2;                           // objek dengan nama 'device2' untuk data device 2

//================ I2C 16X2 LCD LIB  =============//
#include <Wire.h>                                 // inisialisasi library komunikasi i2c
#include <LiquidCrystal_I2C.h>                    // inisialisasi library lcd
LiquidCrystal_I2C lcd(0x27, 16, 2);               // pembuatan objek pada library dengan nama 'lcd'
 
//================== VOID SETUP ==================//
void setup() {                                    // perulangan yang hanya akan dijalankan sekali
  Serial.begin(115200);                           // mulai komunikasi serial dengan kecepatan 115200
  lcd.begin();                                    // mulai komunikasi dengan LCD
  lcd.backlight();                                // hidupkan backlight LCD
  
  lcd.setCursor(0,0);                             // pindahkan kursor lCD ke posisi 0,0
  lcd.print("INITIALIZING....");                  // tampilkan pada LCD
  delay(1000);                                    // jeda 1000ms (1 detik)

  lcd.clear();                                    // hapus semua karakter di lcd
  lcd.setCursor(0,0);                             // pindahkan kursor lCD ke posisi 0,0
  lcd.print("ESPNOW DATA RECV");                  // tampilkan pada LCD
  lcd.setCursor(0,1);                             // pindahkan kursor lCD ke posisi 0,1
  lcd.print("WAITING DATA...");                   // tampilkan pada LCD
  
  WiFi.mode(WIFI_STA);                            // ganti mode wifi ESP menjadi mode Station

  if (esp_now_init() != 0) {                      // pengecekan jika espnow gagal inisialisasi
    Serial.println("Error initializing ESP-NOW"); // tampilkan pesan ESPNOW gagal
    return;                                       // kembali lagi ke awal program
  }
  
  esp_now_set_self_role(ESP_NOW_ROLE_SLAVE);      // atur peran sebagai SLAVE(Penerima data)
  esp_now_register_recv_cb(OnDataRecv);           // atur fungsi callback ketika ada data diterima
}

//================= VOID LOOP ====================//
void loop() {                                     // perulangan yang akan dijalankan selamanya
}

void OnDataRecv(uint8_t * mac, uint8_t *incomingData, uint8_t len) {  // fungsi ketika data diterima
  memcpy(&myData, incomingData, sizeof(myData));  // salin data yang diterima ke objek struct 'myData' 
  
  if(myData.DEV_ID == 1) device1 = myData;        // jika DEV_ID adalah 1, salin data ke struct device1
  else if(myData.DEV_ID == 2) device2 = myData;   // jika DEV_ID adalah 2, salin data ke struct device2
  
  Serial.print("received from DEV_ID: ");         // tampilkan sumber data di serial monitor
  Serial.println(myData.DEV_ID);                  // tampilkan darimana data diterima
  Serial.print("TEMPERATURE: ");                  // tampilkan suhu pada serial monitor
  Serial.println(myData.temp);                    // ambil data suhu yang diterima 
  Serial.print("HUMIDITY: ");                     // tampilkan kelembaban pada serial monitor
  Serial.println(myData.hum);                     // ambil data kelembababan yang diterima
  Serial.println("==== end of received data ====\n\n");// tampilkan pada serial monitor

  lcd.clear();                                    // hapus semua karakter di lcd
  lcd.setCursor(0,0);                             // pindahkan kursor lCD ke posisi 0,0
  lcd.print("T1:");                               // tampilkan informasi suhu device 1 pada lcd
  lcd.print(device1.temp);                        // ambil data suhu dari objek device1  
  lcd.print("C");                                 // tampilkan pada lcd
  lcd.setCursor(8,0);                             // pindahkan kursor lCD ke posisi 8,0
  lcd.print("H1:");                               // tampilkan informasi kelembaban device 1
  lcd.print(device1.hum);                         // ambil data kelembaban dari objek device1
  lcd.print("%");                                 // tampilkan pada lcd

  lcd.setCursor(0,1);                             // pindahkan kursor lCD ke posisi 0,1
  lcd.print("T2:");                               // tampilkan informasi suhu device 2 pada lcd
  lcd.print(device2.temp);                        // ambil data suhu dari objek device2  
  lcd.print("C");                                 // tampilkan pada lcd
  lcd.setCursor(8,1);                             // pindahkan kursor lCD ke posisi 8,1
  lcd.print("H2:");                               // tampilkan informasi kelembaban device 2
  lcd.print(device2.hum);                         // ambil data kelembaban dari objek device2
  lcd.print("%");                                 // tampilkan pada lcd
}
