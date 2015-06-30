#!/bin/bash
rm /tmp/weblabmotor/temp.db
python /opt/weblabmotor/python/criarBD.py /tmp/weblabmotor/temp.db
chmod 777 /tmp/weblabmomtor/temp.db
chown www-data.www-data /tmp/weblabmotor/temp.db
