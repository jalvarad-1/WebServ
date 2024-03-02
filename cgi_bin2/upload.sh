#!/bin/bash

# Verificar si se proporciona un argumento
# if [ $# -eq 0 ]; then
#   echo -e "Status: 400 Bad request"
#   echo -e "\r"
#   echo -e "File content not valid"
#   exit 1
# fi

# Obtener el primer argumento
if [ -f "uploaded_files/$filename" ]; then
    echo -e "Status: 409 Conflict"
    echo -e "\r"
    echo -e "File already exists"
    exit 1
fi

while IFS= read -r input_data; do
    echo -e "$input_data" >> uploaded_files/$filename;
done

# Imprimir el mensaje utilizando el argumento
# echo -e $input_data > uploaded_files/$filename;
# echo -e $input_data

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