<?php
  try
  {
    //open the database
	$db = new PDO('mysql:host=localhost;dbname=sensores','root','root');

    $sql = "INSERT INTO dados (data, a0, a1, a2, a3, a4, a5)
    VALUES (current_timestamp(),'1', '2', '3', '4', '5' , '3')";

    $db->exec($sql);

    // close the database connection
    $db = NULL;
    echo "OK";
  }
  catch(PDOException $e)
  {
    print 'Exception : '.$e->getMessage();
  }  
?>