## Building on Os X

Apart from the various dependencies, one has to make sure that wxWidgets is built with the same osx deployment target or there will be compile errors, link errors or segfaults.

The tl;dr is that the --with-macosx-version-min and the cmake_osx_deployment_target has to match.

#Example

With the following command to build wxWidgets:
    ../configure --with-osx_cocoa --with-macosx-version-min=10.10 --with-macosx-sdk=/Applications/Xcode.app/Contents/Developer/Platforms/MacOSX.platform/Developer/SDKs/MacOSX10.11.sdk --enable-debug --with-opengl  --prefix="$(pwd)"

One has to set the cmake variable cmake_osx_deployment_target to 10.10.

# Sample errors with a bad configuration
* exc_bad_access in wxWindowBase::SetName called in wxNonOwnedWindow::Create
* "non-virtual thunk to Editor::NotifyDeleted(Document*, void*)", referenced from: vtable for ScintillaWX in stcdll_ScintillaWX.o
* type_traits file not found in wx/strvararg.h