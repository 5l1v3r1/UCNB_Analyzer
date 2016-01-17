#!/bin/bash

# Copyright 2016.  Los Alamos National Security, LLC.
# This file is part of UCNB_Analyzer.
# This program is distributed under the terms of the GNU General Public License, version 2.0.  See LICENSE.md included in top directory of this distribution.

# Example of data processing with the Analyzer program

for file in 15
do
    for par in 300
    do
	./Analyzer -p "Files/Dec15/" -raw -sort -trap 50 -f $file -decay 200 -top 300 -shaping $par
    done
done
