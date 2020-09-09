#!/bin/bash

rm -r tetgen-files/; mkdir tetgen-files/; cp stl/* tetgen-files/; cp tetgen_linux64 tetgen-files/; cd tetgen-files/; for f in *; do ./tetgen_linux64 -Y -q10.0 $f; done; rm *.edge; rm *.smesh; rm *.face; rm *.stl; rm tetgen_linux64; cd ..;
