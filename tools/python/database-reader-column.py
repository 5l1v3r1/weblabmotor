#!/usr/bin/python
#python verColunas.py ../database.db

import sqlite3, sys

conn = sqlite3.connect(sys.argv[1])
c = conn.cursor()

c.execute("PRAGMA table_info(sensores)")

print c.fetchone()