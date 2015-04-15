<?php
$con = mysql_connect("localhost","root","root");

if (!$con) {
  die('Could not connect: ' . mysql_error());
}

mysql_select_db("test", $con);

$result = mysql_query("SELECT * FROM exp");

$rows = array();
while($r = mysql_fetch_array($result)) {
    $row[0] = $r[0];
    $row[1] = $r[1];
    array_push($rows,$row);
}

print json_encode($rows, JSON_NUMERIC_CHECK);

mysql_close($con);
?>
