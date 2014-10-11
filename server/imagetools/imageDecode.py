#!/usr/bin/python
import os, sys, argparse
import imageMapper as im

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Decodes a PNG image to the original file')
    parser.add_argument('image', help='the encoded PNG image')
    parser.add_argument('resultdir', help='the directory where the file will be created')

    args = parser.parse_args()

    path = im.decodeData(args.image, args.resultdir)

    print path

