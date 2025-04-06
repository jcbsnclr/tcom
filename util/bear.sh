#!/usr/bin/env sh

TMP="$(mktemp)"
bear --output "$TMP" -- make clean all 
mv "$TMP" build/compile_commands.json
