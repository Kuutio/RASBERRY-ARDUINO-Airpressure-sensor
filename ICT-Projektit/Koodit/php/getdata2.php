<html>
<head>
<title>TAMK Weather</title>
<script src="https://www.gstatic.com/charts/loader.js"></script>
<script src="skriptit.js"></script>
</head>
<div class="well">
    <h1>Tamk Weather!</h1>
</div>
<table class="table_responsive" id="ex_table">
<thead>
  <tr>
    <th>DATE_TIME</th>
    <th>PRESSURE</th>
  </tr>
</thead>
<tbody>
<?php
$configs = include('config.php');
// Create connection
$conn = new mysqli($configs['host'], $configs['username'], $configs['password']);
$sql = "SELECT * FROM dbe7vtuomi1.weather WHERE date_time >= CURDATE()
  AND date_time < CURDATE() + INTERVAL 1 DAY";

$result = $conn->query($sql);
if($result->num_rows>0){
	while($row = $result->fetch_assoc()) {
		echo '<tr>
		<td> '.$row["date_time"].'</td>
		<td> '.$row["pressure1"].'</td>
		</tr>';

}
 }else {
    echo "0 results";
}
?>
</tbody>
</table>
</html>