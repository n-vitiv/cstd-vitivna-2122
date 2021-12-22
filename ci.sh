#!/bin/bash

#cleaning
chmod +x ./clean.sh
./clean.sh

#libs installing
sudo apt install build-essential
sudo apt install libncurses5-dev

#compilation
make

#moving binary files to directory
mkdir bin
mv -f *.o bin/
