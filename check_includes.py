#!/usr/bin/python
import argparse
import sys
import glob
import os
import re


class StopFile(Exception):
    pass


class Include:
    def __init__(self, c, l):
        self.line_class = c
        self.line = l


def int_compare(lhs, rhs):
    return lhs - rhs


def str_cmp(lhs, rhs):
    if lhs == rhs:
        return 0
    if lhs < rhs:
        return -1
    return 1

def include_compare(lhs, rhs):
    if lhs.line_class == rhs.line_class:
        return str_cmp(lhs.line, rhs.line)
    else:
        return int_compare(lhs.line_class,  rhs.line_class)


class Main:
    def __init__(self):
        self.filename = ""
        self.line_num = 0
        self.error_count = 0

    def error(self, message):
        print '{}({}): error CHK3030: {}'.format(self.filename, self.line_num, message)
        self.error_count += 1

    def classify(self, line):
        """
        :rtype : str
        """
        name = os.path.splitext(os.path.basename(self.filename))[0]
        patterns = [
            r'#include "ride/' + name + r'+\.h"',
            r'#include <google/[a-zA-Z_0-9]+(/[a-zA-Z_0-9]+)*.h>',
            r'#include <ride/wx.h>',
            r'#include <wx/[a-zA-Z]+\.h>',
            r'#include <wx/aui/[a-zA-Z]+\.h>',
            r'#include <wx/html/[a-zA-Z]+\.h>',
            r'#include <wx/stc/[a-zA-Z]+\.h>',
            r'#include <[a-zA-Z0-9]+>',
            r'#include "[a-zA-Z]+.pb.h"',
            r'#include "cpptoml.h"',
            r'#include "ride_compiler_settings.h"',
            r'#include "ride/generated/[a-zA-Z]+\.h"',
            r'#include "ride/resources/[a-zA-Z]+\.h"',
            r'#include "ride/[a-zA-Z]+\.h"'
        ]

        index = 0
        for p in patterns:
            if re.match(p, line):
                return index
            index += 1

        self.error('{} is a invalid header'.format(line))
        raise StopFile()


    def main(self):
        parser = argparse.ArgumentParser(description="Check all the includes")
        parser.add_argument('files', metavar='FILE', nargs='+', help='A file')
        parser.add_argument('--verbose', action='store_const', const=True, default=False, help='verbose printing')
        parser.add_argument('--status', action='store_const', const=True, default=False, help='verbose printing')
        args = parser.parse_args()

        self.error_count = 0
        verbose = args.verbose

        file_count = 0
        file_error = 0

        for dir in args.files:
            count = 0
            for filename_glob in glob.glob(dir):
                self.line_num = -1
                self.filename = os.path.abspath(filename_glob)
                count += 1
                file_count += 1
                stored_error = self.error_count

                if os.path.isdir(self.filename):
                    self.error(self.filename + ' is not a file, ignoring')
                else:
                    with open(self.filename) as f:
                        try:
                            if verbose:
                                print "Opening file", self.filename
                            self.line_num = 0
                            includes = []
                            last_class = -1
                            print_sort = False
                            for line in f:
                                self.line_num += 1
                                if line.startswith('#include'):
                                    l = line.rstrip()
                                    line_class = self.classify(l)
                                    includes.append(Include(line_class, l))
                                    if last_class > line_class:
                                        self.error('Include order error for ' + l)
                                        print_sort = True
                                    last_class = line_class
                                    if verbose:
                                        print line_class, l
                            if print_sort:
                                includes.sort(cmp=include_compare)
                                print 'I think the correct order would be:'
                                current_class = includes[0].line_class
                                for i in includes:
                                    if current_class != i.line_class:
                                        print ''
                                    current_class = i.line_class
                                    print i.line
                                print '---------------\n\n'
                        except StopFile:
                            self.error_count += 1
                        self.line_num = -1
                if self.error_count != stored_error:
                    file_error += 1
            if count == 0:
                self.error_count += 1
                sys.stderr.write(dir + " didn't yield any files\n")

        if args.status:
            print 'Files parsed',  file_count
            print 'Files errored',  file_error
            print 'Errors found', self.error_count
        sys.exit(self.error_count > 0)
main = Main()
main.main()