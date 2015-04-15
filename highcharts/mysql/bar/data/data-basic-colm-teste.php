<?php

#Basic Line
require '../../conn/connection-teste.php';

$result = mysql_query();

#"SELECT id, year AS TAHUN, month AS BULAN, COUNT( * ) AS JUMLAH FROM exp WHERE year=2012 GROUP BY MONTH ORDER BY id"
#"SELECT * FROM `cna` WHERE `data` >= NOW() - INTERVAL 20 MINUTE;"

$bln = array();
$bln['name'] = 'Bulan';
$rows['name'] = 'Jumlah Pelawat';
while ($r = mysql_fetch_array($result)) {
    $bln['data'][] = $r['BULAN'];
    $rows['data'][] = $r['JUMLAH'];
}
$rslt = array();
array_push($rslt, $bln);
array_push($rslt, $rows);
print json_encode($rslt, JSON_NUMERIC_CHECK);

mysql_close($con);


