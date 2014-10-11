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

def decolorizeData(bdata, offset, length):
    # Black and white
    data = ''
    endoffset = offset + length*8
    while offset < endoffset:
        char = 0
        e = 1
        for o in range(offset+7, offset-1, -1):
            v = 1 if bdata[o][0] == 0 else 0
            char += v*e
            e *= 2
        data += chr(char)
        offset += 8
    return [data, offset]


def encodeData(filename, data, outputFile, bw=True):
    """Encodes data in a file into a PNG image
    filename is the original name of the file
    data is the data contained in the file passed as string
    outputFile is a file stream where the image will be written"""

    bfilename = colorizeData(filename, bw)
    bdata = colorizeData(data, bw)
    [width,height] = computeSize(bfilename, bdata)

    # Create image structure
    image = [[[255,255,255] for c in range(width)] for r in range(height)]

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

    writer = png.Writer(height=height, width=width)
    with open(outputFile, "w") as fs:
        writer.write(fs, imagePlain)

def decodeData(imageFile):
    reader = png.Reader(imageFile)
    (width, height, pixel, metadata) = reader.asRGB8()

    # Create plain image
    imagePlain = [list(r) for r in pixel]

    ## Pack pixels
    image = [[[] for c in range(width)] for r in range(height)]
    for r in range(height):
        for c in range(width):
            image[r][c] = imagePlain[r][3*c:3*(c+1)]

    ## Serialze actual data
    btotData = []

    # Region 1
    for r in range(1,4):
        for c in range(4, width-4):
            btotData.append(image[r][c])

    # Region 2
    for r in range(4,height-4):
        for c in range(1, width):
            btotData.append(image[r][c])

    # Region 3
    for r in range(height-4,height):
        for c in range(4, width-4):
            btotData.append(image[r][c])

    ## Decode datasize
    e = 1
    datasize = 0
    for i in range(31,-1,-1):
        v = 1 if btotData[i][0] == 0 else 0
        datasize += e*v
        e *= 2

    ## Decode filename size
    e = 1
    fnamesize = 0
    for i in range(47,31,-1):
        v = 1 if btotData[i][0] == 0 else 0
        fnamesize += e*v
        e *= 2

    ## Decode filename and content
    [fname, offset] = decolorizeData(btotData, 48, fnamesize)
    [content, offset] = decolorizeData(btotData, offset, datasize)

    ## Debug
#    bb = btotData[offset:]
#    for i in range(16):
#        print bb[i]
#
    with open('tmp/'+fname, 'w') as fs:
        fs.write(content)


