<?php

#Basic Line
require '../../conn/connection-teste.php';

$result = mysql_query("SELECT date AS DATA, a0 AS A0, a1 as A1 FROM sensores WHERE date >= NOW() - INTERVAL 20 MINUTE");

#"SELECT id, year AS TAHUN, month AS BULAN, COUNT( * ) AS JUMLAH FROM activities WHERE year=2012 GROUP BY MONTH ORDER BY id"
#"SELECT * FROM `cna` WHERE `data` >= NOW() - INTERVAL 20 MINUTE;"
#SELECT date AS DATA, a0 AS A0, a1 as A1 FROM sensores WHERE date >= NOW() - INTERVAL 20 MINUTE

$bln = array();
$bln['name'] = 'DATA';
$rows['name'] = 'TEMP';
while ($r = mysql_fetch_array($result)) {
    $bln['data'][] = $r['DATA'];
    $rows['data'][] = $r['A0'];
}
$rslt = array();
array_push($rslt, $bln);
array_push($rslt, $rows);
print json_encode($rslt, JSON_NUMERIC_CHECK);

mysql_close($con);


