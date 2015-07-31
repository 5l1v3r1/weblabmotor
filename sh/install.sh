#!/bin/bash

##CONFIGURACAO DAS PAGINAS WEB
ln -s -T /opt/weblabmotor/web/ /www/pages/home/
chown -R www-data.www-data /www/pages/home
chmod -R 755 /www/pages/home
