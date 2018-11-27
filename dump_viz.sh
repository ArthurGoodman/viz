#!/bin/bash

if [[ $1 ]]; then
    for f in $1/*; do
        cat $f
        echo +
    done
fi
