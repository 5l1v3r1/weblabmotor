<?php
  
    $a0 = $_GET["a0"];
    $a1 = $_GET["a1"];
    $a2 = $_GET["a2"];
    $a3 = $_GET["a3"];
    $a4 = $_GET["a4"];
    $a5 = $_GET["a5"];
    $a6 = $_GET["a6"];
    $a7 = $_GET["a7"];
    $a8 = $_GET["a8"];
    $a9 = $_GET["a9"];
  try
  {
    
    //open the database
    $conn = new PDO('mysql:host=localhost;dbname=cer','root','root');

    $conn->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);

    //$db->exec("INSERT INTO usr (data, a0, a1, a2, a3, a4, a5)
    //VALUES (current_timestamp(),'$a0', '$a1', '$a2', '$a3', '$a4' , '$a5')");

    $stmt = $conn->prepare("INSERT INTO usrTemporario ( a0, a1, a2, a3, a4, a5, a6, a7, a8, a9)
    VALUES (:a0, :a1, :a2, :a3, :a4, :a5, :a6, :a7, :a8, :a9)");
    $stmt->bindParam(':a0', $a0);
    $stmt->bindParam(':a1', $a1);
    $stmt->bindParam(':a2', $a2);
    $stmt->bindParam(':a3', $a3);
    $stmt->bindParam(':a4', $a4);
    $stmt->bindParam(':a5', $a5);
    $stmt->bindParam(':a5', $a5);
    $stmt->bindParam(':a5', $a5);

    $stmt->execute();

    // close the database connection
    $db = NULL;
    echo "OK";
  }
  catch(PDOException $e)
  {
    print 'Exception : '.$e->getMessage();
  }  
?>
