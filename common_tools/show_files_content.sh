#!/bin/bash

# Asegurarse de que se proporciona un directorio
if [ $# -ne 1 ]; then
    echo "Uso: $0 <directorio>"
    exit 1
fi

DIR="$1"
TOTAL_CHARS=0

# Iterar sobre cada archivo en el directorio proporcionado
for FILE in "$DIR"/*; do
    # Comprobar que es un archivo regular y no un directorio u otro tipo de archivo
    if [ -f "$FILE" ]; then
        FILE_NAME=$(basename "$FILE")

        # Mostrar el nombre del archivo
        echo "                $FILE_NAME"
        echo "============================================="

        # Mostrar el contenido del archivo
        CAT_OUTPUT=$(cat "$FILE")
        echo "$CAT_OUTPUT"
        echo "============================================="
        echo

        # Sumar el número de caracteres del contenido del archivo al total
        TOTAL_CHARS=$((TOTAL_CHARS + ${#CAT_OUTPUT}))
    fi
done
