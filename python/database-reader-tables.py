#!/usr/bin/python
#python verTabelas.py ../database.db

import sqlite3, sys

conn = sqlite3.connect(sys.argv[1])
c = conn.cursor()

c.execute("SELECT name FROM sqlite_master WHERE type='table';")
print c.fetchall()
