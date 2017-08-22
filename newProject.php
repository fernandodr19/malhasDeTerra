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

$pName = $_POST['pName'];
$username = $_POST['username'];

$sql = "SELECT * FROM PROJECTS WHERE PNAME = '$pName' AND USER_NAME= '$username'";
$result = $conn->query($sql);

if ($result->num_rows > 0) {
    echo "Já existe um projeto com este nome.";
} else {
    $sql = "INSERT INTO PROJECTS VALUES('$pName', '$username')";
    if ($conn->query($sql) == TRUE) {
        echo "Projeto criado com sucesso.";
    } else {
        echo "Error: " . $sql . "<br>" . $conn->error;
    }
}

$conn->close();
?>
