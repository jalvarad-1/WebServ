#!/bin/bash

# Verificar si se proporciona un argumento
if [ $# -eq 0 ]; then
  echo -e "Status: 400 Bad request"
  echo -e "\r"
  echo -e "File content not valid"
  exit 1
fi

# Obtener el primer argumento
file=$1

# Imprimir el mensaje utilizando el argumento
echo $file > uploaded_files/$filename;

if [ $? -ne 0 ]; then
  echo -e "Status: 404 Not found"
  echo -e "\r"
  echo -e "Path not found or file not found"
  exit 1
fi

echo -e "Status: 201 Created"
echo -e "Content-Type: text/plain"
echo -e "Filename: $filename"
echo -e "\r"
echo -e "File created successfully"