import nxt.locator
import nxt.bluesock
import sys

robot = '00:16:53:01:80:04'

server_box = 1
nxt_box = 0

s = nxt.bluesock.BlueSock(robot)
b = s.connect()

#First argument can be send message or wait for ack
if sys.argv[1] == 'send':
    #The arguments are direccion_x, direccion_y, move_pen
    print "Got send"
    msge = sys.argv[2] + ',' + sys.argv[3] + ',' + sys.argv[4]
    print "Message to send " + msge
    b.message_write(nxt_box, msge)
    print "Message sent"
else:
    print "Got wait"
    times = int(sys.argv[2])
    print "wait "  + str(times)
    msg = "0"
    i = 0
    while msg == "0" and i < times:
        try:
            i += 1
            local_box, msg = b.message_read(server_box, server_box, True)
        except nxt.error.DirProtError, e:
            i += 1
            msg = "0"
    if i < times:
        print "Got confirmation after " + str(i)
    else:
        print "Tired of waiting for brick"

print "Closing conection "
s.close()
