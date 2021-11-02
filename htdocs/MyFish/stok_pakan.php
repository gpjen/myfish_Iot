<?php 

	//koneksi database
	$koneksi = mysqli_connect("localhost","root","","pakan_iot");
	//tampung nilai yang dikrim nodemcu
	$sensor = $_GET['nilai'];

	mysqli_query($koneksi, "UPDATE sensor_stok set nilai ='$sensor'");


 ?>

