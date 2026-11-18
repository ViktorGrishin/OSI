#!/bin/bash

if [[ $# == 1 ]]; then
  TEST_DIR=$1
else
  TEST_DIR="test"
fi

if [ -e "$TEST_DIR" ]; then
    echo "Каталог уже существует"
    exit 1
fi

mkdir "$TEST_DIR"
cd "$TEST_DIR" || exit

for i in {1..3}; do
  DIRi="dir_$i"
  mkdir "$DIRi"
  cd "$DIRi" || exit

  for j in {1..5}; do
    DIRj="subdir_$j"
    mkdir "$DIRj"
    cd "$DIRj" || exit

    for k in 100 196 292 388 484; do
      filename="file-$k"
      echo "Hello World! $i $j $k" >> "$filename"
    done

    cd ..
  done

  cd ..
done
