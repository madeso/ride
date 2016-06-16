#!/usr/bin/python
import urllib
import os
import zipfile
import sys
import argparse
import re

# todo: change wx solution to use static crt...
# sln file:
# Project("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") = "richtext", "wx_richtext.vcxproj", "{7FB0902D-8579-5DCE-B883-DAF66A885005}"
# in vcxproj file: change:
# <RuntimeLibrary>MultiThreadedDebugDLL</RuntimeLibrary> to <RuntimeLibrary>MultiThreadedDebug</RuntimeLibrary>
# <RuntimeLibrary>MultiThreadedDLL</RuntimeLibrary> to <RuntimeLibrary>MultiThreaded</RuntimeLibrary>

# def hklm(path, var):
#     reg = registry.ConnectRegistry(None,registry.HKEY_LOCAL_MACHINE)
#     key = registry.OpenKey(reg, path)
#     value, type = registry.QueryValueEx(key, var)
#     registry.CloseKey(key)
#     registry.CloseKey(reg)
#     if not type == registry.REG_SZ:
#         raise "registry not a string!"
#     return value
#
# vs_root = hklm(r'SOFTWARE\Microsoft\VisualStudio\14.0', 'InstallDir')
# print "studio path ", vs_root#


if os.name == 'nt':
  print "This is the vs solution path..."
  sys.stdout.flush()
  os.system(r"reg QUERY HKLM\SOFTWARE\Microsoft\VisualStudio\14.0 /v InstallDir /reg:32")

vs_root = r'C:\Program Files (x86)\Microsoft Visual Studio 14.0\Common7\IDE'
sys.stdout.flush()

root = os.getcwd()
install_dist = os.path.join(root, 'install-dist')
wx_root = os.path.join(install_dist, 'wx')
proto_root = os.path.join(install_dist, 'proto')
proto_root_root = os.path.join(proto_root, 'protobuf-2.6.1')


def verify_dir_exist(path):
    if not os.path.isdir(path):
        os.makedirs(path)


def download_file(url, path):
    if not os.path.isfile(path):
        urllib.urlretrieve(url, path)
    else:
        print "Aldready downloaded", path


def list_projects_in_solution(path):
    ret = []
    dir = os.path.dirname(path)
    pl = re.compile(r'Project\("[^"]+"\) = "[^"]+", "([^"]+)"')
    with open(path) as sln:
        for line in sln:
            # Project("{8BC9CEB8-8B4A-11D0-8D11-00A0C91BC942}") = "richtext", "wx_richtext.vcxproj", "{7FB0902D-8579-5DCE-B883-DAF66A885005}"
            m = pl.match(line)
            if m:
                ret.append(os.path.join(dir, m.group(1)))
    return ret


def list_projects_cmd(cmd):
    projects = list_projects_in_solution(cmd.sln)
    for p in projects:
        print "project", p


def add_definition_to_project(path, define):
    # <PreprocessorDefinitions>WIN32;_LIB;_CRT_SECURE_NO_DEPRECATE=1;_CRT_NON_CONFORMING_SWPRINTFS=1;_SCL_SECURE_NO_WARNINGS=1;__WXMSW__;NDEBUG;_UNICODE;WXBUILDING;%(PreprocessorDefinitions)</PreprocessorDefinitions>
    pp = re.compile(r'([ ]*<PreprocessorDefinitions>)([^<]*</PreprocessorDefinitions>)')
    lines = []
    with open(path) as project:
        for line in project:
            m = pp.match(line)
            if m:
                lines.append('{0}{1};{2}'.format(m.group(1), define, m.group(2)))
            else:
                lines.append(line.rstrip())
    with open(path, mode='w') as project:
        for line in lines:
            project.write(line + "\n")


def add_definition_cmd(args):
    add_definition_to_project(args.project, args.define)


def add_definition_to_solution(sln, definition):
    projects = list_projects_in_solution(sln)
    for p in projects:
        add_definition_to_project(p, definition)


def install_cmd(args):
    global vs_root
    global root
    global install_dist
    global wx_root
    global proto_root_root
    build = args.build
    platform='x64'
    if os.environ.get('PLATFORM', 'unknown') == 'x86':
        platform='Win32'

    wx_url = "https://github.com/wxWidgets/wxWidgets/releases/download/v3.1.0/wxWidgets-3.1.0.zip"
    wx_zip = os.path.join(install_dist, "wx.zip")
    wx_sln = os.path.join(wx_root, 'build', 'msw', 'wx_vc14.sln')
    wx_msbuild_cmd = 'msbuild /p:Configuration=Release /p:Platform="{platform}" {solution}'.format(platform=platform, solution=wx_sln)

    proto_url = "https://github.com/google/protobuf/releases/download/v2.6.1/protobuf-2.6.1.zip"
    proto_zip = os.path.join(install_dist, 'proto.zip')
    proto_sln = os.path.join(proto_root_root, 'vsprojects', 'protobuf.sln')
    proto_msbuild_cmd = 'msbuild /t:libprotobuf;protoc /p:Configuration=Release /p:Platform="{platform}" {solution}'.format(platform=platform, solution=proto_sln)

    print root
    print wx_sln
    print proto_sln

    verify_dir_exist(install_dist)
    verify_dir_exist(wx_root)
    verify_dir_exist(proto_root)

    print "downloading wx..."
    download_file(wx_url, os.path.join(install_dist, wx_zip))

    print "downloding proto..."
    download_file(proto_url, os.path.join(install_dist, proto_zip))

    print "extracting wx"
    with zipfile.ZipFile(wx_zip, 'r') as z:
        z.extractall(wx_root)

    print "extracting proto"
    with zipfile.ZipFile(proto_zip, 'r') as z:
        z.extractall(proto_root)

    print "building wxwidgets"
    print "-----------------------------------"
    if build:
      sys.stdout.flush()
      os.system(wx_msbuild_cmd)

    print "upgrading protobuf"
    print "-----------------------------------"
    devenv = os.path.join(vs_root, 'devenv.exe')
    if build:
      sys.stdout.flush()
      os.system('"{devenv}" {sln} /upgrade'.format(sln=proto_sln, devenv=devenv))
    # add definitoon so vs will compile
    add_definition_to_solution(proto_sln, '_SILENCE_STDEXT_HASH_DEPRECATION_WARNINGS')
    
    print "building protobuf"
    print "-----------------------------------"
    if build:
      sys.stdout.flush()
      os.system(proto_msbuild_cmd)


def cmake_cmd(args):
    global root
    global install_dist
    global wx_root
    global proto_root_root
    build = os.path.join(root, 'build')
    proto_src_root = os.path.join(proto_root_root, 'src')
    os.makedirs(build)
    cmakecmd = ("cd {build} && cmake "
                "-DCMAKE_INSTALL_PREFIX= {install_dist} "
                "-DPROTOBUF_SRC_ROOT_FOLDER={proto_root} "
                "-DwxWidgets_ROOT_DIR={wx_root} "
                "-DRIDE_BUILD_COMMIT=%APPVEYOR_REPO_COMMIT% "
                "-DRIDE_BUILD_NUMBER=%APPVEYOR_BUILD_NUMBER% "
                "-DRIDE_BUILD_BRANCH=%APPVEYOR_REPO_BRANCH% "
                "-DRIDE_BUILD_REPO=%APPVEYOR_REPO_NAME% "
                "{root}").format(
        root=root,
        install_dist=install_dist,
        proto_root=proto_root_root,
        wx_root=wx_root,
        build=build
    )
    sys.stdout.flush()
    os.system(cmakecmd)

parser = argparse.ArgumentParser(description='Does the windows build')
subparsers = parser.add_subparsers()

install_parser = subparsers.add_parser('install')
install_parser.set_defaults(func=install_cmd)
install_parser.add_argument('--nobuild', dest='build', action='store_const', const=False, default=True)

install_parser = subparsers.add_parser('listprojects')
install_parser.set_defaults(func=list_projects_cmd)
install_parser.add_argument('sln', help='solution file')

install_parser = subparsers.add_parser('add_define')
install_parser.set_defaults(func=add_definition_cmd)
install_parser.add_argument('project', help='project file')
install_parser.add_argument('define', help='preprocessor to add')

cmake_parser = subparsers.add_parser('cmake')
cmake_parser.set_defaults(func=cmake_cmd)

args = parser.parse_args()
args.func(args)
