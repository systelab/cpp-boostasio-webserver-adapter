#!/bin/bash
current_path=$( cd "$(dirname "${BASH_SOURCE[0]}")" ; pwd -P )

#Checkout Boost 1.67 tag
cd $current_path
cd boost
git checkout --recurse-submodules tags/boost-1.67.0

#Clean compilation directoy
cd $current_path
bin_path=$( cd ../bin ; pwd -P)
rm -rf $bin_path/boost_build

#Compile boost
cd boost
./bootstrap.sh --prefix=$bin_path/boost_build --with-libraries=thread 
./b2 install