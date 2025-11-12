#!/bin/bash
cd build
./deewallet.app/Contents/MacOS/deewallet 2>&1 | tee app.log
