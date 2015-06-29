<!DOCTYPE html>
<html>
<body>

<?php
	if ($_GET['inserir']) {
	  exec("python /opt/weblabmotor/python/inserirDados.py /tmp/weblabmotor/temp.db 23 45 67 12 45 67");
	}else if($_GET['transfer']){
	  exec("python /opt/weblabmotor/python/transferDados.py /tmp/weblabmotor/temp.db /var/lib/weblabmotor/sensores.db");
	}else if($_GET['reset']){
	  exec("./opt/weblabmotor/sh/resetBD.db");
	}
?>

<!-- This link will add ?run=true to your URL, myfilename.php?run=true -->
 <h2> Gerenciar dados  </h2>
 <a href="?inserir=true">Inserir</a>
 <a href="?transfer=true">Gravar</a>
 <a href="?reset=true">Apagar</a>

 <h1>Graficos</h1>
 <h3>Ultima coleta</h3>
 <iframe src="../graficos/dados.html" height="300" width="500"></iframe> 
 <h3>Todos dados</h3>
 <iframe src="../graficos/dadosGeral.html" height="300" width="500"></iframe> 
 

</body>
</html>