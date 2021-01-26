#!/bin/bash
set -e
exec /usr/bin/python build.py
read  -n 1 -p "Press any key to continue . . ." mainmenuinput
