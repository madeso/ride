#!/usr/bin/python
import urllib
import os
import zipfile

root = os.getcwd()
build = os.path.join(root, 'build')
wxbuild = os.path.join(build, 'wx')
localwxzip = os.path.join(build, "wx.zip")
wxsln = os.path.join(wxbuild, 'build', 'msw', 'wx_vc14.sln')
print root
print wxsln

wxurl = "https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.0/wxWidgets-3.1.0.zip"

urllib.urlretrieve (wxurl, os.path.join(build, localwxzip))

with zipfile.ZipFile(localwxzip, 'r') as z:
    z.extractall(wxbuild)

os.system('msbuild ' + wxsln)
