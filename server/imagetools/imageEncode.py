#!/usr/bin/python
import os, sys, argparse, tempfile
import imageMapper as im
import gzip


def readInString(filename):
    string = ''
    with open(filename, 'rb') as fs:
        byte = fs.read(1)
        while byte:
            string += byte
            byte = fs.read(1)
    return string

if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Encodes a file into a PNG image')
    parser.add_argument('file', help='the input file')
    parser.add_argument('filename', help='the original name of the file')
    parser.add_argument('tmpdir', help='the temporary directory that will be used')

    args = parser.parse_args()

	
    #with open(args.file, "rb") as file_in:
    #   with gzip.open(args.file+"_c", "wb") as file_out:
    #      file_out.writelines(file_in)


    fname = tempfile.mkstemp(suffix='.png', dir=args.tmpdir)[-1]



    #im.encodeData(args.filename, readInString(args.file+"_c"), fname)

    im.encodeData(args.filename, readInString(args.file), fname)
		
    print fname

