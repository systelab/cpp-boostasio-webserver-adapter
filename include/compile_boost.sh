#!/bin/bash
current_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )
cd $current_path
bin_path=$( cd ../bin ; pwd -P)
rm -rf $bin_path/boost_build

cd boost
./bootstrap.sh --prefix=$bin_path/boost_build --with-libraries=thread 
./b2 install