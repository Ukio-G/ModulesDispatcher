#!/bin/bash

mkdir -p include/modules
mv include/*.hpp include/modules
mv include ..

REPO_DIR=`pwd`
cd ..
rm -r $REPO_DIR

