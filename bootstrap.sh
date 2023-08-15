#!/bin/bash

mkdir -p include/modules
mv include/* include/modules
mv include ..

REPO_DIR=`pwd`
cd ..
rm -r $REPO_DIR

