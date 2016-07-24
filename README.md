# bartender-1.1

# What's bartender?

Bartender is a c++ tool that is designed to process random barcode data. Bartender is distributed in the hope that it will be useful, but WITHOUT ANY WARRANTY.

It currently has three functionalities. 

1. Extracts the barcode from the original reads
2. It can call out true barcodes and their population size.
3. It can generate the trajectory for time-course data.

# Installation
 Bartender gcc compiler which should be no older than gcc47. If you want use the extraction components, boost library should be installed prior installing bartender. Installation is simple.
 1.To build bartender_single (barcode clustering): make bartender_single
 2.To build bartender_extractor (barcode extractor): make bartender_extractor
 3.To build bartender_combiner (multiple time point mode): make bartender_combiner
 3.To build three components in one batch: make all
 4.To install: sudo make install

The default install directory is /usr/local/bin and is hard coded in the make file. If you want to change the install directory, you need to make a small change to the Makefile.

# Components
##Extraction
###Input
This components takes a reads file and outputs the extracted barcode. Currently it only supports single-end reads. The command is bartender_extractor_com. The following are the input parameters.

-f: the input reads file(required). Only supports FASTQ and FASTA.

-o: the output prefix(required). The output filename will be output prefix + "_barcode.txt"

-q: the quality threshold(optional). Only those barcode that has average quality no less than this threshold will be kept. This threshold is the corresponding ascii value. Different formats might have different ranges, please check relevant reference to determine the value. The default value is 0. That is it will accept all valid barcode.  We typically use a cutoff of 30, which is 63, using the current Illumina FASTQ output. Please EITHER CHANGE THE DEFAULT TO 30 OR GIVE THEM THE CORRESPONING ASCII VALUE. 

-m: the total number of mismatches allowed in the preceeding and suceeding sequence(optional). Default value is 2. The mismatches will be evenly distributed to two parts.

-p: the barcode pattern(required). The general pattern looks like XXXX[min-max]XXXXX[min-max]XXXXX. XXXX part are those fixed DNA sequence, ie. preceeding sequence, spacers and succeeding sequence. [min-max] specified the range of number of random base pairs between two fix parts. Both min and max are integers. The pattern should obey the following rules:
 1. It could only have DNA sequence, numerical value, brackets and '-'.
 2. The DNA sequence before the first bracket is viewed as the proceeding sequence.
 3. The DNA sequence after the last bracket is viewed as the succeeding sequence.
 5. The range specified by the numeric values within each pair of brackets specifies the possible number of random positions for that part. i.e [2-3] means this random part at least has 2 base pair and 3 base pairs at most. [3] means this part has 3 base pair. 
 6. The pattern must start with fixed sequence and end with fixed sequence. In another word, the preceeding sequence and succeeding sequence should not be empty.
 7. The maximum length of preceeding and succeeding sequence is 5.
 
Here are some valid examples. 

TACCT[10]ATAA: Preceeding sequence is TACCT followed by a random part having 10 base pairs. The succeeding sequence is ATAA.

TTAC[3]AA[4-5]CCT: The preceeding sequence is TTAC. First random part has 3 base pairs followed by a spacers,which is AA. The second random parts could possibly has 4 or 5 base pairs. The succeeding sequence is CCT.

TACC[4-7]AA[4-7]AA[4-7]TT[4-7]ATAA. This pattern contains four random parts. Each one at most has 7 base pairs and 4 base pairs at least. The proceeding sequence is TACC. The suceeding sequence is ATAA.

Here are some invalid examples.

[3-4]ATCC: No preceeding sequence.

ATTTCAT[3-4]ATC: the length of preceeding sequence exceeds 5.

ATC[3-]TAC: the numerical range is not valid.

###Output
One output file will be generated. It is in csv format and has two columns. The first column is the extracted barcode(Only random parts are kept). The second column is the original line number in the reads file. We include this line number for use in more complex situations (for example double barcodes or UMIs). 

Please check the test_extractor.sh under the example folder for more example.

##Clustering

# Input:

 Currently it accepts input format that is consistent with the output of extraction components. The second column in the input file does not need to be the line number. It could be anything that is associated with the corresponding extracted barcode, i.e UMI. The first column could be the combined barcode for the double barcodes design.

# Usage

The command name is bartender_single_com, which is a python file. Use "bartender_single_com -h" for help. There are 7 options and is very easy to use.

-f: the input fastq file 

-o: the output prefix

-c: the frequency cutoff. All clusters with size less then this threshold will not included in the result.

-e: the sequencing error. The default value is 0.01. Could be obtained by running the extraction components.

-z: The index of merging willingness. The default value is 4.0. The higher this value is, the more chance that two similar barcodes get merged

-l: The seed length. The default value is 5. The possible values range from 3 to 8. The larger this value, the faster the program will be with sacrificing the accuracy and memory efficiency!

-t: The number of threads. The default value is 1.

-s: The distance between two adjacent seeds. The default value is 1. if step value equals to or larger than seed length, then there will be no overlap!

# Output:
Bartender outputs three files. 

## a. Cluster file.
   
This file contains general information of each cluster. It has at least four columns.
  1. Cluster.ID: the unique ID of the cluster.
  2. Center: the center of this cluster. The center is the representative sequence of all original barcodes that belong to this cluster. Typically the center will be viewed as the true barcode in the downstream analysis.
  3. Cluster.Score: the score measures the quality of the cluster. The lower the score is, the higher the cluster quality is. The score is calculated based on the entropy of binary variable using the Position Weight Matrix(PWM). Here is a brief explanation. Each cluster will have a PWM, which has the frequency of each nucleotide at each position. Based on the PWM, pick up the majority nucleotide and calculate its percentage(P). then the entropy value of each position is -P*log2(P) - (1 - P)*log2(1-P) . The score is the maximum value of entropy values among all positions.
  4. The remaining columns are the cluster size at each time point. The overall size is the sum of these remainng columns. For single time point case, the fourth column is the size of the cluster.

## b.Quality file.

This file keeps the position weight matrix of each cluster.  Since barcodes might have different length due to many reasons, such as insertion, deletion,  the total number of columns of this file equals the maximum length of barcodes plus two. The first column is cluster ID. The second column gives the nucleotide that the line represents. The remaining columnes represents the frequency of corresponding necleotide.

From cluster aspect, each cluster will take four lines. Each line represents each nucleotide. i.e. first line represents 'A'.  Check the 2M_clustering_quality.csv in the example folder for more detail.

## c.Barcode file.

This file is used to keep track the assignment of each raw barcode with respect to the cluster. It has three columns.
  1. Unique.reads: the extracted barcode from the raw read.
  2. Frequency: the raw count of this unique read.
  3. Cluster.ID: the cluster id which this barcode belongs to.

##Multiple time point mode

# Input:

 Currently it accepts input format that is consistent with the output of bartender_single_com component. 

# Usage

The command name is bartender_combiner_com, which is a python file. Use "bartender_combiner_com -h" for help. There are 3 options and is very easy to use.

-f: the cluster results files from different time points. Only the cluster and quality files should be included. The files should be separated by comma and ordered by the time.

-o: the output prefix

-c: the cutoff that all clusters below this threshold will be removed from the result. 

It ouputs three files that are same with the output of bartender_single_com components.

# Problems and questions

##What's the meaning of cluster in bartender?

A cluster represents a "true" barcode. The size of the cluster represents the frequency of the "true" barcode. All barcodes within one cluster have the same length. And the distance between any barcode and the center is within a certain hamming distance. This distance is dynamically determined in the clustering process. Typically the distance ranges from 2 to 4.

##Will barcode with insertion and deletion be clustered to its orignal "true" barcode?

No. BC with insertino and deletion will form a cluster by themselves. Of course, you can assign those barcode back to the "true barcode" base certain critiria on top of the clustering result.

##What's the center in the cluster?

A center is the consensus barcode of all barcodes within that cluster.

##I had hard time to compile bartender due to the boost installation.

First, download and install boost by following the instructions in boost official webpage http://www.boost.org/. Remember to specify the installation directory by setting the prefix. 

On OSX, use one of the following commands: 
brew install boost;
sudo port install boost 

Second, export BOOST_INSTALL_DIR=prefix directory that is the prefix specified when installing the boost.

## Is the estimated trajectory ready for usage?
No. It only matches the clusters between adjacent time points. The trajectory should, at least, be normalized within each time point for downstream analysis.
