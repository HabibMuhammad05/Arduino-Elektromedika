//================ ESPNOW LIB&VARS ===============//
#include <ESP8266WiFi.h>                          // masukkan library WiFi ESP8266
#include <espnow.h>                               // masukkan library ESPNOW

typedef struct struct_message {                   // buat struct dengan nama 'struct_message'
  uint8_t DEV_ID = 1;                             // variabel tanda pengenal setiap pengirim data
  float hum, temp;                                // variabel float(desimal) untuk kelembaban dan suhu
} struct_message;                                 // pengenalan struct
struct_message myData;                            // buat objek struct dengan nama 'myData'

uint8_t broadcastAddress[] = {0xA4, 0xCF, 0x12, 0xFC, 0xA3, 0xCC}; // ALAMAT MAC PENERIMA

//================ DHT11 LIB&VARS ================//
#include <DHT.h>                                  // masukkan library DHT
DHT dht(14, DHT11);                               // buat objek sensor dengan nama DHT pada pin GPIO14              

//================== OTHER VARS ==================//
unsigned long lastTime = 0;                       // variabel untuk menyimpan millis() sementara
unsigned long timerDelay = 500;                   // variabel yang mengontrol jeda setiap pengiriman
 
//================== VOID SETUP ==================//
void setup() {                                    // perulangan yang hanya akan dijalankan sekali
  Serial.begin(115200);                           // mulai komunikasi serial dengan kecepatan 115200
  dht.begin();                                    // nulai komunikasi dengan sensor DHT
  pinMode(LED_BUILTIN, OUTPUT);                   // gunakan led internal sebagai output untuk indikator
  WiFi.mode(WIFI_STA);                            // ganti mode wifi ESP menjadi mode Station
  
  if (esp_now_init() != 0) {                      // pengecekan jika espnow gagal inisialisasi
    Serial.println("Error initializing ESP-NOW"); // tampilkan pesan ESPNOW gagal
    return;                                       // kembali lagi ke awal program
  }
  esp_now_set_self_role(ESP_NOW_ROLE_CONTROLLER); // atur peran sebsgai CONTROLLER(pengirim data sensor)
  esp_now_register_send_cb(OnDataSent);           // atur fungsi callback untuk data yang akan dikirimkan
  esp_now_add_peer(broadcastAddress, ESP_NOW_ROLE_SLAVE, 1, NULL, 0);// tambahkan perangkat penerima data
                 //(alamat MAC penerima,peran penerima,wifi ch1,tanpa enskripsi,panjang enskripsi 0)
}
 
//================== VOID LOOP ===================//
void loop() {                                     // perulangan yang akan dijalankan selamanya
  if ((millis() - lastTime) > timerDelay) {       // jeda millis setiap interval 2000ms(2 detik)
    myData.hum = dht.readHumidity();              // baca data kelembaban dan masukkan ke variabel objek
    myData.temp = dht.readTemperature();          // baca data suhu dan masukkan ke variabel objek
    
    esp_now_send(broadcastAddress, (uint8_t *) &myData, sizeof(myData));// kirim data dengan informasi
                 //(alamat penerima, pointer struct, ukuran struct) 
    lastTime = millis();                          // simpan nilai millis saat ini
  }
}

void OnDataSent(uint8_t *mac_addr, uint8_t sendStatus) {// fungsi callback ketika data dikirimkan 
  Serial.print("Last Packet Send Status: ");      // tampilkan pada serial monitor
  if (sendStatus == 0){                           // jika status pengiriman berhasil
    Serial.println("Delivery success");           // tampilkan sukses pada serial monitor
    digitalWrite(LED_BUILTIN, !digitalRead(LED_BUILTIN)); //flip flop LED jika sukses
  }
  else{                                           // jika status pengiriman gagal
    Serial.println("Delivery fail");              // tampilkan gagal pada serial monitor
    digitalWrite(LED_BUILTIN, HIGH);              // matikan LED jika data gagal dikirim
  }
}
