/*
 * Copyright 2011, Ben Langmead <langmea@cs.jhu.edu>
 *
 * This file is part of Bowtie 2.
 *
 * Bowtie 2 is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Bowtie 2 is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Bowtie 2.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef FORMATS_H_
#define FORMATS_H_

namespace barcodeSpace {
/**
 * File-format constants and names
 */

typedef enum file_format {
    FASTQ = 1,
    FASTA,
    RAWSEQ,
}file_format;
typedef enum CLUSTERTYPE{
    DICTATOR = 1,
    CONSENSUS,
}CLUSTERTYPE;

typedef enum NStrategy{
    CHOMP = 1,
    DISCARD,
    NTOA,
    NTOG
}NStrategy;

}
#endif /*FORMATS_H_*/
