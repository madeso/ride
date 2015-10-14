#!/usr/bin/python
import argparse
import sys
import glob
import platform
import os
from subprocess import call

parser = argparse.ArgumentParser(description="Run clang-format with globbing support and less arguments")
parser.add_argument('files', metavar ='FILE', nargs='+', help='A file, may contain wildcards to modify with clang-format')
args = parser.parse_args()

clangformat = "clang-format"
# on osx assume that the clang-format is installed via alcatraz in eclipse
# http://stackoverflow.com/questions/110362/how-can-i-find-the-current-os-in-python
if platform.system() == 'Darwin':
	clangformat = "~/Library/Application Support/Developer/Shared/Xcode/Plug-ins/ClangFormat.xcplugin/Contents/Resources/clang-format"

print "clang-format: ", clangformat
print os.path.exists(clangformat)

error_count = 0

lastcmd = []

try:
	for dir in args.files:
		count = 0
		for fname in glob.glob(dir):
			filename = os.path.abspath(fname)
			count += 1
			print "Working ", filename
			lastcmd = [clangformat, "-i", "-style=Google", filename]
			retcode = call(lastcmd)
			if retcode <> 0:
				error_count += 1
				sys.stderr.write(filename + ' failed to clang-format\n')
			# clang-format ARGS -i -style=Google ${game_src}
		if count == 0:
			error_count += 1
			sys.stderr.write(dir + ' didnt yield any files\n')
except OSError, e:
	sys.stderr.write("""Failed to call clang-format!
		Command: %s
		Error %s - %s
		In %s
		""" % (str(lastcmd), str(e.errno), e.strerror, e.filename) )

sys.exit(error_count > 0)