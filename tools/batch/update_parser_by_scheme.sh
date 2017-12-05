#!/bin/bash

function pause(){
   read -p "$*"
}

echo Generate the cpp code by glTF 2.0 schema files

python ./../../tools/jsonschematoc11 ./../../external/glTF/specification/2.0/schema/ libgltf --output_header_path=./../../include/libgltf/ --output_source_path=./../../source/libgltf --namespace=libgltf

pause 'Press [Enter] key to continue...'
