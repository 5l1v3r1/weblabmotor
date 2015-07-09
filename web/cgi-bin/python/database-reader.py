#!/usr/bin/python

# Import modules for CGI handling 
import cgi, cgitb, sqlite3, sys, json 

# Create instance of FieldStorage 
bd = cgi.FieldStorage() 

# Get data from fields
bd = bd.getvalue('bd')

def dict_factory(cursor, row):
    d = {}
    for idx, col in enumerate(cursor.description):
        d[col[0]] = row[idx]
    return d
 
connection = sqlite3.connect(bd)
connection.row_factory = dict_factory
 
cursor = connection.cursor()

cursor.execute("SELECT * FROM sensores ORDER BY date ASC")
 
dados = cursor.fetchall()

A0 = []
A1 = []
DATA = []

for x in xrange((len(dados))):
	A0.append(dados[x]['a0'])
	A1.append(dados[x]['a1'])
	DATA.append(dados[x]['date'])

dic_A0 = {'A0':(A0)}
dic_A1 = {'A1':(A1)}
dic_DATA = {'DATA':(DATA)}

dados = [dic_DATA,dic_A0,dic_A1]
dadosJson = json.dumps(dados)

connection.close() 

print "Status: 200 OK"
print "Content-Type: application/json"
print ""
print "%s" % (dadosJson)