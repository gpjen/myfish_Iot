<?php 

	//koneksi database
	$koneksi = mysqli_connect("localhost","root","","pakan_iot");

	//update status Semuanya menjadi 0
	mysqli_query($koneksi, "update jadwal_makan set status=0");



 ?>