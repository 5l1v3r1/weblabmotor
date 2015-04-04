#!/usr/bin/python

import sqlite3
conn = sqlite3.connect('/opt/db/sensores.db')
c = conn.cursor()


c.execute('SELECT * FROM sensores ORDER BY date')
print c.fetchall()
