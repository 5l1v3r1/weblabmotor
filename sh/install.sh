#!/bin/bash

ln -s /opt/weblabmotor/graficos/ /var/www/graficos
sudo apt-get install mysql-server mysql-common mysql-client php5-mysql libapache2-mod-php5 sqlite3
sudo service apache2 restart
