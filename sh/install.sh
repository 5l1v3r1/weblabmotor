#!/bin/bash
sudo mkdir /tmp/weblabmotor/
sudo mkdir /var/lib/weblabmotor/
sudo chown -R www-data.www-data /var/lib/weblabmotor/
sudo chown -R www-data.www-data /tmp/weblabmotor/
sudo chmod -R 777 /tmp/weblabmotor/
sudo chmod -R 777 /var/lib/weblabmotor/

sudo chown -R www-data.www-data /var/www/graficos
sudo chown -R www-data.www-data /var/www/cgi-bin
sudo chown -R www-data.www-data /var/www/admin
sudo chown -R 755 /var/www/graficos
sudo chown -R 755 /var/www/cgi-bin
sudo chown -R 755 /var/www/admin

sudo ln -s -T /opt/weblabmotor/web/graficos /var/www/graficos
sudo ln -s -T /opt/weblabmotor/web/cgi-bin /var/www/cgi-bin
sudo ln -s -T /opt/weblabmotor/web/admin /var/www/admin
