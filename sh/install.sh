#!/bin/bash
python /opt/weblabmotor/python/criarBD.py /tmp/temp.db
python /opt/weblabmotor/python/criarBD.py /var/lib/weblabmotor/sensores.db
chmod 777 /tmp/temp.db
chmod 777 /var/lib/weblabmotor/sensores.db
chown -R www-data.www-data /var/lib/weblabmotor/
chown www-data.www-data /tmp/temp.db
ln -s -T /opt/weblabmotor/html/ /var/www/weblabmotor
apt-get install mysql-server mysql-common mysql-client php5-mysql php5-cgi php5-sqlite libapache2-mod-php5 sqlite3
service apache2 restart
