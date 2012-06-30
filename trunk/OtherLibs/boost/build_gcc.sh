# !/bin/sh

./bootstrap.sh
./bjam --toolset=gcc --stagedir=./lib_gcc_x86 --builddir=./ architecture=x86 address-model=32 --with-date_time --with-filesystem --with-program_options --with-signals --with-system --with-thread link=shared runtime-link=shared threading=multi stage debug release
./bjam --toolset=gcc --stagedir=./lib_gcc_x64 --builddir=./ architecture=x86 instruction-set=nocona address-model=64 --with-date_time --with-filesystem --with-program_options --with-signals --with-system --with-thread link=shared runtime-link=shared threading=multi stage debug release

