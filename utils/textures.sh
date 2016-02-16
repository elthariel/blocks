#! /bin/bash

if [ -z $1 ]; then
  echo "usage: $0 /path/to/minecraft.jar"
  exit 1
fi

dest="./media/"
dir="/tmp/lol"

echo "Work dir: $dir"

echo "Unzipping jar"
unzip -d $dir -q $1

echo "Creating $dest"
mkdir -p $dest

echo "Copying textures to $dest"
cp -rf $dir/assets/minecraft/textures $dest

echo "Cleaning work dir"
# rm -rf $dir

exit 0
