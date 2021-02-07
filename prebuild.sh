#!/bin/bash
set -e
exec /usr/bin/python3 build.py -g
read  -n 1 -p "Press any key to continue . . ." mainmenuinput
