#!/bin/bash

# Directory to store padded PDFs
TEMP_DIR=$(mktemp -d)

echo showpage | ps2pdf -sPAPERSIZE=a4 - "$TEMP_DIR/blank.pdf"

# Pad each PDF
for pdf in "$@"; do
  PAGES=$(qpdf --show-npages "$pdf")
  echo "$pdf $PAGES"
  BASENAME=$(basename "$pdf")
  OUTPUT_PDF="$TEMP_DIR/$BASENAME"

  if (( PAGES % 2 != 0 )); then
    echo "pad"
    qpdf --empty --pages "$pdf" "$TEMP_DIR/blank.pdf" -- "$OUTPUT_PDF"
  else
    cp "$pdf" "$OUTPUT_PDF"
  fi
done

# Merge the padded PDFs
qpdf --empty --pages "$TEMP_DIR"/*.pdf -- merged.pdf

# Clean up
rm -r "$TEMP_DIR"

echo "Merged PDF saved as merged.pdf"
