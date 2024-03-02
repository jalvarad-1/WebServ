#!/bin/bash

# Verificar si se proporciona un argumento
# if [ $# -eq 0 ]; then
#   echo -e "Status: 400 Bad request"
#   echo -e "\r"
#   echo -e "File content not valid"
#   exit 1
# fi


if [ ! -f "$filename" ]; then
    echo -e "Status: 404 Bad request"
    echo -e "\r"
    echo -e "File not found"
    exit 1
fi


# Elimina el archivo pasado como variable de entorno.
rm -rf $filename;

if [ $? -ne 0 ]; then
  echo -e "Status: 404 Not found"
  echo -e "\r"
  echo -e "Path not found or file not found"
  exit 1
fi

echo -e "Status: 204 Deleted"
echo -e "\r"