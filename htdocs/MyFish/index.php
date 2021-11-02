<?php 
	//koneksi database
	$koneksi = mysqli_connect("localhost","root","","pakan_iot");

	//uji apakah tombol simpan diklik
	if (isset($_POST['btnsimpan'])) {
		$waktu = $_POST['jam'];
		$status = 0;

		//simpa ke database
		mysqli_query($koneksi, "ALTER TABLE jadwal_makan AUTO_INCREMENT=1");
		mysqli_query($koneksi, "insert into jadwal_makan(waktu,status)values('$waktu','$status')");
	}

	//tampung data sensor jarak
	if (isset($_GET['nilai'])) {
		$sensor = $_GET['nilai'];
		mysqli_query($koneksi, "UPDATE sensor_stok set nilai ='$sensor'");
	}

	$sql = mysqli_query($koneksi,"SELECT * from sensor_stok");
	$row = mysqli_fetch_array($sql);
	$x = $row['nilai'];
	if ($x >=1200) {
		$x =0;
	}elseif ($x >=16) {
		$x =16;
	}
	$presentase = 100-($x/16*100);

 ?>
<!doctype html>
<html lang="en">

<head>
  <title>RAHAYUK IOT</title>
  <!-- Required meta tags -->
  <meta charset="utf-8">
  <meta content="width=device-width, initial-scale=1.0" name="viewport" />
  <meta http-equiv="X-UA-Compatible" content="IE=edge,chrome=1" />
  <link rel="stylesheet" type="text/css" href="assets/css/css.css">
  <link rel="stylesheet" type="text/css" href="assets/css/material-dashboard.css?v=2.1.2">
  <script type="text/javascript" src="jquery/jquery.min.js"></script>
  <script type="text/javascript" src="js/js.js"></script>
  <style type="text/css">
  	.tengah {
  		display: flex;
  		flex-direction: column;
  		justify-content: center;
  		align-items: center;
  		text-align: center;
  	}
  </style>

  <script type="text/javascript">
  	//refresh jam digital 
  	$(document).ready(function(){
  		setInterval(function(){
  			$("#datajam").load('infojam.php');
  		}, 1000); //update setiap 1 detik
  	});

  	//refresh stok pakan setiap 1 menit

  </script>

</head>

<body style="background: url(assets/img/koi.png); background-repeat: no-repeat; background-position: center; background-size:900px ">

	<div class="content">
		<div class="container-fluid tengah">
			<div style="text-align: center;">
				<h3>IOT PAKAN IKAN OTOMATIS</h3></div>
			<div style="width: 550px">
				<div class="card card-chart" style="height: auto;">
					<div class="card-header card-header-success"> 
					<i class="fas fa-fish fa-spin" style="font-size:30px"></i>
						<h5> SETTING WAKTU</h5>						
					</div>
					<div class="card-body">
						Sekarang Jam :
						<h2 style="font-weight: bold;">
							<div id="datajam"></div>
						</h2><br>
						<div class="form-grup">
							<form method="POST">
								<div>
									<input type="text" id="jam" name="jam" class="form-control" placeholder="Jam : Menit : Detik" required style="text-align: center;">
									<button type="submit" name="btnsimpan" class="btn btn-success btn-sm">
									Tambah Jadwal
									<i class="fas fa-pen"></i></button>
								</div>
							</form>
						</div><br>
						<!-- data jam dari database -->
						<table class="table table-bordered" style="text-align: center;">
							<tr style="background-color: lightgrey; color: white;">
								<th>List Jam</th>
								<th>Aksi</th>
							</tr>
							<?php 
							//baca isi tabel jadwal_makan database
								$sql = mysqli_query($koneksi, "select * from jadwal_makan order by id asc");
								while($data = mysqli_fetch_array($sql)){
							 ?>
							<tr>
								<td><?= $data['waktu']; ?></td>
								<td>
									<a href="hapus.php?id=<?= $data['id']?>">
									<i class="fa fa-trash "></i>
									</a>
								</td>
							</tr>
							<?php } ?>
						</table>

						<br>
							<h5>PERSEDIAN STOK PAKAN (
							<?= $presentase; ?>
							%)</h5>
							<div class="progress" style="height: 1px;">
							  <div class="progress-bar bg-success" role="progressbar" style="width: <?= $presentase ?>%;" aria-valuenow="25" aria-valuemin="0" aria-valuemax="100"></div>
							</div>
							<div class="progress" style="height: 20px;">
							  <div class="progress-bar bg-success" role="progressbar" style="width: <?= $presentase ?>%;" aria-valuenow="25" aria-valuemin="0" aria-valuemax="100"></div>
							</div>
						<br>
					</div>
				</div>
			</div>
		</div>
	</div>



</body>
</html>