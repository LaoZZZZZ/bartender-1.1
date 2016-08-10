# bartender-1.1

# What's bartender?

Bartender is a c++ tool that is designed to process random barcode data. Bartender is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY.

It currently has three functionalities. 

1. It extracts barcodes from FASTA or FASTQ files.
2. It clusters barcode reads and counts the frequency of each cluster.
3. It generates count trajectories for time-course data.

# Installation
 Bartender uses a gcc compiler which should be no older than gcc47. If you want use the extraction components, the boost library should be installed prior installing bartender. Installation is simple.
 1.To build bartender_single (barcode clustering): make bartender_single
 2.To build bartender_extractor (barcode extractor): make bartender_extractor
 3.To build bartender_combiner (multiple time point mode): make bartender_combiner
 3.To build three components in one batch: make all
 4.To install: sudo make install

The default install directory is /usr/local/bin and is hard coded in the make file. If you want to change the install directory, you need to make a small change to the Makefile.

# Components
##Bartender Extractor
###Input
This component takes a sequencing reads file and outputs extracted barcodes. Currently it only supports single-end reads. The command is bartender_extractor_com. The following are the input parameters.

-f: the input reads file (required). Only supports FASTQ and FASTA.

-o: the output prefix (required). The output filename will be output prefix + "_barcode.txt"

-q: the ascii value for the quality threshold (optional). Only those barcodes with an average quality greater than this threshold will be kept. This is the value of the ascii character that corresponds to a quality score, not the quality score itself.  For example, the ascii character on the Illumina 1.8+ platform that corresponds to a quality of 30 is “?”. So to filter out reads with an average quality score below 30 , the threshold should be set to “?”. Different sequencing platforms might have different ascii character maps, so please check before determining the value. The default value is 0 (all valid barcodes are accepted).

-m: the total number of mismatches allowed in the proceeding and succeeding sequences (optional). The default value is 2 which allows 1 mismatch only in both the proceeding and succeeding sequences. For values greater than 2, the number of allowable mismatches will be split evenly between the proceeding and succeeding sequences (e.g. a value of 4 allows 2 mismatches in each). For odd values, an extra mismatch will be allowed in the proceeding sequence.

-p: the barcode pattern (required). The general pattern is XXXX[min-max]XXXXX[min-max]XXXXX, where XXXX is fixed DNA sequence (ie. proceeding sequence, spacers and succeeding sequence), and [min-max] is the range of random bases allowed. Both min and max must be integers. The pattern should obey the following rules:
 1. It should only have DNA sequences, numerical values, brackets and '-'.
 2. The DNA sequence before the first bracket is the proceeding sequence (important for the -m parameter).
 3. The DNA sequence after the last bracket is the succeeding sequence (important for the -m parameter).
 4. The range specified by the numeric values within the brackets specifies the possible number of random positions. For example, [2-3] means between 2 and 3 random bases, and [3] means 3 random bases. 
 5. The pattern must start with fixed sequence and end with fixed sequence. In other words, the proceeding sequence and succeeding sequence cannot be empty.
 6. The maximum length of both the proceeding and succeeding sequences is 5.
 
Here are some valid examples. 

TACCT[10]ATAA: Proceeding sequence is TACCT followed by a random 10 bases, and the succeeding sequence ATAA.

TTAC[3]AA[4-5]CCT: The proceeding sequence is TTAC; the first random region has 3 base pairs. It is followed by a constant spacer, AA. The second random region has between 4 or 5 bases The succeeding sequence is CCT.

TACC[4-7]AA[4-7]AA[4-7]TT[4-7]ATAA. This pattern contains four random regions. Each one is between 4 and 7 bases. The proceeding sequence is TACC, and the succeeding sequence is ATAA.

Here are some invalid examples.

[3-4]ATCC: No proceeding sequence.

ATTTCAT[3-4]ATC: the length of proceeding sequence exceeds 5.

ATC[3-]TAC: the numerical range is not valid.

###Output
One output file will be generated. It is in csv format and has two columns. The first column is the extracted barcode (only random parts are kept). The second column is the original line number in the FASTA or FASTQ file. We include this line number for use in more complex situations (for example, pairing UMIs). 

Please check the test_extractor.sh under the example folder for more details.

##Bartender Clustering

# Input:

Currently it only accepts input format that are consistent with the output of extraction components. The input must be a csv file with two columns. The second column in the input file does not need to be the line number (the default extractor output). It could be anything that is associated with the corresponding extracted barcode (i.e an UMI). 

# Usage

The command name is bartender_single_com, which is a python file. Use "bartender_single_com -h" for help. There are 7 options.

-f: the input file, a two column csv file 

-o: the output prefix

-c: the frequency cutoff. All clusters with size less then this threshold will not be reported in the results.

-d: The maximum cluster distance that may be merged. The default value is 2. If the distance between two cluster sequences is within this threshold, these two sequences will be merged if they pass the statistical test set by the cluster merging threshold (z).

-z: The cluster merging threshold. Higher z values result in more cluster merging (see the publication for a full description).  This should be set according to the expected coverage per barcode and the barcode library complexity.  For low to medium coverage (<500 reads/barcode), we recommend starting with the default setting for z (= 5). However, in some cases, it may be necessary to adjust the z higher to prevent over-merging (see publication). In cases where all barcodes are expected to be distant (an average of 5-6 mismatches from a nearest neighbor), we recommend disabling the merging threshold (z = -1) to make merging decisions based on cluster distance only.
 
-l: The seed length. The possible values range from 3 to 8, with a default of 5 (recommended). The larger this value, the faster the program will run. In some cases with high sequence error rates, a higher setting will result in under-merging. 

-t: The number of threads. The default value is 1.

-s: The number of non-overlapping positions between two adjacent seeds. The default value is 1 (recommended). For example, using l=5 and s=2, will result in adjacent seeds that have 2 unique positions and 3 positions in common. If the step value is equal to or larger than the seed length, then there will be no overlap between seeds.

# Output:
Bartender outputs three files. 

## a. Cluster file.
   
This file contains general information of each cluster. It has at least four columns.
  1. Cluster.ID: the unique ID of the cluster.
  2. Center: the center of this cluster. The center is the consensus sequence of all barcodes that belong to this cluster. It is calculated by taking the most frequent base at each position. Typically, the center is the most frequent sequence in a cluster and represents the true barcode.
  3. Cluster.Score: this score measures the quality of the cluster. The lower the score is, the higher the cluster quality is. Low quality clusters have a greater likelihood of being more than one barcode that were erroneously clustered together. The score is calculated based on the entropy of a binary variable using the Position Weight Matrix (PWM). Each cluster will have a PWM, which has the frequency of each nucleotide at each position. Based on the PWM, the percentage of majority nucleotide at each position is denoted as P and the entropy at that position is -P*log2(P) - (1 - P)*log2(1-P) . The Cluster Score is the maximum entropy value across all positions.
  4. The remaining columns are the cluster count at each time point. The overall count is the sum of these remaining columns. When only a single time point is processed, the fourth column is the size of the cluster.

## b.Quality file.

This file is the position weight matrix of each cluster.  The first column is cluster ID. The second column is the nucleotide that the line represents. The remaining columns are the frequency of that nucleotide at each position in the barcode cluster. Each cluster takes four lines, with each line being a nucleotide (e.g. A, C, G, T).  Check the 2M_clustering_quality.csv in the example folder for more details.

## c.Barcode file.

This file is used to keep track of the assignment of each unclustered barcode to the clusters. It has three columns.
  1. Unique.reads: the unclustered barcode sequence.
  2. Frequency: the count of this sequence before clustering (unique reads).
  3. Cluster.ID: the cluster id to which this unique read belongs.

##Multiple time point mode

# Input:

 Currently it only accepts input format that is consistent with the output of the bartender_single_com component. 

# Usage

The command name is bartender_combiner_com, which is a python file. Use "bartender_combiner_com -h" for help. There are 3 options.

-f: the cluster results files from different time points. The cluster and quality files should be included in the list. The files should be separated by commas and ordered by time. For example, if there are two time points included in the time-course data, then the input file should be: “-f time_point_1_cluster.csv,time_point_1_quality.csv,time_point_2_cluster.csv,timepoint_2_quality.csv”. 

-o: the output prefix

-c: the cluster size cutoff. All clusters below this threshold will be removed from the results. 

It outputs three files that in the same format as the bartender_single_com outputs.

# Problems and questions

##What's the meaning of cluster in bartender?

A cluster represents a putative "true" barcode. The size of the cluster represents the count of the barcode. All unclustered barcode reads that are merged into a cluster must have the same length since hamming distance is used to measure sequence dissimilarity. 

##Will a barcode sequence with an insertion or deletion be clustered to its original "true" barcode?

No. Barcodes with insertions and deletions will form a cluster by themselves. Bartender only merges sequences with identical lengths.

##What's the center in a cluster?

The center is the consensus nucleotide sequence of all barcodes within a cluster.

##I had hard time compiling bartender because boost was not installed correctly.

First, download and install boost by following the instructions on boost official webpage http://www.boost.org/. Remember to specify the prefix (--prefix=place you want to install the BOOST) when installing BOOST via the b2 command supplied by the BOOST package. 

On OSX, use one of the following commands: 
brew install boost; (brew installs packages to /usr/local/Cellar by default)
sudo port install boost (port installs packages to /opt/local by default)

Second, export the BOOST_INSTALL_DIR=prefix directory where BOOST is installed. For example, if BOOST is installed at /usr/local/ and BOOST libraries and headers are in the folder /usr/local/lib and /usr/local/include. Then BOOST_INSTALL_DIR should be /usr/local. One way to add BOOST_INSTALL_DIR is using export command. For example, at the command line: “export BOOST_INSTALL_DIR=/usr/local”. This command will set BOOST_INSTALL_DIR to /usr/local.

## Does a barcode trajectory output from Bartender accurately describe its relative frequency over time? 
No. It only tells you the count at each time point. Because total sequencing reads may vary between time points, Bartender output must be normalized by the sequencing depth to obtain barcode frequency trajectories. 

