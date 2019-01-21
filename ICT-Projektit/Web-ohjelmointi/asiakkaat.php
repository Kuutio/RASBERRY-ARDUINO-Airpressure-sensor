<html lang="fi">
<head>
	<?php header('Content-Type: text/html; charset=ISO-8859-1'); ?>

    <!-- meta tagit -->
    <meta charset="utf-8">
    <meta name="viewport" content="width=device-width, initial-scale=1, shrink-to-fit=no">
    <!-- Bootstrap CSS -->
    <link rel="stylesheet" href="css/bootstrap.min.css">
	<?php require_once "conf.php"; ?>
	
    <title>Asiakkaat</title>
</head>

<body>

<header class="container">
	<h1 class="text-center">Asiakastietokanta</h1>
</header>
	
<!-- Yritä luoda yhteys, anna virhe jos ei onnistu -->
<?php
$conn = mysqli_connect($servername, $username, $password, $dbname);
if (!$conn) {
	$connstatus = "Yhteyttä ei voitu muodostaa"; ?>
	
	<div class="container">
		<div class="alert alert-danger col-md-5 mx-auto text-center">
			<strong>Virhe! </strong><?php echo $connstatus; ?>
		</div>
	</div>
	
	<?php die(); 
	}
	
else {
	$connstatus = "Yhteys OK"; 
	mysqli_set_charset($conn, 'utf8'); ?>
		<div class="container">
		<div class="alert alert-success col-md-5 mx-auto text-center">
			<?php echo $connstatus; ?>
		</div>
	</div>
<?php
}

// Lähetä kysely ja luo taulukko jos dataa löytyy
$queryStr = "select * from asiakas";
$result = mysqli_query($conn, $queryStr);

if (mysqli_num_rows($result) > 0) { ?>
	<div class="container">
		<table class="table table-striped">
			<thead class="thead-dark">
				<tr><?php 
					while ($col = mysqli_fetch_field($result)) { ?>
						<th scope="col"><?php echo $col->name ?></th>
					<?php } ?>
				</tr>
			</thead>
			<tbody>
				<?php while($row = mysqli_fetch_assoc($result)) { ?>
					<tr><?php foreach ($row as $data) {
							echo '<td>'; echo $data; echo '</td>';
						} ?> 
					</tr>
				<?php } ?>
			</tbody>
		</table>
	</div>
<?php } else { ?>
	<div class="container">
		<h3>Ei löytynyt dataa</h3>
	</div>
<?php }
mysqli_close($conn);
?>

</body>