#!/bin/bash
killall RPiFirmwareExe
cd ../build
cmake ../
make -j4
./RPiFirmwareExe