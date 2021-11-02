<?php 
	//koneksi database
	$koneksi = mysqli_connect("localhost","root","","pakan_iot");

	$id = $_GET['id'];
	//hapus data
	mysqli_query($koneksi,"delete from jadwal_makan where id='$id'");

		echo"
		<script>
			alert('Jadwal Berhasil di hapus.. !');
			document.location.href = 'index.php';
		</script>

		";



?>