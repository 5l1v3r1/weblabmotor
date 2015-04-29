#!/usr/bin/python

import sqlite3, sys

conn = sqlite3.connect(sys.argv[1])
c = conn.cursor()

c.execute("PRAGMA table_info(sensores)")

print c.fetchone()