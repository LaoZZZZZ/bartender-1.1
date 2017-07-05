#!/bin/bash

bartender_extractor_com -f 2M_test.fq -o 2M_extracted -q ? -p TACC[4-7]AA[4-7]AA[4-7]TT[4-7]ATAA -m 2 -u 0,10
