<!DOCTYPE html>
<html>
<body>

<?php

    $a0 = $_POST['a0'];
    print ($a0);

	if ($_GET['inserir']) {
	  exec("python /opt/weblabmotor/python/inserirDados.py /tmp/weblabmotor/temp.db 23 45 67 12 45 67");
	}else if($_GET['transfer']){
	  exec("python /opt/weblabmotor/python/transferDados.py /tmp/weblabmotor/temp.db /var/lib/weblabmotor/sensores.db");
	}else if($_GET['reset']){
	  shell_exec("/opt/weblabmotor/sh/resetTempBD.sh");
	}
?>

<!-- This link will add ?run=true to your URL, myfilename.php?run=true -->
 <h1>Gerenciar dados</h1>
 <a class="btn" href="?inserir=true">INSERIR</a>
 <a href="?inserir=true">Inserir</a>
 <a href="?transfer=true">Gravar</a>
 <a href="?reset=true">Apagar</a>

<form action="" method="post">
A0   <input type="number" name="a0" min="0" max="1023"/>
A1   <input type="number" name="a1" min="0" max="1023"/>
A2   <input type="number" name="a2" min="0" max="1023"/>
A3   <input type="number" name="a3" min="0" max="1023"/>
A4   <input type="number" name="a4" min="0" max="1023"/>
A5   <input type="number" name="a5" min="0" max="1023"/>
        <input type="submit" value="Enviar" />
</form>

 <h1>Graficos</h1>
 <h3>Ultima coleta</h3>
 <iframe src="../graficos/dados.html" height="300" width="500"></iframe> 
 <h3>Todos dados</h3>
 <iframe src="../graficos/dadosGeral.html" height="300" width="500"></iframe> 

</body>
</html>