<?php 

	//koneksi database
	$koneksi = mysqli_connect("localhost","root","","pakan_iot");

	// cek status jam yang bernilai 1
	$sql = mysqli_query($koneksi, "select * from jadwal_makan where status=1");

	//jika ada data status yang bernilai 1, maka infokan ke node mcu waktunya beri pakan
	if (mysqli_num_rows($sql) > 0) {
		echo "Beri_Makan";
	}




 ?>