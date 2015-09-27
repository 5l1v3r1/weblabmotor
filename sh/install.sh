#!/bin/sh

option="${1}" 
case ${option} in 
   -y) FILE="${2}" 
      echo "Configurando diretorios de acesso web para linux yocto"
        ln -s -T /opt/weblabmotor/web/ /www/pages/home
        chown -R www-data.www-data /www/pages/home
        chmod -R 755 /www/pages/home
      ;; 
   -d) DIR="${2}" 
      echo "Configurando diretorios de acesso web para linux debian like"
          sudo ln -s -T /opt/weblabmotor/web/ /var/www/home
          sudo chown -R www-data.www-data /var/www/home
          sudo chmod -R 755 /var/www/home/
      ;;
      -c) DIR="${2}" 
   echo "Configurando diretorios de acesso web para RPi"
       ln -s -T /opt/weblabmotor/web/ /var/www/home
       chown -R www-data.www-data /var/www/home
       chmod -R 755 /var/www/home/
   ;;
    -i) DIR="${2}" 
        echo "instalando pacotes para linux debian like"
        sudo apt-get update
        sudo apt-get install php5 apache2 mysql-server php5-mysql phpmyadmin
      ;;
    -r) DIR="${2}" 
        echo "instalando pacotes para Rpi"
        apt-get update
        apt-get install php5 apache2 mysql-server php5-mysql phpmyadmin
      ;;
   *)  
      echo "`basename ${0}`:usage: [-y yocto] | [-d debianLike] [-i installDebianLike] [-r installRpi]" 
      exit 1 # Command to come out of the program with status 1
      ;; 
esac 
