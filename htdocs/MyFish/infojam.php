<?php 
	//koneksi database
	$koneksi = mysqli_connect("localhost","root","","pakan_iot");

//tampilkan jam otomatis secara realtime
date_default_timezone_set("Asia/Jayapura");
$jam = date('H:i:s');
echo $jam;

//cek jadwal sekarang dengan jadwal yang sudah di seting dari database

$sql = mysqli_query($koneksi, "select * from jadwal_makan order by id asc");

while($data = mysqli_fetch_array($sql)){
	$id = $data['id'];
	$jamSet = $data['waktu'];
	//bandingkan dengan jam sekarang
	if($jam == $jamSet){
		//updet status jam dalam database bernilai 1
		mysqli_query($koneksi,"update jadwal_makan set status=1 where id='$id'");
	}
}


 ?>