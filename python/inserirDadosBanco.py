import sqlite3
import sys

conn = sqlite3.connect('/opt/db/sensores.db')
c = conn.cursor()

a0=sys.argv[1]
a1=sys.argv[2]

# Insert a row of data
c.execute("INSERT INTO sensores VALUES (datetime('now','localtime'),'123','232',?,?,'12312','123123','123123','123123')",(a0,a1))

# Save (commit) the changes
conn.commit()

# We can also close the connection if we are done with it.
# Just be sure any changes have been committed or they will be lost.
conn.close()
