#!/usr/bin/python

import random, sys, string, argparse
from optparse import OptionParser


class shuf:
    def __init__(self, isrange, filename,):
        #f = open(filename, 'r')
        if isrange:
            self.lines = filename
            self.length = len(self.lines)
        else:
            self.lines = filename.readlines()
            self.length = len(self.lines)
            filename.close()
        
    def file_len(self):
        return len(self.lines)


    
    def chooseline(self):
        return random.choice(self.lines)
    
    def shufflines(self):
        return random.sample(self.lines, self.length)
    
def main():
    usage_msg = """shuf.py [OPTION]... [FILE]
   or: shuf.py -i LO-HI [OPTION]..."""
    parser = argparse.ArgumentParser( description="""Write a random 
permutation of the input lines to standard output.  With no FILE, 
or when FILE is -, read standard input.""",usage = usage_msg)
    group = parser.add_mutually_exclusive_group()
    group.add_argument("-i", "--input-range", action="store", dest="LO_HI",
                       type=str ,
                       help="treat each number LO through HI as an input line")
    parser.add_argument("-n", "--head-count", action="store", dest="COUNT",
                      help="output at most COUNT lines")
    parser.add_argument("-r", "--repeat", action ="store_true", dest="repeat",
                        help="output lines can be repeated")
    group.add_argument("FILE", nargs='?', type=argparse.FileType('r'),
                       default = sys.stdin)

    args = parser.parse_args(sys.argv[1:])
    therange = args.LO_HI
    low = ""
    high= ""
    isrange=False
    if therange is not None:
        pastdash = False
        for char in therange:
            if char.isalpha():
                parser.error("invald input range: '" + char + "'")
            if char is "-":
                if pastdash is True:
                    parser.error("invalid input range: '" + char + "'")
                pastdash=True
                continue
            if not pastdash:    
                low += char
            elif pastdash:    
                high += char
        int_low = int(low)
        int_high = int(high)
        if int_low > int_high:
            parser.error("invalid input range: '" + therange + "'")
        rangelist=[]
        for x in range(0, int_high-int_low+1):
            valuetoappend = int_low+x
            valuetoappend = str(valuetoappend)
            rangelist.append(valuetoappend+"\n")

        input_file = rangelist
        isrange=True

    repeat = bool(args.repeat)
    
    COUNT = (args.COUNT)


    if not isrange:
        input_file = args.FILE

    try:
        generator = shuf(isrange, input_file)
        if COUNT is None:
            while repeat: #options are -r
                sys.stdout.write(generator.chooseline())
            newshuffle = generator.shufflines()
            for item in newshuffle: #no options
                sys.stdout.write(item)
            sys.exit()    
        try:
            COUNT = int(args.COUNT)
        except:
            parser.error("invalid line count: {0}".
                     format(args.COUNT))
        if COUNT < 0:
            parser.error("invalid line count: '"+ str(COUNT) + "'")
            
        if repeat: #options are -n -r
            for index in range(COUNT):
                sys.stdout.write(generator.chooseline())
        else: #options are -n only
            if COUNT > generator.length:
                COUNT = generator.length
            newshuffle = generator.shufflines()
            for index in range(COUNT):
                sys.stdout.write(newshuffle[index])
            sys.exit()
            
    
    except OSError as err:
        parser.error("OS error: {0}".format(err))    
    
if __name__ == "__main__":
    main()
