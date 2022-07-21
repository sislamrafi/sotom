#!/usr/bin/env bash
SCRIPT_DIR=$( cd -- "$( dirname -- "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )

#echo $SCRIPT_DIR
python $SCRIPT_DIR/sotom.py $*