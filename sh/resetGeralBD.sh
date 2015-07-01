#!/bin/bash
rm /var/lib/weblabmotor/sensores.db
python /opt/weblabmotor/python/criarBD.py /var/lib/weblabmotor/sensores.db
chmod 777 /var/lib/weblabmotor/sensores.db
chown www-data.www-data /var/lib/weblabmotor/sensores.db
