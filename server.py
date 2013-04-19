import nxt.locator
import nxt.bluesock
import sys

robot = '00:16:53:01:80:04'

server_box = 1
nxt_box = 0

msge = 'Hola nxt'
s = nxt.bluesock.BlueSock(robot)
b = s.connect()
b.message_write(nxt_box, msge)
s.close()
