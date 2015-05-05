#!/usr/bin/python

import sqlite3, sys

conn = sqlite3.connect(sys.argv[1])
c = conn.cursor()

#c.execute('SELECT date AS DATA, a0 AS A0, a1 as A1 FROM sensores WHERE a0 IS NOT NULL')
c.execute('SELECT * FROM sensores ORDER BY date DESC')
print c.fetchall()
