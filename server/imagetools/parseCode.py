#!/usr/bin/python

import cv2
import numpy as np
from matplotlib import pyplot as plt

def findPixelSize(image):
    (height, width, three) = image.shape
    del three

    # Find left border
    for c in range(width):
        if image[:,c,:].min() < 123:
            minc = c
            break

    # Find top border
    for r in range(height):
        if image[r,:,:].min() < 123:
            minr = r
            break

    # Find right border
    for c in range(width-1,0,-1):
        if image[:,c,:].min() < 123:
            maxc = c+1
            break

    # Find bottom border
    for r in range(height-1,0,-1):
        if image[r,:,:].min() < 123:
            maxr = r+1
            break

    # Crop image
    image = image[minr:maxr,minc:maxc,:]
    (height, width, three) = image.shape
    del three

    # Parse vertical dotted line
    c = 0
    while True:
        line = image[:,c,0]
        mins, maxs = [], []
        for r in range(1,height-1):
            minofmax = 255
            maxofmin = 0
            if line[r] > max(line[r-1:r+2:2]):
                maxs.append(r)
                minofmax = min(minofmax, line[r])
            if line[r] < min(line[r-1:r+2:2]):
                mins.append(r)
                maxofmin = max(maxofmin, line[r])
        maxs = maxs[1:]
        if maxofmin < 50 and minofmax > 200:
            break
        c += 1

    # Sorted list of distance between peaks
    peakdiff = np.sort(list(np.diff(mins)) + list(np.diff(maxs)))
    peaks = len(peakdiff)

    # Take mean 50%
    discard = int(np.floor(float(peaks)/4.))
    peakdiff = peakdiff[discard:-discard]

    # Mean value is pixelsize
    # Also return cropped image
    return np.mean(peakdiff)/2., image

filename = '/home/spiros/sharpened.png'
image = cv2.imread(filename)
ps, image = findPixelSize(image)

(height, width, three) = image.shape
del three

newheight = int(np.round(float(height)/ps))
newwidth = int(np.round(float(width)/ps))
newimage = np.zeros((newheight, newwidth, 3))

for r in range(newheight):
    for c in range(newwidth):
        posr = int(np.round(r * ps))
        posc = int(np.round(c * ps))

        startr = max(0, posr-1)
        startc = max(0, posc-1)
        endr = min(height, posr+2)
        endc = min(width, posc+2)

        square = image[startr:endr,startc:endc,0]
        maxv = square.max()
        minv = square.min()

        if 255-maxv < minv:
            newimage[r,c,:] = [255,255,255]
        else:
            newimage[r,c,:] = [0,0,0]
cv2.imwrite('/home/spiros/parsed.png', newimage)
