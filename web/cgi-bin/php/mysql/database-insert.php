<?php
  
    $a0 = $_GET["a0"];
    $a1 = $_GET["a1"];
    $a2 = $_GET["a2"];
    $a3 = $_GET["a3"];
    $a4 = $_GET["a4"];
    $a5 = $_GET["a5"];
    
  try
  {
    //open the database
  	$db = new PDO('mysql:host=localhost;dbname=sensores','root','root');

    $db->exec("INSERT INTO dados (data, a0, a1, a2, a3, a4, a5)
    VALUES (current_timestamp(),'$a0', '$a1', '$a2', '$a3', '$a4' , '$a5')");

    // close the database connection
    $db = NULL;
    echo "OK";
  }
  catch(PDOException $e)
  {
    print 'Exception : '.$e->getMessage();
  }  
?>