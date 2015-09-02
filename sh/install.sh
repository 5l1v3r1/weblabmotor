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
          sudo chmod -R 755 /var/www/home
      ;; 
   *)  
      echo "`basename ${0}`:usage: [-y yocto] | [-d debianLike]" 
      exit 1 # Command to come out of the program with status 1
      ;; 
esac 