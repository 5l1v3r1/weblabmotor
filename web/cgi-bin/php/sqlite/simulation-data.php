<?php

    $a0 = escapeshellarg($_POST['a0']);
    $a1 = escapeshellarg($_POST['a1']);
    $a2 = escapeshellarg($_POST['a2']);
    $a3 = escapeshellarg($_POST['a3']);
    $a4 = escapeshellarg($_POST['a4']);
    $a5 = escapeshellarg($_POST['a5']);
    
	if ($_POST['enviar'] & $_POST['ok']) {
	  exec("python /opt/weblabmotor/python/inserirDados.py /tmp/weblabmotor/temp.db $a0 $a1 $a2 $a3 $a4 $a5");
	}else if($_POST['reset'] & $_POST['ok']){
	  shell_exec("/opt/weblabmotor/sh/resetGeralBD.sh");
	}
?>
