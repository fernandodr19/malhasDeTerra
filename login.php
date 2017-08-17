<?php
header("Access-Control-Allow-Origin: *");
$servername = "localhost";
$username = "root";
$password = "root";
$dbname = "mydb";

// Create connection
$conn = new mysqli($servername, $username, $password, $dbname);
// Check connection
if ($conn->connect_error) {
    die("Connection failed: " . $conn->connect_error);
} 

$username = $_POST['username'];
$password = $_POST['password'];

$sql = "SELECT * FROM USERS WHERE USER_NAME = '$username' AND USER_PASSWORD = '$password'";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    echo "Success.";
} else {
    echo "Invalid username or password.";
}

$conn->close();
?>
