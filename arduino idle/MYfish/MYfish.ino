//librali pendukung
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <Servo.h>
#include <WiFiClient.h>


// deklarasi pin nodemcu
#define echo D3                     //pin echo sensor jarak di pin D3 / GPIO0
#define triger D4                   //pin triger sensor jarak di pin D4 /GPIO2
#define buzer D6                    //buzzer di pin D6 / GPIO12


float stok;
const char* ssid = "PakaIOT";                           //nama wifi yang mau di koneksikan ke NODEMCU
const char* pass = "pakanikanv1";                           //pasword wifi
const char* host = "192.168.43.147";                        //alamat host server
Servo servo;                                             //kelas Servo di jadikan objeck servo


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
  
  int banyak = 5;
  
  for (int i = 0; i <= banyak; i++) {
    servo.write(180);
    delay(350);
    servo.write(0);
    delay(350);
  }
}

// akhir function()

void setup() {

  Serial.begin(9600);             //set potret tampilan serial arduino
  pinMode(buzer, OUTPUT);         //set buzer jadi output
  pinMode(echo, INPUT);           //set echo ultrasonik jadi input
  pinMode(triger, OUTPUT);        //set triger ultrasonik jadi output
  servo.attach(14);               //set servo terpasang pada pin data GPIO14 NODEMCU / pin D5
  servo.write(0);                 //saat mulai posisikan servo pada 0 derajat (botol pakan tertutup)


  WiFi.hostname("IOT ESP8266 by Rahayu");
  WiFi.begin(ssid, pass);                    //mulai melakukan koneksi ke wifi
  while (WiFi.status() != WL_CONNECTED) {    //jika status wifi tidak konek maka akan berulang di While sampai konek
    Serial.print("*");
    delay(1000);
  }
  Serial.println("");
  Serial.print("Berhasil Terhubung ke ");
  Serial.println(ssid);
  //bila berhasil terkoneksi makaka akan keluar dari while di atas dan program di jalankan

}

void loop() {

  //baca koneksi ke server
  WiFiClient client;
  if (!client.connect(host, 80)) {                      //bila tidak kodek ke host di port 80 maka akan berulang di IF
    Serial.println("Koneksi ke server bermasalah");
    return;
  }

  //keluar dari if karna berhasil konek ke server
  String Link;                                                    //variabel penampung data yang akan dirikwes k server
  HTTPClient http;                                                //deklarasikan kelas httpclient ke obyek
  Link = "http://" + String(host) + "/MyFish/bacadata.php";

  //eksekusi linknya
  http.begin(client, Link);                                       //bagian exekusi link rikwes ke server (nilai dikirim ke server)
  http.GET();

  //baca feedback
  String hasilrikwes = http.getString();                           //variabel penampung balasan server
  Serial.println(hasilrikwes);

  //kondisi setelah menerima hasil rikwes dari server
  if (hasilrikwes == "Beri_Makan") {                                //bila kondisi balasan server = Beri_Makan
    beri_makan();                                                   //panggil fungsi beri makan untuk memberi pakan ikan

    //setelah berhasil memberikan makan kembalikan status jadwal_makan ke 0
    Link = "http://" + String(host) + "/MyFish/ubahstatus.php";
    http.begin(client, Link);
    http.GET();

    //setelah itu sistem akan mengirimkan data Stok pakan dari sensor ke database
    stok = cek_pakan();
    Link = "http://" + String(host) + "/MyFish/index.php?nilai=" + String(stok);
    http.begin(client, Link);
    http.GET();
    Serial.println(stok);
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
