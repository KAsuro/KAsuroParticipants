#!/bin/bash

echo "Building Library"
make clean
make all
make distclean

echo "Building Selftest"
pushd examples/SelfTest 1>/dev/null
make clean
make all
make distclean
popd 1>/dev/null

echo "Building Bootloader"
pushd tinyasuroboot 1>/dev/null
make clean
make all
make distclean
popd 1>/dev/null

echo "Building Documentation"
rm -rf doc/de/html
rm -rf doc/de/latex
rm doc/de/dokumentation.pdf
rm -rf doc/en/html
rm -rf doc/en/latex
rm doc/en/documentation.pdf
doxygen doc/doc_de
doxygen doc/doc_en
pushd doc/de/latex 1>/dev/null
make all
mv refman.pdf ../dokumentation.pdf
popd 1>/dev/null
pushd doc/en/latex 1>/dev/null
make all
mv refman.pdf ../documentation.pdf
popd 1>/dev/null
rm -rf doc/de/latex
rm -rf doc/en/latex

read
