<?php

    $a0 = $_POST['a0'];
    $a1 = $_POST['a1'];
    $a2 = $_POST['a2'];
    $a3 = $_POST['a3'];
    $a4 = $_POST['a4'];
    $a5 = $_POST['a5'];
    
	if ($_POST['enviar'] & $_POST['ok']) {
	  exec("python /opt/weblabmotor/python/inserirDados.py /tmp/weblabmotor/temp.db $a0 $a1 $a2 $a3 $a4 $a5");
	}else if($_POST['reset'] & $_POST['ok']){
	  shell_exec("/opt/weblabmotor/sh/resetGeralBD.sh");
	}
?>