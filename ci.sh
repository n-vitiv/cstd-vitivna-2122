#!/bin/bash

#cleaning
chmod +x ./clean.sh
./clean.sh

#libs installing
sudo apt install build-essential
sudo apt install libncurses5-dev
sudo apt install libgtest-dev
sudo apt install libmysqlclient-dev
sudo apt install zlib1g-dev
sudo apt install mysql-server

#compilation
make

#moving binary files to directory
mkdir bin
mv -f *.o bin/

cd tests
make
mv unitTest ../
