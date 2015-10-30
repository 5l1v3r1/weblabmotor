<?php

  try
  {
    //open the database
	$db = new PDO('mysql:host=localhost;dbname=cer','root','root');
  $db->setAttribute(PDO::ATTR_ERRMODE, PDO::ERRMODE_EXCEPTION);


    $result = $db->query('SELECT * FROM usrTemporario');

    //nome para os dados nos graficos
    $tempo['name'] = 'TEMPO';
    $a0['name'] = 'A0';
    $a1['name'] = 'A1';
    $a2['name'] = 'A2';
    $a3['name'] = 'A3';
    $a4['name'] = 'A4';
    $a5['name'] = 'A5';

    foreach($result as $row)
    {
      $tempo['data'][] = $row['tempo'];
      $a0['data'][] = $row['a0'];
      $a1['data'][] = $row['a1'];
      $a2['data'][] = $row['a2'];
      $a3['data'][] = $row['a3'];
      $a4['data'][] = $row['a4'];
      $a5['data'][] = $row['a5'];

    }

    $rslt = array();
    array_push($rslt, $tempo);
    array_push($rslt, $a0);
    array_push($rslt, $a1);
    array_push($rslt, $a2);
    array_push($rslt, $a3);
    array_push($rslt, $a4);
    array_push($rslt, $a5);

    print json_encode($rslt, JSON_NUMERIC_CHECK);

    // close the database connection
    $db = NULL;
  }
  catch(PDOException $e)
  {
    print 'Exception : '.$e->getMessage();
  }  
?>
