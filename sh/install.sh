#!/bin/bash
mkdir /tmp/weblabmotor/
mkdir /var/lib/weblabmotor/
chown -R www-data.www-data /var/lib/weblabmotor/
chown -R www-data.www-data /tmp/weblabmotor/
chmod -R 777 /tmp/weblabmotor/
chmod -R 777 /var/lib/weblabmotor/

chown -R www-data.www-data /var/www/graficos
chown -R www-data.www-data /var/www/cgi-bin
chown -R www-data.www-data /var/www/admin
chown -R 755 /var/www/graficos
chown -R 755 /var/www/cgi-bin
chown -R 755 /var/www/admin

ln -s -T /opt/weblabmotor/web/graficos /var/www/graficos
ln -s -T /opt/weblabmotor/web/cgi-bin /var/www/cgi-bin
ln -s -T /opt/weblabmotor/web/admin /var/www/admin