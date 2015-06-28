<?php
	//if ($_GET['run']) {
	  # This code will run if ?run=true is set.
	  echo shell_exec("whoami");
	  exec("python /opt/weblabmotor/python/inserirDados.py /tmp/temp.db 23 45 67 12 45 67");
	//}
	echo 'asdasds';
?>

<!-- This link will add ?run=true to your URL, myfilename.php?run=true -->
<!--<a href="?run=true">click</a>

 h1>Dados Gerais</h1>
 <iframe src="../graficos/0/geral.html" height="300" width="500"></iframe> 
 <h1>Ultima coleta</h1>
 <iframe src="../graficos/0/index.html" height="300" width="500"></iframe> 
 -->
