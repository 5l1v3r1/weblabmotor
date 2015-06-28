#!/bin/bash

ln -s /opt/weblabmotor/html/ /var/www/weblabmotor
sudo apt-get install mysql-server mysql-common mysql-client php5-mysql php5-sqlite libapache2-mod-php5 sqlite3
sudo service apache2 restart
