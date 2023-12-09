#!/bin/bash

# Asegurarse de que se proporciona un directorio
if [ $# -ne 1 ]; then
    echo "Uso: $0 <directorio>"
    exit 1
fi

DIR="$1"

# Buscar todos los archivos de manera recursiva en el directorio proporcionado
find "$DIR" -type f -not -path '*/.git/*' -not -name 'WebServPDF.pdf' | while read FILE; do
    # Mostrar la ruta relativa del archivo
    REL_PATH="${FILE#$DIR/}"
    echo "                $REL_PATH"
    echo "============================================="

    # Mostrar el contenido del archivo
    CAT_OUTPUT=$(cat "$FILE")
    echo "$CAT_OUTPUT"
    echo "============================================="
    echo
done
