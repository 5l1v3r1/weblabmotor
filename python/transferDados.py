#!/usr/bin/python
#python transferDados.py '../databaseOrigem.db' '../databaseAlvo.db'

import sqlite3, sys

bancoDadosOrigem=sys.argv[1];
bancoDadosAlvo=sys.argv[2];

conn = sqlite3.connect(bancoDadosAlvo)
c = conn.cursor()                            

c.execute("ATTACH (?) AS temporaria",[bancoDadosOrigem])
c.execute("INSERT INTO sensores SELECT * FROM temporaria.sensores")
c.execute("DETACH DATABASE temporaria")

# Save (commit) the changes
conn.commit()

# We can also close the connection if we are done with it.
# Just be sure any changes have been committed or they will be lost.
conn.close()
