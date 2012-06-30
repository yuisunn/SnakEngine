CALL "%VS80COMNTOOLS%..\..\VC\vcvarsall.bat" x86

devenv libogg_static.sln /Build "Debug|Win32"
devenv libogg_static.sln /Build "Release|Win32"

move /Y Win32\Debug\libogg_static.lib ..\..\lib\Win32\libogg_static_d.lib
move /Y Win32\Release\libogg_static.lib ..\..\lib\Win32\libogg_static.lib

CALL "%VS80COMNTOOLS%..\..\VC\vcvarsall.bat" x86_amd64

devenv libogg_static.sln /Build "Debug|x64"
devenv libogg_static.sln /Build "Release|x64"

move /Y x64\Debug\libogg_static.lib ..\..\lib\x64\libogg_static_d.lib
move /Y x64\Release\libogg_static.lib ..\..\lib\x64\libogg_static.lib
