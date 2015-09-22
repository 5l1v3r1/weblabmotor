<?php
  try
  {
    //open the database
	$db = new PDO('mysql:host=localhost;dbname=sensores','root','root');

    $result = $db->query('SELECT * FROM dados');

    $bln['name'] = 'DATA';
    $rows['name'] = 'A0';
    $rowss['name'] = 'A1';
    foreach($result as $row)
    {
      $bln['data'][] = $row['data'];
      $rows['data'][] = $row['a0'];
      $rowss['data'][] = $row['a1'];
    }

    $rslt = array();
    array_push($rslt, $bln);
    array_push($rslt, $rows);
    array_push($rslt, $rowss);

    print json_encode($rslt, JSON_NUMERIC_CHECK);

    // close the database connection
    $db = NULL;
  }
  catch(PDOException $e)
  {
    print 'Exception : '.$e->getMessage();
  }  
?>
