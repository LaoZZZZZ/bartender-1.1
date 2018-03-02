# bartender-1.1

# What's bartender?

Bartender is a c++ tool that is designed to process random barcode data. Bartender is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY.

It currently has three functionalities. 

1. It extracts barcodes from FASTA or FASTQ files.
2. It clusters barcode reads and counts the frequency of each cluster.
3. It generates count trajectories for time-course data.

# Installation
 Bartender uses a gcc compiler which should be no older than gcc47. If you want use the extraction components, the boost library should be installed prior installing bartender. Installation is simple.
 * To build bartender_single (barcode clustering): make bartender_single
 * To build bartender_extractor (barcode extractor): make bartender_extractor
 * To build bartender_combiner (multiple time point mode): make bartender_combiner
 * To build three components in one batch: make all
 * To install: sudo make install

The default install directory is /usr/local/bin and is hard coded in the make file. If you want to change the install directory, you need to make a small change to the Makefile.

# Getting started
This section will introduce Bartender with a simple example. This exmaple illustrates how to converte a raw fastq reads file to a frequency table containing discovered barcode and raw reads count. The results also provide the quality of the discovered barcode in the position matrix format. Please download this project to your local machine in order to go through this example. The data is in example/random_small_data folder.
The raw input is 2M_test.fq, a file containing 1000 reads (250 unique reads).

## Barcode extraction 
We first extract barcodes from an example fastq file (2M_tst.fq) using the bartender extraction component (bartender_extractor_com). The barcode pattern in the 2M_tst.fq file is TACC[4-7]AA[4-7]AA[4-7]TT[4-7]ATAA. It has four random regions separated by 3 spacers. Both proceeding and succeeding sequences have 4 characters. In this example, we will allow one mismatch each at proceeding and suceeding sequences. You can filter out low quality reads by specifying a lowest average quality score as an ascii value. Here we will filter out any read with average quality score less than 30 (corresponding ascii code is '?'). We name the output file as 2M_extracted_barcode.txt by giving the prefix - 2M_extracted. The command to perform this is:

**bartender_extractor_com -f 2M_test.fq -o 2M_extracted -q ? -p TACC[4-7]AA[4-7]AA[4-7]TT[4-7]ATAA -m 2**. 

This command outputs the extracted barcode and the line number in input file to 2M_extracted_barcode.txt in the following format:

"barcode sequence","line number"

"barcode sequence","line number"

"barcode sequence","line number"

...

It also reports the total number of reads, total number of reads that have valid barcode, and total number of reads that have valid reads that pass the average quality score to the command line. There is a bash script containing the actual command (/example/random_small_data/test_extractor.sh). Feel free to try that script. The Bartender clustering component will also accept a unique molecular identifier (UMI) instead of a line number in these files and this is discussed below.


## Clustering the barcode

Once the barcode is extracted, we can group similar extracted barcodes into putative barcodes by using the bartender clustering component (bartender_single_com). Bartender provides a way to assign a unique molecular identifier (UMI) to each barcode read to handle PCR jackpotting errors. In the extracted barcode file, the line number is associated with each extracted barcode and this will act as a default UMI for clustering without UMI handling (each barcode read has a unique line number and therefore a unique UMI). The user can replace this line number with the corresponding UMI in the original read file. Bartender has also provides an option to extract UMI and barcode in the same step. There is an example script (./example/random_small_data/test_extractor_umi.sh) illustrating how to add umi paramter to the command line. The user can also refer the corresponding section (Bartender Extractor) for more details. For reads with multiple UMIs (e.g. one on the forward read and one on the reverse read), we recommend concatenating the sequences to generate one longer UMI. There are several things that user can configure for the grouping process (described in Bartender Clustering section). The most important parameter (-d) is the maximum allowable sequence distance that allows two barcode reads to be clustered. A second important parameter is the seed length (-s). The possible values of s range from 3 to 8, with a default of 5 (recommended). The larger this value, the faster the program will run. In some cases with high sequence error rates, a higher setting will result in under-merging. You can also choose the number of threads to leverage the computing power. Here we will use this command to clustering the extacted barcode. 

**bartender_single_com -f 2M_extracted_barcode.txt -o 2M_barcode -d 3**        

There is one file named 2M_extracted_barcode_umi.txt in the same folder. This file was generated on top of 2M_extracted_barcode.txt by giving each unique barcode a UMI. We use the same config parameters to process this file:

**bartender_single_com -f 2M_extracted_barcode_umi.txt -o 2M_barcode -d 3**        

You can find these two commands in two bash files(test_clustering.sh and test_clustering_umi.sh) in the example/random_small_data/ folder. Both of these commands specify the following behavior:
 
Extracted barcodes within 3 base pairs of each other have a chance of being clustered together. 2M_extracted_barcode.txt is the input file. We name the output file starting with 2M_barcode prefix. All other parameters are in default values. Bartender will generate three output files (described in the Bartender clustering section).

# Components
## Bartender Extractor
### Input
This component takes a sequencing reads file and outputs extracted barcodes. Currently it only supports single-end reads. The command is bartender_extractor_com. The following are the input parameters.

-f: the input reads file (required). Only supports FASTQ and FASTA.

-o: the output prefix (required). The output filename will be output prefix + "_barcode.txt"

-q: the ascii value for the quality threshold (optional). Only those barcodes with an average quality greater than this threshold will be kept. This is the value of the ascii character that corresponds to a quality score, not the quality score itself.  For example, the ascii character on the Illumina 1.8+ platform that corresponds to a quality of 30 is “?”. So to filter out reads with an average quality score below 30 , the threshold should be set to “?”. Different sequencing platforms might have different ascii character maps, so please check before determining the value. The default value is 0 (all valid barcodes are accepted).

-m: the total number of mismatches allowed in the proceeding and succeeding sequences (optional). The default value is 2 which allows 1 mismatch only in both the proceeding and succeeding sequences. For values greater than 2, the number of allowable mismatches will be split evenly between the proceeding and succeeding sequences (e.g. a value of 4 allows 2 mismatches in each). For odd values, an extra mismatch will be allowed in the proceeding sequence.

-u: the Unique Molecular Identifier (UMI) position and length. The parameter format is position,length. The position is 0-based. For example, "-u 1,20" means the UMIs start at the second position and the UMI length is 20. That means UMI covers bases 2 through 21 in each read. Since Bartender only accepts single-end reads, it only accepts one UMI per read and the UMI region should not overlap with the barcode region. Bartender also requires that the UMIs are located at a specific location of the reads. 

-d: the strand direction of raw reads. There are three possible options - forward, reverse complement and both. Each option can be provided with short or long value(case insensitive). For example, forward direction can be specified with '-d f' or '-d forward'. Same with the other two options: '-d rc' or '-d reverse_complement'; '-d b' or '-d both'. User can also specify this with --direction parameter keyword, like '--direction=f', '--direction=forward'. If this parameter is not specified, bartender extractor will only consider the forward direction of raw reads when matching the barcode pattern.
 
-p: the barcode pattern (required). The general pattern is XXXX[min-max]XXXXX[min-max]XXXXX, where XXXX is fixed DNA sequence (ie. proceeding sequence, spacers and succeeding sequence), and [min-max] is the range of random bases allowed. Both min and max must be integers. The pattern should obey the following rules:
 *  It should only have DNA sequences, numerical values, brackets and '-'.
 *  The DNA sequence before the first bracket is the proceeding sequence (important for the -m parameter).
 *  The DNA sequence after the last bracket is the succeeding sequence (important for the -m parameter).
 *  The range specified by the numeric values within the brackets specifies the possible number of random positions. For example, [2-3] means between 2 and 3 random bases, and [3] means 3 random bases. 
 *  The pattern must start with fixed sequence and end with fixed sequence. In other words, the proceeding sequence and succeeding sequence cannot be empty.
 *  The maximum length of both the proceeding and succeeding sequences is 5.
 
Here are some valid examples. 

TACCT[10]ATAA: Proceeding sequence is TACCT followed by a random 10 bases, and the succeeding sequence ATAA.

TTAC[3]AA[4-5]CCT: The proceeding sequence is TTAC; the first random region has 3 base pairs. It is followed by a constant spacer, AA. The second random region has between 4 or 5 bases The succeeding sequence is CCT.

TACC[4-7]AA[4-7]AA[4-7]TT[4-7]ATAA. This pattern contains four random regions. Each one is between 4 and 7 bases. The proceeding sequence is TACC, and the succeeding sequence is ATAA.

Here are some invalid examples.

[3-4]ATCC: No proceeding sequence.

ATTTCAT[3-4]ATC: the length of proceeding sequence exceeds 5.

ATC[3-]TAC: the numerical range is not valid.

### Output
One output file will be generated. It is in csv format and has two columns. The first column is the extracted barcode (only the random parts are kept). The second column is the original line number in the FASTA or FASTQ file if UMI handling is not implemented, or the UMI is UMI handling is implemented. We include this line number for use in more complex situations (for example, pairing paired end reads). 

Please check the test_extractor.sh under the example folder for more details.

## Bartender Clustering

### Input:

Currently it only accepts input format that are consistent with the output of extraction components. The input must be a csv file with two columns. The second column in the input file does not need to be the line number (the default extractor output). It could be anything that is associated with the corresponding extracted barcode (i.e an UMI). See "Clusering the Barcode" in the example above for more details. 

### Usage

The command name is bartender_single_com, which is a python file. Use "bartender_single_com -h" for help. There are 7 options.

-f: the input file, a two column csv file 

-o: the output prefix

-c: the frequency cutoff. All clusters with size less then this threshold will not be reported in the results.

-d: The maximum cluster distance that may be merged. The default value is 2. If the distance between two cluster sequences is within this threshold, these two sequences will be merged if they pass the statistical test set by the cluster merging threshold (z).

-z: The cluster merging threshold. Higher z values result in more cluster merging (see the publication for a full description).  This should be set according to the expected coverage per barcode and the barcode library complexity.  For low to medium coverage (<500 reads/barcode), we recommend starting with the default setting for z (= 5). However, in some cases, it may be necessary to adjust the z higher to prevent over-merging (see publication). In cases where all barcodes are expected to be distant (an average of 5-6 mismatches from a nearest neighbor), we recommend disabling the merging threshold (z = -1) to make merging decisions based on cluster distance only.
 
-l: The seed length. The possible values range from 3 to 8, with a default of 5 (recommended). The larger this value, the faster the program will run. In some cases with high sequence error rates, a higher setting will result in under-merging. 

-t: The number of threads. The default value is 1.

-s: The number of non-overlapping positions between two adjacent seeds. The default value is 1 (recommended). For example, using l=5 and s=2, will result in adjacent seeds that have 2 unique positions and 3 positions in common. If the step value is equal to or larger than the seed length, then there will be no overlap between seeds.

### Output:
Bartender outputs three files. 

#### a. Cluster file.
   
This file contains general information of each cluster. It has at least four columns.
  1. Cluster.ID: the unique ID of the cluster.
  2. Center: the center of this cluster. The center is the consensus sequence of all barcodes that belong to this cluster. It is calculated by taking the most frequent base at each position. Typically, the center is the most frequent sequence in a cluster and represents the true barcode.
  3. Cluster.Score: this score measures the quality of the cluster. The lower the score is, the higher the cluster quality is. Low quality clusters have a greater likelihood of being more than one barcode that were erroneously clustered together. The score is calculated based on the entropy of a binary variable using the Position Weight Matrix (PWM). Each cluster will have a PWM, which has the frequency of each nucleotide at each position. Based on the PWM, the percentage of majority nucleotide at each position is denoted as P and the entropy at that position is -P*log2(P) - (1 - P)*log2(1-P) . The Cluster Score is the maximum entropy value across all positions.
  4. The remaining columns are the cluster count at each time point. The overall count is the sum of these remaining columns. When only a single time point is processed, the fourth column is the size of the cluster.

#### b. Quality file.

This file is the position weight matrix of each cluster.  The first column is cluster ID. The second column is the nucleotide that the line represents. The remaining columns are the frequency of that nucleotide at each position in the barcode cluster. Each cluster takes four lines, with each line being a nucleotide (e.g. A, C, G, T).  Check the 2M_clustering_quality.csv in the example folder for more details.

#### c. Barcode file.

This file is used to keep track of the assignment of each unclustered barcode to the clusters. It has three columns.
  1. Unique.reads: the unclustered barcode sequence.
  2. Frequency: the count of this sequence before clustering (unique reads).
  3. Cluster.ID: the cluster id to which this unique read belongs.

## Multiple time point mode

### Input:

 Currently it only accepts input format that is consistent with the output of the bartender_single_com component. 

### Usage

The command name is bartender_combiner_com, which is a python file. Use "bartender_combiner_com -h" for help. There are 3 options.

-f: the cluster results files from different time points. The cluster and quality files should be included in the list. The files should be separated by commas(NO SPACE) and ordered by time. For example, if there are two time points included in the time-course data, then the input file should be: “-f time_point_1_cluster.csv,time_point_1_quality.csv,time_point_2_cluster.csv,timepoint_2_quality.csv”. 

-o: the output prefix

-c: the cluster size cutoff. All clusters below this threshold will be removed from the results. 

It outputs three files that in the same format as the bartender_single_com outputs.

# Problems and questions

1. What's the meaning of cluster in bartender?

A cluster represents a **putative** "true" barcode. The size of the cluster represents the count of the barcode. All unclustered barcode reads that are merged into a cluster must have the same length since hamming distance is used to measure sequence dissimilarity. 

2. Will a barcode sequence with an **insertion** or **deletion** be clustered to its original "true" barcode?

**No**. Barcodes with insertions and deletions will form a cluster by themselves. Bartender only merges sequences with identical lengths.

3. What's the center in a cluster?

The center is the consensus nucleotide sequence of all barcodes within a cluster.

4. I had hard time compiling bartender because boost was not installed correctly.

First, download and install **boost** by following the instructions on boost official webpage http://www.boost.org/. Remember to specify the prefix (--prefix=place you want to install the BOOST) when installing BOOST via the b2 command supplied by the BOOST package. 

On OSX, use one of the following commands: 
brew install boost; (brew installs packages to /usr/local/Cellar by default)
sudo port install boost (port installs packages to /opt/local by default)

Second, export the BOOST_INSTALL_DIR=prefix directory where BOOST is installed. For example, if BOOST is installed at /usr/local/ and BOOST libraries and headers are in the folder /usr/local/lib and /usr/local/include. Then BOOST_INSTALL_DIR should be /usr/local. One way to add BOOST_INSTALL_DIR is using export command. For example, at the command line: “export BOOST_INSTALL_DIR=/usr/local”. This command will set BOOST_INSTALL_DIR to /usr/local.

5. Does a barcode trajectory output from Bartender accurately describe its **relative** frequency over time? 

**No**. It only tells you the count at each time point. Because total sequencing reads may vary between time points, Bartender output must be normalized by the sequencing depth to obtain barcode frequency trajectories. 


# Bugs and Fixes
You can post any question or suggestions on google group https://groups.google.com/forum/#!forum/bartenderrandombarcode.
* Bartender just tabled unique reads and did not cluster the reads.
It was caused by an incorrect merge between local branch and remote branch happened in Aug 13th 2016. It was reported by one user and was fixed at Dec 1th. I am really sorry for these fatal error introduced by this incorrect conflict resolve. Now Batender works fine.
* Bartender combiner has problem with the cluster id, which will trigger an fatal alert.
It was caused by a wrong array initialization. Combiner should work fine now.
* Bartender extractor could not accept other mismatch number except 2. This bug is in the bartender_extractor_com pythong script. There was some logic issue in the regular expression pattern generation function.
It was fixed at Jun 24 2017.
* Bartender segments high diversity seeds into range by the seed length and steps. It will extends the last range if it has less number seeds positions than seed length by borrowing some left side positions. But there is a bug in borrowing left side seeds. It was fixed at Oct 1st 2017.
* Bartender always dedups the uni for initial unique reads even for the clustering result before pcr process, which is a confusing behavior. The behavior is changed to more intuitive pattern which does not dedup anything before pcr process. It was changed at Feb 24 2018.
