#!/bin/bash
set -e

while [[ $# -gt 0 ]]
do
    case "$1" in
        -config)
            CONFIG="$2"
            shift 2
            ;;
        *)
            echo "Unknown argument: $1"
            exit 1
            ;;
    esac
done

CONFIG="$(tr '[:lower:]' '[:upper:]' <<< "${CONFIG:0:1}")${CONFIG:1}"

mkdir -p build
cd ./build

cmake .. -DCMAKE_BUILD_TYPE="$CONFIG"