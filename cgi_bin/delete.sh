#!/bin/bash

# Verificar si se proporciona un argumento
if [ $# -eq 0 ]; then
  echo -e "Status: 400 Bad request"
  echo -e "\r"
  echo -e "File content not valid"
  exit 1
fi

# Elimina el archivo pasado como variable de entorno.
rm -rf uploaded_files/$filename;

echo -e "Status: 204 Deleted"
echo -e "Filename: $filename"
echo -e "Content-Type: text/plain"
echo -e "\r"
echo -e "File deleted successfully"
