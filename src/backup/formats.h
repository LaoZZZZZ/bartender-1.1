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

namespace prefixMatching {
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
// datastruture used in the algorithm
typedef enum dataStructure {
    TRIE = 1,  // suffixtrie
    TREE,      // suffixtree
    TREEMULTIPLE, //does not do pair match, just scan the leaf node and kick those id that have already included
    BLOCK         // does not do pair match, group sequence by depth(length of common prefix)
}structure;
typedef enum NStrategy{

    CHOMP = 1,
    DISCARD,
    NTOA,
    NTOG
}NStrategy;
/*static const std::string file_format_names[] = {
	"Invalid!",
	"FASTA",
	"FASTA sampling",
	"FASTQ",
	"Tabbed mated",
	"Raw",
	"Command line",
	"Chain file",
	"Random",
	"Qseq"
};*/
}
#endif /*FORMATS_H_*/
