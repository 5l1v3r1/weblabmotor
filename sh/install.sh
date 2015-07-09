#!/bin/bash
mkdir /tmp/weblabmotor/
mkdir /var/lib/weblabmotor/
chown -R www-data.www-data /var/lib/weblabmotor/
chown -R www-data.www-data /tmp/weblabmotor/
python /opt/weblabmotor/python/criarBD.py /tmp/weblabmotor/temp.db
python /opt/weblabmotor/python/criarBD.py /var/lib/weblabmotor/sensores.db
chmod -R 777 /tmp/weblabmotor/
chmod -R 777 /var/lib/weblabmotor/
ln -s -T /opt/weblabmotor/html/ /var/www/weblabmotor
apt-get install mysql-server mysql-common mysql-client php5-mysql php5-cgi php5-sqlite libapache2-mod-php5 sqlite3