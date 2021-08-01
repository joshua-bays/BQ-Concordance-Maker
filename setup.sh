#!/bin/bash

git clone https://github.com/bontibon/kjv
cd kjv
make
cp kjv ../bible
cd ..
bash add-verses.sh
sed -e 's/\ \ /|/g' verses.txt > verses-edited.txt
echo "y" | mv verses-edited.txt verses.txt
make

exit
