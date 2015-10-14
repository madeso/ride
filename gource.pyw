#!/usr/bin/python
# This script runs gource of the files
import shlex, subprocess
cmd = "gource -a 1 -s 0.1 --highlight-users -hide filenames"
subprocess.Popen(shlex.split(cmd))
