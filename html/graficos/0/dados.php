<?php
/*
	//conexao mysql

	$con = mysql_connect("localhost","root","root");

	if (!$con) {
	  die('Could not connect: ' . mysql_error());
	}

	mysql_select_db("test", $con);
*/

	//conexao sqlite
  try
  {
    //open the database
    $db = new PDO('sqlite:../database/temp.db');

    $result = $db->query('SELECT * FROM sensores');

    $bln['name'] = 'DATA';
    $rows['name'] = 'A0';
    $rowss['name'] = 'A1';
    foreach($result as $row)
    {
      $bln['data'][] = $row['date'];
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
