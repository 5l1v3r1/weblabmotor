#!/usr/bin/python

#python criarDB.py ./sensores.db

import sqlite3, sys

bancoDados=sys.argv[1];

conn = sqlite3.connect(bancoDados)

c = conn.cursor()

# Create table
c.execute('''CREATE TABLE sensores(date text, a0 numeric, a1 numeric, a2 numeric, a3 numeric,a4 numeric, a5 numeric, d2 numeric, d3 numeric)''')

# Save (commit) the changes
conn.commit()

# We can also close the connection if we are done with it.
# Just be sure any changes have been committed or they will be lost.
conn.close()