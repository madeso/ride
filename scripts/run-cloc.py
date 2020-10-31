#!/usr/bin/python
import argparse
import sys
import glob
import os
import re
from subprocess import call
import itertools

def main():
    parser = argparse.ArgumentParser(description="Run cloc")
    parser.add_argument('files', nargs='+', help='A file')
    parser.add_argument('--out', help='save the log here')
    args = parser.parse_args()
    cmd = [["cloc"], args.files, ["--quiet", "-out", args.out]]
    lastcmd = list(itertools.chain(*cmd))
    # print lastcmd
    retcode = call(lastcmd)
    if retcode <> 0:
        sys.stderr.write('failed to cloc\n')
    with open(args.out) as f:
        content = f.readlines()

    # now after cloc has run
    # remove that pesky line about how long it took
    # so only the stat is left
    content = content[1:]

    with open(args.out, 'w') as f:
        f.write("".join(content))
main()