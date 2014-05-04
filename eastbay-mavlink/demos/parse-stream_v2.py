#!/usr/bin/python
#-----------------------------------------------------------------------
# python mavlink testing
#
# this program demonstrates how to connect to a mavlink device,
# receive, decode, and verify mavlink messages.
#-----------------------------------------------------------------------

import sys
import array
import struct
import serial    # install from https://pypi.python.org/pypi/pyserial

#-----------------------------------------------------------------------
# set your port and baud rates here
#-----------------------------------------------------------------------

PORT='COM4'                       # typical windows port
#PORT='/dev/tty.usbmodem621'        # typical mac port
BAUD=57600                         # baud rate for radio connection
#BAUD=115200                        # baud rate for direct usb connection

#-----------------------------------------------------------------------
# manifest constants
#-----------------------------------------------------------------------

MAV_STARTB=0xfe      # this byte indicates start of mavlink packet

#-----------------------------------------------------------------------
# utility stuff
#-----------------------------------------------------------------------

xdumpf=''.join([(len(repr(chr(x)))==3) and chr(x) or '.' for x in range(256)])
def xdump(src, length=16):
    """dump a string in classic hexdump format
       adapted from http://code.activestate.com/recipes/142812-hex-dumper
    """
    N=0;
    while src:
        s,src = src[:length],src[length:]
        hexa = ' '.join(["%02X"%ord(x) for x in s])
        s = s.translate(xdumpf)
        print "%04X   %-*s   %s" % (N, length*3, hexa, s)
        N+=length

#-----------------------------------------------------------------------
# checksum special note:
#
# each message type (0,1,2,...) has an extra one-byte magic number.
# heartbeat (message 0) has magic number of 50, so you can index this
# table with the message number to get the magic number.  After
# accumulating the bytes of the message, accumulate the magic number.
# this table is copied from the mavlink source.
#-----------------------------------------------------------------------

MAVLINK_MESSAGE_CRCS=[
  50,124,137,0,237,217,104,119,0,0,0,89,0,0,0,0,0,0,0,0,214,159,220,168,
  24,23,170,144,67,115,39,246,185,104,237,244,222,212,9,254,230,28,28,
  132,221,232,11,153,41,39,214,223,141,33,15,3,100,24,239,238,30,240,183,
  130,130,0,148,21,0,243,124,0,0,0,20,0,152,143,0,0,127,106,0,0,0,0,0,0,
  0,231,183,63,54,0,0,0,0,0,0,0,175,102,158,208,56,93,0,0,0,0,235,93,124,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,42,
  241,15,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,
  0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,204,49,
  170,44,83,46,0]

#-----------------------------------------------------------------------
class x25crc(object):
    """x25 CRC - based on checksum.h from mavlink library"""

    def __init__(self, buf=''):
        self.crc = 0xffff
        self.accumulate(buf)

    def accumulate(self, buf):
        '''add in some more bytes'''
        bytes = array.array('B')
        if isinstance(buf, array.array):
            bytes.extend(buf)
        else:
            bytes.fromstring(buf)
        accum = self.crc
        for b in bytes:
            tmp = b ^ (accum & 0xff)
            tmp = (tmp ^ (tmp<<4)) & 0xFF
            accum = (accum>>8) ^ (tmp<<8) ^ (tmp<<3) ^ (tmp>>4)
            accum = accum & 0xFFFF
        self.crc = accum

#-----------------------------------------------------------------------
"""def decode2(buf):
    global lastseq
    print '-----------'
    print 'found 253 msgId'
    xdump(buf)
    magic, mlen, seq, srcSystem, srcComponent, msgId = struct.unpack('<6B', buf[:6])
    b0,b1=struct.unpack('2B',buf[6+mlen:])
    givenCk=b0+b1*256
    crc=x25crc()
    crc.accumulate(buf[1:len(buf)-2]) # skip magic and cksum
    crc.accumulate(chr(MAVLINK_MESSAGE_CRCS[msgId]))

    print 'mlen=%d, seq=%d, sys=(%d,%d), msgId=%d, sums=(%04x,%04x)'%\
        (mlen,seq,srcSystem,srcComponent,msgId,givenCk,crc.crc)"""

import math

def distance_on_unit_sphere(lat1, long1, lat2, long2):
    # Convert latitude and longitude to 
    # spherical coordinates in radians.
    degrees_to_radians = math.pi/180.0
    # phi = 90 - latitude
    phi1 = (90.0 - lat1)*degrees_to_radians
    phi2 = (90.0 - lat2)*degrees_to_radians
    # theta = longitude
    theta1 = long1*degrees_to_radians
    theta2 = long2*degrees_to_radians  
    # Compute spherical distance from spherical coordinates.
    # For two locations in spherical coordinates 
    # (1, theta, phi) and (1, theta, phi)
    # cosine( arc length ) = 
    #    sin phi sin phi' cos(theta-theta') + cos phi cos phi'
    # distance = rho * arc length
    cos = (math.sin(phi1)*math.sin(phi2)*math.cos(theta1 - theta2) + math.cos(phi1)*math.cos(phi2))
    arc = math.acos( cos )
    # Remember to multiply arc by the radius of the earth 
    # in your favorite set of units to get length.
    return arc


 
def printToFile(buf):
    global lastseq
    print '-----------'
    print 'found 253 msgId'
    xdump(buf)
    magic, mlen, seq, srcSystem, srcComponent, msgId = struct.unpack('<6B', buf[:6])
    b0,b1=struct.unpack('2B',buf[6+mlen:])
    givenCk=b0+b1*256
    crc=x25crc()
    crc.accumulate(buf[1:len(buf)-2]) # skip magic and cksum
    crc.accumulate(chr(MAVLINK_MESSAGE_CRCS[msgId]))

    print 'mlen=%d, seq=%d, sys=(%d,%d), msgId=%d, sums=(%04x,%04x)'%\
        (mlen,seq,srcSystem,srcComponent,msgId,givenCk,crc.crc)
    f = open('target_found', 'w')

    startLat = 33.969565
    startLon = -118.4145917
    latStr = ""
    for x in buf[10:14]:
        latStr = "%02X"%ord(x) + latStr
    lat = 10**-7 * int(latStr, 16)

    lonStr = ""
    for x in buf[14:18]:
        lonStr = "%02X"%ord(x) + lonStr
    lon = -10**-7 * (int('100000000', 16) - int(lonStr, 16))

    radiusOfEarthInches = 251106299
    inchesToTravel= radiusOfEarthInches * distance_on_unit_sphere(startLat, startLon, lat, lon)


    startCoords = "Start: X " + str(startLat) + " Y " + str(startLon) + "\n"
    #coords = "X " + "%0.7f" % lat + " Y " + "%0.7f" % lon + "\n"
    coords = str(int((lat - 33) * 10**7)) + " " + str(int((lon + 118) * -10**7))
    #coords = "X " + str(lat) + " Y " + str(lon) + "\n"
    #hexa = ' '.join(["%02X"%ord(x) for x in buf[10:18]])
    #f.write(startCoords)
    f.write(coords)
    #f.write(str(inchesToTravel))
    f.close()
    #f.write(buf[10:18]) (binary signed)
#-----------------------------------------------------------------------
lastseq=255
"""def decode(buf):
    "decode a mavlink message"

    global lastseq
    print '-----------'
    xdump(buf)
    magic, mlen, seq, srcSystem, srcComponent, msgId = struct.unpack('<6B', buf[:6])
    b0,b1=struct.unpack('2B',buf[6+mlen:])
    givenCk=b0+b1*256
    crc=x25crc()
    crc.accumulate(buf[1:len(buf)-2]) # skip magic and cksum
    crc.accumulate(chr(MAVLINK_MESSAGE_CRCS[msgId]))

    print 'mlen=%d, seq=%d, sys=(%d,%d), msgId=%d, sums=(%04x,%04x)'%\
        (mlen,seq,srcSystem,srcComponent,msgId,givenCk,crc.crc)

    
    if crc.crc == givenCk:
        # good message, process it
        if seq != (lastseq+1)%256:
            print 'WARNING, lost message? seq=%d,lastseq=%d'%(seq,lastseq)
        lastseq=seq
        if msgId == 253:
            decode2(buf)
	
    else:
        # complain!
        print 'BAD CRC on message'"""

#-----------------------------------------------------------------------
def has253msgId(buf):
    """decode a mavlink message"""

    global lastseq
    print '-----------'
    xdump(buf)
    magic, mlen, seq, srcSystem, srcComponent, msgId = struct.unpack('<6B', buf[:6])
    b0,b1=struct.unpack('2B',buf[6+mlen:])
    givenCk=b0+b1*256
    crc=x25crc()
    crc.accumulate(buf[1:len(buf)-2]) # skip magic and cksum
    crc.accumulate(chr(MAVLINK_MESSAGE_CRCS[msgId]))

    print 'mlen=%d, seq=%d, sys=(%d,%d), msgId=%d, sums=(%04x,%04x)'%\
        (mlen,seq,srcSystem,srcComponent,msgId,givenCk,crc.crc)

    
    if crc.crc == givenCk:
        # good message, process it
        if seq != (lastseq+1)%256:
            print 'WARNING, lost message? seq=%d,lastseq=%d'%(seq,lastseq)
        lastseq=seq
        return msgId == 253
	
    else:
        # complain!
        print 'BAD CRC on message'



def timedread(ser,n):
    """read octets, complain upon timeout"""

    while True:
        x = ser.read(n)
        if len(x) == 0:
            print "TIMEOUT"
        else:
            return x

#-----------------------------------------------------------------------
def process(ser):
    """process the mavlink input stream"""
    Found = False
    while True:
        # scan stream until we see sync byte
        x = timedread(ser,1)
        if ord(x) == MAV_STARTB:
            #  read the length and rest of message, and process
            len=timedread(ser,1)
            rest=timedread(ser,4+ord(len)+2)
            buf=x
            buf+=len
            buf+=rest
            #decode(buf)
            if Found:
                printToFile(buf)
                Found = False
            if has253msgId(buf):
                Found = True
                

#-----------------------------------------------------------------------
def connect():
    """connect to the mavlink device"""

    autoselect=1

    # here's some autoselect logic for mac
    if autoselect and sys.platform == 'darwin':
        import glob
        candidates=glob.glob('/dev/tty*usb*')
        print 'candidate ports (%d):'%(len(candidates))
        for c in candidates:
            print '   ',c
        myport=candidates[0]
        mybaud=57600
        mybaud=115200
    else:
        myport=PORT
        mybaud=BAUD

    print 'connecting to:', myport
    ser = serial.Serial()
    ser.port=myport
    ser.baudrate=mybaud
    ser.parity=serial.PARITY_NONE
    ser.stopbits=serial.STOPBITS_ONE
    ser.bytesize=serial.EIGHTBITS
    ser.timeout=2
    ser.open()
    return ser

#-----------------------------------------------------------------------
def main():
    """the main thing!"""

    ser=connect()
    print 'STARTING'
    process(ser)

#-----------------------------------------------------------------------
if __name__=="__main__":
    main()
