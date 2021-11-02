//librali pendukung
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h>
#include <WiFiClient.h>


// deklarasi pin nodemcu
#define echo D1                     //pin echo sensor jarak di pin D1 /GPIO-05
#define triger D2                   //pin triger sensor jarak di pin D2 / GPIO-04
#define buzer D7                    //buzzer di pin D7 / GPIO-13


float stok;
const char* ssid = "Ayu kotabanda";                           //nama wifi yang mau di koneksikan ke NODEMCU
const char* pass = "alhamdulillah2021";                       //pasword wifi
const char* host = "http://pakanikaniot.kontrollampu.xyz";          //alamat host server
Servo servo;                                                  //kelas Servo di jadikan objeck servo


//awal function
float cek_pakan() {
  float durasi, jarak;
  digitalWrite(triger, LOW); delayMicroseconds(2);
  digitalWrite(triger, HIGH); delayMicroseconds(2);
  digitalWrite(triger, LOW);
  durasi = pulseIn(echo, HIGH);
  jarak = (durasi / 2) / 29.1;
  return jarak;
}

void beri_makan() {
  tone(buzer, 400);
  delay(200);
  noTone(buzer);
  tone(buzer, 600);
  delay(200);
  noTone(buzer);
  tone(buzer, 900);
  delay(500);
  noTone(buzer);
  int banyak = 10;
  for (int i = 0; i <= banyak; i++) {
    servo.write(180);
    delay(400);
    servo.write(0);
    delay(400);
  }


}

// akhir function()

void setup() {

  Serial.begin(9600);                       //set potret tampilan serial arduino
  pinMode(buzer, OUTPUT);                   //set buzer jadi output
  pinMode(echo, INPUT);                     //set echo ultrasonik jadi input
  pinMode(triger, OUTPUT);                  //set triger ultrasonik jadi output
  servo.attach(12);                         //set servo terpasang pada pin data GPIO-12 NODEMCU / pin D6
  servo.write(0);                           //saat mulai posisikan servo pada 0 derajat (botol pakan tertutup)


  WiFi.hostname("IOT ESP8266 by Rahayu");
  WiFi.begin(ssid, pass);                    //mulai melakukan koneksi ke wifi
  while (WiFi.status() != WL_CONNECTED) {    //jika status wifi tidak konek maka akan berulang di While sampai konek
    Serial.print("*");
    delay(1000);
  }
  Serial.println("");
  tone(buzer, 800);
  delay(200);
  noTone(buzer);
  Serial.print("Berhasil Terhubung ke ");
  Serial.println(ssid);
  tone(buzer, 1000, 2000);
  //bila berhasil terkoneksi makaka akan keluar dari while di atas dan program di jalankan

}

void loop() {

  //baca koneksi ke server
  WiFiClient client;
  Serial.println(host);
  if (!client.connect(host, 80)) {                                //bila tidak kodek ke host di port 80 maka akan berulang di IF
    Serial.println("Koneksi ke server bermasalah");
    return;
  }

  //keluar dari if karna berhasil konek ke server
  String Link;                                                    //variabel penampung data yang akan dirikwes k server
  HTTPClient http;                                                //deklarasikan kelas httpclient ke obyek
  Link = String(host) + "/bacadata.php";
  Serial.println(Link);
  Serial.println("/n");

  //eksekusi linknya
  int hasilkirim1 = http.GET();
  Serial.println(hasilkirim1);

  //baca feedback
  String hasilrikwes = http.getString();                           //variabel penampung balasan server
  Serial.println(hasilrikwes);

  //kondisi setelah menerima hasil rikwes dari server
  if (hasilrikwes == "Beri_Makan") {                                //bila kondisi balasan server = Beri_Makan
    beri_makan();                                                   //panggil fungsi beri makan untuk memberi pakan ikan

    //setelah berhasil memberikan makan kembalikan status jadwal_makan ke 0
    Link = String(host) + "/ubahstatus.php";
    Serial.println(Link);
    Serial.println("/n");
    http.begin(client, Link);
    int hasilkirim2 = http.GET();
    Serial.println(hasilkirim2);


    //setelah itu sistem akan mengirimkan data Stok pakan dari sensor ke database
    stok = cek_pakan();
    Link = String(host) + "/index.php?nilai=" + String(stok);
    http.begin(client, Link);
    Serial.println(Link);
    Serial.println("/n");
    int hasilkirim3 = http.GET();
    Serial.println(hasilkirim3);
  }

  delay(2000);

  //cek apakah pakan sudah mau habis, bila menipus alaram berbunyi

  if (stok >= 1200) {               //kondisikan sensor dalam jarak terlalu dekat akan eror dan bernilai > 12000
    Serial.println("FULL AHHH....!");
  } else if (stok >= 16) {
    Serial.println("PAKAN MENIPIS....!");
    tone(buzer, 100);
    delay(100);
    tone(buzer, 200);
    delay(200);
    noTone(buzer);
  }
}
