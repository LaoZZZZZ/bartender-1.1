#!/usr/bin/python

import glob
import os
import sys

# argument list
# inputdir, outputdir, featureCut 0.01,featureNumber 200, replication 20

def drive(direct):
	
	inputdir = '../features'
	outputdir = '../selectedFeatures'
	featureCut = 0.01
	featureNum = 200
	replication = 200
	for f in glob.glob(os.path.join(direct,'Feature_Selection*')):
		cmd = 'screen nice matlab -nodesktop -nodisplay -nojvm ' + f + ' ' + inputdir + ' ' + outputdir + ' ' + str(featureCut) + ' ' + str(featureNum) + ' ' + str(replication)
		print(cmd)
		os.system( cmd ) 

if __name__ == '__main__':
	direct = '../matlabCode'
	drive(direct)	


