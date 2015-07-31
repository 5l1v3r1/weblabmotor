#!/usr/bin/python

import cgi, cgitb, sqlite3, sys

dados = cgi.FieldStorage() 

bd = dados.getvalue('bd')

conn = sqlite3.connect(bd)

c = conn.cursor()

c.execute("CREATE TABLE sensores(date text, a0 numeric, a1 numeric, a2 numeric, a3 numeric,a4 numeric, a5 numeric, d2 numeric, d3 numeric)")

conn.commit()
conn.close()

print "Status: 200 OK"
print "Content-Type: text/html"
print ""
print "%s" %(dados)
