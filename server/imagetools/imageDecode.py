#!/usr/bin/python
import os, sys, argparse
import imageMapper as im
import gzip

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Decodes a PNG image to the original file')
    parser.add_argument('image', help='the encoded PNG image')
    parser.add_argument('resultdir', help='the directory where the file will be created')

    args = parser.parse_args()

    with open(args.filename, "rb") as file_in:
        with gzip.open(args.filename+"_c", "wb") as file_out:
            file_out.writelines(file_in)



    path = im.decodeData(args.image+"_c", args.resultdir)

    print path

