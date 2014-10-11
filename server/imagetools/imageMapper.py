#!/usr/bin/python
from math import sqrt, ceil, floor
import png

"""
This script has the following purposes:
    * generating a PNG image from data coming from a file
    * outputting the oriinal file when provided with the PNG image
"""

def byteToBW(byte):
    s = []
    obyte = byte
    for i in range(7,-1,-1):
        v = byte / (2**i)
        byte -= v*(2**i)
        v = (1-v)*255
        s.append([v,v,v])

        if v < 0 or v > 255:
            print "Oh, god!", obyte
    return s


def computeSize(filename, data):
    """ This computes the total data length and divides it into a
    rectangle with possibly square ratio.  Total data lenght is as follows:
    * 4 bytes for legth of data (without filename)
    * 2 bytes for length of filename
    * length of filename
    * length of data
    * 4 square patterns of 4x4 points each
    * up to 2*sqrt(sum of the previous values) for the dotted lines

    Data must be an iterable with booleans.
    """
    # Lengths are always stored as bits
    totalLength = len(filename) + len(data) + 4*8 + 2*8 + 64
    totalLength += 2*ceil(sqrt(totalLength))
    
    s = max(11, int(ceil(sqrt(totalLength))))
    return (s,s)

def colorizeData(data, bw=True):
    bdata = []

    if bw:
        for d in data:
            d = ord(d)
            bdata += byteToBW(d)
    else:
        print "Not implemented yet..."

    return bdata

def encodeData(filename, data, outputFile, bw=True):
    """Encodes data in a file into a PNG image
    filename is the original name of the file
    data is the data contained in the file passed as string
    outputFile is a file stream where the image will be written"""

    bfilename = colorizeData(filename, bw)
    bdata = colorizeData(data, bw)
    [width,height] = computeSize(bfilename, bdata)

    # Create image structure
    image = [[[125,125,125] for c in range(width)] for r in range(height)]

    # Create upper left pattern
    for r in range(0,4):
        for c in range(0,4):
            v = 255 if (r==1 and c==1) or (r==3) or (c==3) else 0
            image[r][c] = [v,v,v]

    # Create upper right pattern
    for r in range(0,4):
        for c in range(-4,0):
            image[r][c] = [255,255,255]

    # Create lower left pattern
    for r in range(-4,0):
        for c in range(0,4):
            image[r][c] = [255,255,255]

    # Create lower right pattern
    for r in range(-4,0):
        for c in range(-4,0):
            image[r][c] = [255,255,255]

    # Create horizontal dotted line
    for c in range(4, width-4, 2):
        image[0][c] = [0, 0, 0]

    # Create vertical dotted line
    for r in range(4, width-4, 2):
        image[r][0] = [0, 0, 0]

    ## Put all the data together
    btotdata = []

    # Data size
    datasize = len(data)
    btotdata += (byteToBW((datasize >> 24) % 256))
    btotdata += (byteToBW((datasize >> 16) % 256))
    btotdata += (byteToBW((datasize >> 8 ) % 256))
    btotdata += (byteToBW((datasize >> 0 ) % 256))

    # Filename length
    filenamelength = len(filename)
    btotdata += (byteToBW(filenamelength >> 8 % 256))
    btotdata += (byteToBW(filenamelength >> 0 % 256))

    # Filename and data
    btotdata += bfilename
    btotdata += bdata

    ## Write data into image
    n = -1
    maxn = len(btotdata)

    # Region 1
    for r in range(1,4):
        for c in range(4, width-4):
            n += 1
            if n < maxn:
                #print "%4d" % n, "Writing to", r, c, btotdata[n]
                image[r][c] = btotdata[n]

    # Region 2
    for r in range(4,height-4):
        for c in range(1, width):
            n += 1
            if n < maxn:
                #print "%4d" % n, "Writing to", r, c, btotdata[n]
                image[r][c] = btotdata[n]

    # Region 3
    for r in range(height-4,height):
        for c in range(4, width-4):
            n += 1
            if n < maxn:
                #print "%4d" % n, "Writing to", r, c, btotdata[n]
                image[r][c] = btotdata[n]

    ## Make the rows plain
    imagePlain = []
    for r in range(height):
        myrow = []
        for c in range(width):
            myrow = myrow + image[r][c]
        imagePlain.append(myrow)

    for ir,r in enumerate(imagePlain):
        for ic,c in enumerate(r):
            if c < 0 or c > 255:
                print ir, ic, c


    writer = png.Writer(height=height, width=width)
    with open(outputFile, "w") as fs:
        writer.write(fs, imagePlain)
