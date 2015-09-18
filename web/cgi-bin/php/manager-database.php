<?php
    if($_POST['transfer']){
	  exec("python /opt/weblabmotor/python/transferDados.py /tmp/weblabmotor/temp.db /var/lib/weblabmotor/sensores.db");
	}else if($_POST['reset']){
	  shell_exec("/opt/weblabmotor/sh/resetTempBD.sh");
	}
?>
