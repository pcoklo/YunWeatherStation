#!/usr/bin/python
import _mysql
import sys

file_path = "/tmp/datalog.txt"

web = "sql5.freesqldatabase.com"
user = "sql580426"
pas = "bQ5*pT3*"
dbname = "sql580426"
port = 3306

log = open (file_path, "r")
data = log.readline(); log.close()
pieces = data.split('\t')
print pieces

try:
	con = _mysql.connect(web, user, pas, dbname, port)
	con.query("INSERT INTO sql580426 (vrijeme, svjetlost, temperatura, tlak, vlaga, kisa) VALUES('%s', '%s', '%s', '%s', '%s', '%s')" %(pieces[0], pieces[1], pieces[2], pieces[3], pieces[4], pieces[5]))
	con.close()

except _mysql.Error, e:
	print "Error %d: %s" % (e.args[0], e.args[1])
	sys.exit(1)
