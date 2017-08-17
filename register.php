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

$sql = "SELECT * FROM USERS WHERE USER_NAME = '$username'";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    echo "Email já cadastrado";
} else {
    $sql = "INSERT INTO USERS VALUES('$username', '$password')";
    if ($conn->query($sql) == TRUE) {
        echo "Usuário criado com sucesso.";
    } else {
        echo "Error: " . $sql . "<br>" . $conn->error;
    }
}

$conn->close();
?>
