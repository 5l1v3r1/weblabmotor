#!/usr/bin/python

import sqlite3, sys

conn = sqlite3.connect(sys.argv[1])
c = conn.cursor()


c.execute('SELECT * FROM sensores ORDER BY date DESC')

dado=c.fetchone();

print dado
