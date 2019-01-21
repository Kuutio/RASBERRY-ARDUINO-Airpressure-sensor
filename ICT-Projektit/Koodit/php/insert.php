 <?php
$configs = include('config.php');

// Create connection
$conn = new mysqli($configs['host'], $configs['username'], $configs['password']);

// Check connection
if ($conn->connect_error) {
	echo "Connection error!";
    die("Connection failed: " . $conn->connect_error);
}
//echo "Connected successfully"."<br>";
mysql_select_db('dbe7vtuomi1',$conn);

if(isset($_GET['temperature1'])){
	echo "ISSET";
   $temperature1 = $_GET['temperature1'];
}else{echo "ISNOTSET"; $temperature1 = null;}

if(isset($_GET['temperature2'])){
   $temperature2 = $_GET['temperature2'];
}else{$temperature2 = null;}

if(isset($_GET['wind_speed1'])){
   $wind_speed1 = $_GET['wind_speed1'];
}else{$wind_speed1 = null;}

if(isset($_GET['wind_speed2'])){
   $wind_speed2 = $_GET['wind_speed2'];
}else{$wind_speed2 = null;}

if(isset($_GET['wind_direction1'])){
   $wind_direction1 = $_GET['wind_direction1'];
}else{$wind_direction1 = null;}

if(isset($_GET['wind_direction2'])){
   $wind_direction2 = $_GET['wind_direction2'];
}else{$wind_direction2 = null;}

if(isset($_GET['pressure1'])){
   $pressure1 = $_GET['pressure1'];
}else{$pressure1 = null;}

if(isset($_GET['pressure2'])){
   $pressure2 = $_GET['pressure2'];
}else{$pressure2 = null;}

if(isset($_GET['humidity_in'])){
   $humidity_in = $_GET['humidity_in'];
}else{$humidity_in = null;}

if(isset($_GET['humidity_out'])){
   $humidity_out = $_GET['humidity_out'];
}else{$humidity_out = null;}

if(isset($_GET['light1'])){
   $light1 = $_GET['light1'];
}else{$light1 = null;}

if(isset($_GET['light2'])){
   $light2 = $_GET['light2'];
}else{$light2 = null;}

if(isset($_GET['rain1'])){
   $rain1 = $_GET['rain1'];
}else{$rain1 = null;}

if(isset($_GET['rain2'])){
   $rain2 = $_GET['rain2'];
}else{$rain2 = null;}


$sql = "INSERT INTO dbe7vtuomi1.weather (temperature1, temperature2, wind_speed1, wind_speed2,
 wind_direction1, wind_direction2, pressure1, pressure2, humidity_in, humidity_out, light1, light2, rain1, rain2)
VALUES ('$temperature1', '$temperature2', '$wind_speed1','$wind_speed2','$wind_direction1','$wind_direction2',
'$pressure1','$pressure2','$humidity_in','$humidity_out','$light1','$light2','$rain1','$rain2')";
 
 if ($conn->query($sql) === TRUE) {
    echo "New record created successfully";
} else {
    echo "Error: " . $sql . "<br>" . $conn->error;
}
?> 