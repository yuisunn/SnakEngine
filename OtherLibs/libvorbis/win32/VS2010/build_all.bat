CALL "%VS100COMNTOOLS%..\..\VC\vcvarsall.bat" x86

devenv vorbis_static.sln /Build "Debug|Win32"
devenv vorbis_static.sln /Build "Release|Win32"

move /Y Win32\Debug\libvorbis_static.lib ..\..\libs\Win32\libvorbis_static_d.lib
move /Y Win32\Debug\libvorbisfile_static.lib ..\..\libs\Win32\libvorbisfile_static_d.lib
move /Y Win32\Release\libvorbis_static.lib ..\..\libs\Win32\libvorbis_static.lib
move /Y Win32\Release\libvorbisfile_static.lib ..\..\libs\Win32\libvorbisfile_static.lib

CALL "%VS100COMNTOOLS%..\..\VC\vcvarsall.bat" x86_amd64

devenv vorbis_static.sln /Build "Debug|x64"
devenv vorbis_static.sln /Build "Release|x64"

move /Y x64\Debug\libvorbis_static.lib ..\..\libs\x64\libvorbis_static_d.lib
move /Y x64\Debug\libvorbisfile_static.lib ..\..\libs\x64\libvorbisfile_static_d.lib
move /Y x64\Release\libvorbis_static.lib ..\..\libs\x64\libvorbis_static.lib
move /Y x64\Release\libvorbisfile_static.lib ..\..\libs\x64\libvorbisfile_static.lib
