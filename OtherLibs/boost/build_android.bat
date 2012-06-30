set CXXFLAGS="-I%ANDROID_NDK%/platforms/android-9/arch-arm/usr/include -I%ANDROID_NDK%/sources/cxx-stl/gnu-libstdc++/include/4.4.3 -I%ANDROID_NDK%/sources/cxx-stl/gnu-libstdc++/libs/armeabi/4.4.3/include -I%ANDROID_NDK%/sources/crystax/include"
set TOOLSET=gcc-android_armeabi
bjam --toolset=%TOOLSET% --user-config=./user-config-android-armeabi.jam cxxflags=%CXXFLAGS% threadapi=pthread --stagedir=./lib_android_armeabi --builddir=./ target-os=linux --with-date_time --with-filesystem --with-program_options --with-signals --with-system --with-thread link=static runtime-link=static threading=multi stage

set CXXFLAGS="-I%ANDROID_NDK%/platforms/android-9/arch-arm/usr/include -I%ANDROID_NDK%/sources/cxx-stl/gnu-libstdc++/include/4.4.3 -I%ANDROID_NDK%/sources/cxx-stl/gnu-libstdc++/libs/armeabi-v7a/4.4.3/include -I%ANDROID_NDK%/sources/crystax/include"
set TOOLSET=gcc-android_armeabi_v7a
bjam --toolset=%TOOLSET% --user-config=./user-config-android-armeabi-v7a.jam cxxflags=%CXXFLAGS% threadapi=pthread --stagedir=./lib_android_armeabi-v7a --builddir=./ target-os=linux --with-date_time --with-filesystem --with-program_options --with-signals --with-system --with-thread link=static runtime-link=static threading=multi stage

set CXXFLAGS="-I%ANDROID_NDK%/platforms/android-9/arch-x86/usr/include -I%ANDROID_NDK%/sources/cxx-stl/gnu-libstdc++/include/4.4.3 -I%ANDROID_NDK%/sources/cxx-stl/gnu-libstdc++/libs/x86/4.4.3/include -I%ANDROID_NDK%/sources/crystax/include"
set TOOLSET=gcc-android_x86
bjam --toolset=%TOOLSET% --user-config=./user-config-android-x86.jam cxxflags=%CXXFLAGS% threadapi=pthread --stagedir=./lib_android_x86 --builddir=./ target-os=linux --with-date_time --with-filesystem --with-program_options --with-signals --with-system --with-thread link=static runtime-link=static threading=multi stage
