#!/usr/bin/python

import cgi, cgitb, sqlite3, sys

dados = cgi.FieldStorage() 

bd = dados.getvalue('bd')

conn = sqlite3.connect(bd)

c = conn.cursor()

c.execute("DROP TABLE sensores")

conn.commit()
conn.close()

print "Status: 200 OK"
print "Content-Type: text/html"
print ""
print "%s" %(dados)
