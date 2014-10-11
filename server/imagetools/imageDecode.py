#!/usr/bin/python
import os, sys, argparse
import imageMapper as im
import gzip
import os



if __name__ == '__main__':
    parser = argparse.ArgumentParser(description='Decodes a PNG image to the original file')
    parser.add_argument('image', help='the encoded PNG image')
    parser.add_argument('resultdir', help='the directory where the file will be created')

    args = parser.parse_args()

   

    path = im.decodeData(args.image, args.resultdir)


    #Decompress data to a temporary file
    #with gzip.open(path+".gz","rb") as file_in:
    #    with open(path, "wb") as file_out:
    #        file_out.writelines(file_in)
            
    #os.rename(path+"_tmp", path)
    
    print path

