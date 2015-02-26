#!/bin/bash
mkdir /tmp/weblabmotor/
mkdir /var/lib/weblabmotor/
chown -R www-data.www-data /var/lib/weblabmotor/
chown -R www-data.www-data /tmp/weblabmotor/
chmod -R 777 /tmp/weblabmotor/
chmod -R 777 /var/lib/weblabmotor/

ln -s -T /opt/weblabmotor/web/graficos /www/pages/graficos    
ln -s -T /opt/weblabmotor/web/cgi-bin /www/pages/cgi-bin    
ln -s -T /opt/weblabmotor/web/admin /www/pages/admin   

chown -R www-data.www-data /www/pages/graficos
chown -R www-data.www-data /www/pages/cgi-bin
chown -R www-data.www-data /www/pages/admin
chown -R 755 /www/pages/graficos
chown -R 755 /www/pages/cgi-bin
chown -R 755 /www/pages/admin

chown -R www-data.www-data /opt/weblabmotor/web/
chown -R 755 /opt/weblabmotor/web/

