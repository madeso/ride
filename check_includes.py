#!/usr/bin/python
import argparse
import sys
import glob
import os
import re


class StopFile(Exception):
    pass


class Main:
    def __init__(self):
        self.filename = ""
        self.line_num = 0

    def error(self, message):
        print '{}({}): error CHK3030: {}'.format(self.filename, self.line_num, message)

    def classify(self, line):
        """
        :rtype : str
        """
        name = os.path.splitext(os.path.basename(self.filename))[0]
        index = 0
        if re.match(r'#include "ride/' + name + r'+\.h"', line):
            return index
        index += 1
        if re.match(r'#include <google/[a-zA-Z_0-9]+(/[a-zA-Z_0-9]+)*.h>', line):
            return index
        index += 1
        if re.match(r'#include <ride/wx.h>', line):
            return index
        index += 1
        if re.match(r'#include <wx/[a-zA-Z]+\.h>', line):
            return index
        index += 1
        if re.match(r'#include <wx/aui/[a-zA-Z]+\.h>', line):
            return index
        index += 1
        if re.match(r'#include <wx/html/[a-zA-Z]+\.h>', line):
            return index
        index += 1
        if re.match(r'#include <wx/stc/[a-zA-Z]+\.h>', line):
            return index
        index += 1
        if re.match(r'#include <[a-zA-Z0-9]+>', line):
            return index
        index += 1
        if re.match(r'#include "[a-zA-Z]+.pb.h"', line):
            return index
        index += 1
        if re.match(r'#include "cpptoml.h"', line):
            return index
        index += 1
        if re.match(r'#include "ride/generated/[a-zA-Z]+\.h"', line):
            return index
        index += 1
        if re.match(r'#include "ride/resources/[a-zA-Z]+\.h"', line):
            return index
        index += 1
        if re.match(r'#include "ride/[a-zA-Z]+\.h"', line):
            return index

        self.error('{} is a invalid header'.format(line))
        raise StopFile()


    def main(self):
        parser = argparse.ArgumentParser(description="Check all the includes")
        parser.add_argument('files', metavar='FILE', nargs='+', help='A file')
        parser.add_argument('--verbose', action='store_const', const=True, default=False, help='verbose printing')
        args = parser.parse_args()

        error_count = 0
        verbose = args.verbose

        for dir in args.files:
            count = 0
            for filename_glob in glob.glob(dir):
                self.line_num = -1
                self.filename = os.path.abspath(filename_glob)
                count += 1

                if os.path.isdir(self.filename):
                    self.error(self.filename + ' is not a file, ignoring')
                else:
                    with open(self.filename) as f:
                        try:
                            if verbose:
                                print "Opening file", self.filename
                            self.line_num = 0
                            for line in f:
                                self.line_num += 1
                                if line.startswith('#include'):
                                    l = line.rstrip()
                                    line_class = self.classify(l)
                                    if verbose:
                                        print line_class, l
                        except StopFile:
                            error_count += 1
                        self.line_num = -1
            if count == 0:
                error_count += 1
                sys.stderr.write(dir + " didn't yield any files\n")

        sys.exit(error_count > 0)

main = Main()
main.main()