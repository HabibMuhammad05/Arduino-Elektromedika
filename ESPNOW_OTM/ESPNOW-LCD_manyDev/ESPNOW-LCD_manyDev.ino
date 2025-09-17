//================ ESPNOW LIB&VARS ===============//
#include <ESP8266WiFi.h>                          // inisialisasi library WiFi ESP8266
#include <espnow.h>                               // inisialisasi library ESPNOW

typedef struct struct_message {                   // buat struct dengan nama 'struct_message'
  float hum, temp;                                // variabel float(desimal) untuk kelembaban dan suhu
} struct_message;                                 // pengenalan struct
struct_message myData;                            // buat objek struct dengan nama 'myData'

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
  Serial.print("Bytes received: ");               // tampilkan statistik data di serial monitor
  Serial.println(len);                            // tampilkan ukuran data yang dikirimkan
  Serial.print("TEMPERATURE: ");                  // tampilkan suhu pada serial monitor
  Serial.println(myData.temp);                    // ambil data suhu dari objek myData 
  Serial.print("HUMIDITY: ");                     // tampilkan kelembaban pada serial monitor
  Serial.println(myData.hum);                     // ambil data kelembababan dari objek myData 

  lcd.clear();                                    // hapus semua karakter di lcd
  lcd.setCursor(0,0);                             // pindahkan kursor lCD ke posisi 0,0
  lcd.print("ESPNOW DATA RECV");                  // tampilkan pada lcd
  lcd.setCursor(0,1);                             // pindahkan kursor lCD ke posisi 0,1
  lcd.print("T: ");                               // tampilkan informasi suhu pada lcd
  lcd.print(myData.temp);                         // ambil data suhu dari objek myData  
  lcd.print("C");                                 // tampilkan pada lcd
  
  lcd.setCursor(8,1);                             // pindahkan kursor lCD ke posisi 0,0
  lcd.print("H: ");                               // tampilkan informasi kelembaban pada lcd
  lcd.print(myData.hum);                          // ambil data kelembaban dari objek myData
  lcd.print("%");                                 // tampilkan pada lcd
}
