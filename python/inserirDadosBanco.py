import sqlite3, sys

a0=sys.argv[1];
a1=sys.argv[2];
a2=sys.argv[3];
a3=sys.argv[4];
a4=sys.argv[5];
a5=sys.argv[6];
bancoDados=sys.argv[7];

conn = sqlite3.connect(bancoDados)
c = conn.cursor()                            

# Insert a row of data
c.execute("INSERT INTO sensores VALUES (datetime('now','localtime'),?,?,?,?,?,?,'1','0')",(a0,a1,a2,a3,a4,a5))

# Save (commit) the changes
conn.commit()

# We can also close the connection if we are done with it.
# Just be sure any changes have been committed or they will be lost.
conn.close()
