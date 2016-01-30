CC	= g++
#CC	= clang++ 
CFLAGS	= -g -fPIC -w -O3 -std=c++11 
LDFLAGS	=  -lboost_regex 
WRAPPER	= barcode_com
INSTALLDIR	= /usr/bin/
# command

RM	=rm
CP	=cp
CD	=cd
MV	=mv
PWD	=pwd
EXPORT	=export
ECHO	=echo
LN	=ln

SEQUENCESOURCES	= ../sequence.cpp ../kmers_dictionary.cpp ../kmers_bitwisetransform.cpp ../fastareader.cpp ../fastqreader.cpp ../sequencereader.cpp ./sequencereader_test.cpp 
SEQUENCEOBJECTS=$(SEQUENCESOURCES:.cpp=.o)

INPUTPROCESSORSOURCES	= ../rawbarcodeprocessor.cpp ../countbarcodeprocessor.cpp ./inputprocessor_test.cpp 
INPUTPROCESSOROBJECTS=$(INPUTPROCESSORSOURCES:.cpp=.o)

KMERDECODERSOURCES	= ../kmerdecoder.cpp ../kmers_dictionary.cpp ../kmers_bitwisetransform.cpp ./kmerdecoder_test.cpp 
KMERDECODEROBJECTS=$(KMERDECODERSOURCES:.cpp=.o)

SEEDSELECTORSOURCES	= ../seedselector.cpp ../kmers_dictionary.cpp ./seedselector_test.cpp 
SEEDSELECTOROBJECTS=$(SEEDSELECTORSOURCES:.cpp=.o)

BARCODEEDITORSOURCES	= ../barcodeeditor.cpp ../kmers_dictionary.cpp ../seedselector.cpp ../kmer_freq.cpp ./barcodeeditor_test.cpp ../kmerdecoder.cpp ../kmers_bitwisetransform.cpp
BARCODEEDITOROBJECTS=$(BARCODEEDITORSOURCES:.cpp=.o)

DICTIONARYSOURCES	= ../kmers_dictionary.cpp ./dictionary_test.cpp
DICTIONARYOBJECTS=$(DICTIONARYSOURCES:.cpp=.o)

UTILSOURCES	= ../kmers_dictionary.cpp ./util_test.cpp ../util.cpp ../binaryoutfilebuf.cpp ../binaryinfilebuf.cpp ../cluster.cpp ../dictatorcluster.cpp ../kmer_freq.cpp ../kmers_bitwisetransform.cpp ../dictatorcenter.cpp 
UTILOBJECTS=$(UTILSOURCES:.cpp=.o)

TRANSFORMSOURCES	= ../kmers_bitwisetransform.cpp ../kmers_dictionary.cpp ./bitwisetransformer_test.cpp
TRANSFORMOBJECTS	= $(TRANSFORMSOURCES:.cpp=.o)

PATTERNPARSERSOURCES	= ../fastqpattern.cpp ../fastapattern.cpp ../sequence.cpp ./patternparser_test.cpp
PATTERNPARSEROBJECTS	= $(PATTERNPARSERSOURCES:.cpp=.o)

BARCODEEXTRACTORSOURCES	= ./barcodeextractor_test.cpp ../barcodeextractor.cpp ../sequence.cpp ../kmers_dictionary.cpp ../util.cpp
BARCODEEXTRACTOROBJECTS	= $(BARCODEEXTRACTORSOURCES:.cpp=.o)

BARCODECOUNTERSOURCES	= ./barcodecounter_test.cpp ../barcodecounter.cpp ../sequence.cpp ../kmers_bitwisetransform.cpp ../kmers_dictionary.cpp
BARCODECOUNTEROBJECTS	= $(BARCODECOUNTERSOURCES:.cpp=.o)

CENTERSOURCES	= ./center_test.cpp ../dictatorcenter.cpp ../util.cpp ../binaryinfilebuf.cpp ../binaryoutfilebuf.cpp ../kmers_bitwisetransform.cpp ../kmers_dictionary.cpp ../dictatorcluster.cpp
CENTEROBJECTS	= $(CENTERSOURCES:.cpp=.o)


CENTERTRACKERSOURCES	= ./centerstracker_test.cpp 
CENTERTRACKEROBJECTS	= $(CENTERTRACKERSOURCES:.cpp=.o)

CSVREADERSOURCES	= ./csvreader_test.cpp 
CSVREADEROBJECTS	= $(CSVREADERSOURCES:.cpp=.o)

CLUSTERSOURCES	= ./cluster_test.cpp ../kmer_freq.cpp ../dictatorcenter.cpp ../util.cpp ../binaryinfilebuf.cpp ../binaryoutfilebuf.cpp ../kmers_bitwisetransform.cpp ../kmers_dictionary.cpp ../dictatorcluster.cpp ../cluster.cpp ../kmerdecoder.cpp
CLUSTEROBJECTS	= $(CLUSTERSOURCES:.cpp=.o)

CENTERCALIBRATORSOURCES	= ./center_calibrator_test.cpp ../kmer_freq.cpp ../dictatorcenter.cpp ../util.cpp ../kmers_bitwisetransform.cpp ../kmers_dictionary.cpp ../dictatorcluster.cpp ../cluster.cpp ../mixturebptester.cpp ../centerrecalibrator.cpp ../kmerdecoder.cpp
CENTERCALIBRATOROBJECTS	= $(CENTERCALIBRATORSOURCES:.cpp=.o)

CLUSTERSPLITTERSOURCES	= ./clustersplitter_test.cpp ../kmer_freq.cpp ../dictatorcenter.cpp ../util.cpp ../kmers_bitwisetransform.cpp ../kmers_dictionary.cpp ../dictatorcluster.cpp ../cluster.cpp ../mixturebptester.cpp ../centerrecalibrator.cpp ../clustersplitter.cpp ../idgenerator.cpp ../kmerdecoder.cpp
CLUSTERSPLITTEROBJECTS = $(CLUSTERSPLITTERSOURCES:.cpp=.o)

CENTERCLUSTERMAPPERSOURCES	= ./center_cluster_mapper_test.cpp ../kmer_freq.cpp ../dictatorcenter.cpp ../util.cpp ../binaryinfilebuf.cpp ../binaryoutfilebuf.cpp ../kmers_bitwisetransform.cpp ../kmers_dictionary.cpp ../dictatorcluster.cpp ../cluster.cpp ../mixturebptester.cpp ../centerrecalibrator.cpp ../centerclustermapper.cpp
CENTERCLUSTERMAPPEROBJECTS	= $(CENTERCLUSTERMAPPERSOURCES:.cpp=.o)

CENTERCLUSTERMAPPERITERATORSOURCES	= ./center_cluster_mapper_iterator_test.cpp ../kmer_freq.cpp ../dictatorcenter.cpp ../util.cpp ../binaryinfilebuf.cpp ../binaryoutfilebuf.cpp ../kmers_bitwisetransform.cpp ../kmers_dictionary.cpp ../dictatorcluster.cpp ../cluster.cpp ../mixturebptester.cpp ../centerrecalibrator.cpp ../centerclustermapper.cpp ../centerclustermapperiterator.cpp
CENTERCLUSTERMAPPERITERATOROBJECTS	= $(CENTERCLUSTERMAPPERITERATORSOURCES:.cpp=.o)

LINKGENERATORSOURCES	= ./link_generator_test.cpp ../kmer_freq.cpp ../dictatorcenter.cpp ../util.cpp ../binaryinfilebuf.cpp ../binaryoutfilebuf.cpp ../kmers_bitwisetransform.cpp ../kmers_dictionary.cpp ../dictatorcluster.cpp ../cluster.cpp ../mixturebptester.cpp ../centerrecalibrator.cpp ../centerclustermapper.cpp ../errorrateestimator.cpp ../clustercenterlinkgenerator.cpp
LINKGENERATOROBJECTS	= $(LINKGENERATORSOURCES:.cpp=.o)

TIMEPOINTSMERGERSOURCES	= ./timepoint_merger_test.cpp ../kmer_freq.cpp ../dictatorcenter.cpp ../util.cpp ../binaryinfilebuf.cpp ../binaryoutfilebuf.cpp ../kmers_bitwisetransform.cpp ../kmers_dictionary.cpp ../dictatorcluster.cpp ../cluster.cpp ../mixturebptester.cpp ../centerrecalibrator.cpp ../centerclustermapper.cpp ../errorrateestimator.cpp ../clustercenterlinkgenerator.cpp ../timepointsmerger.cpp ../centerclustermapperiterator.cpp ../idgenerator.cpp ../kmerdecoder.cpp
TIMEPOINTSMERGEROBJECTS	= $(TIMEPOINTSMERGERSOURCES:.cpp=.o)

ERRORESTIMATORSOURCES	= ./errorrate_estimator_test.cpp ../kmer_freq.cpp ../dictatorcenter.cpp ../util.cpp ../binaryinfilebuf.cpp ../binaryoutfilebuf.cpp ../kmers_bitwisetransform.cpp ../kmers_dictionary.cpp ../dictatorcluster.cpp ../cluster.cpp ../errorrateestimator.cpp ../clusterloader.cpp 
ERRORESTIMATOROBJECTS	= $(ERRORESTIMATORSOURCES:.cpp=.o)

CLUSTEROUTPUTSOURCES	= ./cluster_output_test.cpp ../kmer_freq.cpp ../dictatorcenter.cpp ../util.cpp ../binaryinfilebuf.cpp ../binaryoutfilebuf.cpp ../kmers_bitwisetransform.cpp ../kmers_dictionary.cpp ../dictatorcluster.cpp ../cluster.cpp ../clusteroutput.cpp 
CLUSTEROUTPUTOBJECTS	= $(CLUSTEROUTPUTSOURCES:.cpp=.o)

CLUSTERREADERSOURCES	= ./cluster_reader_test.cpp ../kmer_freq.cpp ../dictatorcenter.cpp ../util.cpp ../kmers_bitwisetransform.cpp ../kmers_dictionary.cpp ../dictatorcluster.cpp ../cluster.cpp ../clusterloader.cpp  ../kmerdecoder.cpp
CLUSTERREADEROBJECTS	= $(CLUSTERREADERSOURCES:.cpp=.o)

SINGLEENDPROCESSORSOURCES	= ./single_end_barcode_processor_test.cpp ../barcodecounter.cpp ../sequence.cpp ../kmers_bitwisetransform.cpp ../kmers_dictionary.cpp ../fastqpattern.cpp ../fastapattern.cpp ../barcodeextractor.cpp ../util.cpp ../binaryoutfilebuf.cpp ../binaryinfilebuf.cpp ../singleendbarcodeprocessor.cpp ../pattern.cpp
SINGLEENDPROCESSOROBJECTS	= $(SINGLEENDPROCESSORSOURCES:.cpp=.o)

CLUSTERALGORITHMSOURCES	= ./cluster_algorithm_test.cpp ../barcodecounter.cpp ../sequence.cpp ../kmers_bitwisetransform.cpp ../kmers_dictionary.cpp ../fastqpattern.cpp ../fastapattern.cpp ../barcodeextractor.cpp ../util.cpp ../binaryoutfilebuf.cpp ../binaryinfilebuf.cpp ../singleendbarcodeprocessor.cpp ../pattern.cpp ../clusteralgorithm.cpp ../dictatorcenter.cpp ../dictatorcluster.cpp ../kmer_freq.cpp ../cluster.cpp
CLUSTERALGORITHMOBJECTS	= $(CLUSTERALGORITHMSOURCES:.cpp=.o)

CLUSTERPIPELINESOURCES	= ./cluster_pipeline_test.cpp ../barcodecounter.cpp ../sequence.cpp ../kmers_bitwisetransform.cpp ../kmers_dictionary.cpp ../fastqpattern.cpp ../fastapattern.cpp ../barcodeextractor.cpp ../util.cpp ../binaryoutfilebuf.cpp ../binaryinfilebuf.cpp ../singleendbarcodeprocessor.cpp ../pattern.cpp ../clusteralgorithm.cpp ../dictatorcenter.cpp ../dictatorcluster.cpp ../kmer_freq.cpp ../cluster.cpp ../clusterpipeline.cpp
CLUSTERPIPELINEOBJECTS	= $(CLUSTERPIPELINESOURCES:.cpp=.o)

SIMILARITYSCORESOURCES	= ../util.cpp  ../kmers_bitwisetransform.cpp ../kmers_dictionary.cpp ./similarity_score_test.cpp ../binaryinfilebuf.cpp ../binaryoutfilebuf.cpp
SIMILARITYSCOREOBJECTS	= $(SIMILARITYSCORESOURCES:.cpp=.o)

SIMULATIONSOURCES	= ./simulation_test.cpp ../barcodecounter.cpp ../sequence.cpp ../kmers_bitwisetransform.cpp ../kmers_dictionary.cpp ../fastqpattern.cpp ../fastapattern.cpp ../barcodeextractor.cpp ../util.cpp ../singleendbarcodeprocessor.cpp ../pattern.cpp ../clusteralgorithm.cpp ../dictatorcenter.cpp ../dictatorcluster.cpp ../kmer_freq.cpp ../cluster.cpp ../clusterpipeline.cpp ../clusteroutput.cpp ../testSimulation.cpp  ../mergebycenters.cpp ../errorrateestimator.cpp ../centerrecalibrator.cpp ../mixturebptester.cpp ../centerclustermapperiterator.cpp ../centerclustermapper.cpp ../proportiontest.cpp ../clustermergertester.cpp ../hypothesistester.cpp ../proportionpooltest.cpp ../clustermergerpooltester.cpp ../clustermergerunpooltester.cpp ../meansequentialestimator.cpp ../clusteringwithtest.cpp ../errorintroducer.cpp ../distanceselector.cpp ../barcodemutationgenerator.cpp ../kmerdecoder.cpp ../qualitytabledumper.cpp ../barcodetabledumper.cpp ../clustertabledumper.cpp ../clustermergeronesampletester.cpp ../oneproportiontest.cpp ../clusterpruner.cpp ../clustersplitter.cpp ../idgenerator.cpp ../meanestimator.cpp
SIMULATIONOBJECTS	= $(SIMULATIONSOURCES:.cpp=.o)

BARCODEANALYSISSOURCES	= ./barcode_analysis.cpp ../barcodecounter.cpp ../sequence.cpp ../kmers_bitwisetransform.cpp ../kmers_dictionary.cpp ../fastqpattern.cpp ../fastapattern.cpp ../barcodeextractor.cpp ../util.cpp ../binaryoutfilebuf.cpp ../binaryinfilebuf.cpp ../singleendbarcodeprocessor.cpp ../pattern.cpp ../clusteralgorithm.cpp ../dictatorcenter.cpp ../dictatorcluster.cpp ../kmer_freq.cpp ../cluster.cpp ../clusterpipeline.cpp ../clusteroutput.cpp ../testSimulation.cpp  ../mergebycenters.cpp ../errorrateestimator.cpp ../centerrecalibrator.cpp ../mixturebptester.cpp ../centerclustermapperiterator.cpp ../centerclustermapper.cpp ../proportiontest.cpp ../clustermergertester.cpp ../hypothesistester.cpp ../proportionpooltest.cpp ../clustermergerpooltester.cpp ../clustermergerunpooltester.cpp ../meansequentialestimator.cpp ../clusteringwithtest.cpp ../errorintroducer.cpp ../distanceselector.cpp ../barcodemutationgenerator.cpp ../kmerdecoder.cpp ../barcodeeditor.cpp ../seedselector.cpp ../countbarcodeprocessor.cpp ../rawbarcodeprocessor.cpp ../qualitytabledumper.cpp ../barcodetabledumper.cpp ../clustertabledumper.cpp
BARCODEANALYSISOBJECTS	= $(BARCODEANALYSISSOURCES:.cpp=.o)

LOADDATACSVRESOURCES	= ./loadDataCSV_test.cpp ../cluster.cpp ../testSimulation.cpp ../util.cpp ../dictatorcluster.cpp ../kmers_bitwisetransform.cpp ../kmer_freq.cpp ../dictatorcenter.cpp ../kmers_dictionary.cpp ../binaryoutfilebuf.cpp ../binaryinfilebuf.cpp
LOADDATACSVOBJECTS	= $(LOADDATACSVRESOURCES:.cpp=.o)
SYSTEMSOURCES	= ./system_test.cpp ../barcodecounter.cpp ../sequence.cpp ../kmers_bitwisetransform.cpp ../kmers_dictionary.cpp ../fastqpattern.cpp ../fastapattern.cpp ../barcodeextractor.cpp ../util.cpp ../binaryoutfilebuf.cpp ../binaryinfilebuf.cpp ../singleendbarcodeprocessor.cpp ../pattern.cpp ../clusteralgorithm.cpp ../dictatorcenter.cpp ../dictatorcluster.cpp ../kmer_freq.cpp ../cluster.cpp ../clusterpipeline.cpp ../clusteroutput.cpp ../testSimulation.cpp ../proportiontest.cpp ../clustermergertester.cpp ../errorintroducer.cpp ../distanceselector.cpp ../hypothesistester.cpp ../clusteringwithtest.cpp ../clustermergerpooltester.cpp ../clustermergerunpooltester.cpp ../meansequentialestimator.cpp ../proportionpooltest.cpp ../barcodemutationgenerator.cpp
SYSTEMOBJECTS	= $(SYSTEMSOURCES:.cpp=.o)

COMBINERSOURCES	= ./timepoints_combiner_test.cpp ../barcodecounter.cpp ../sequence.cpp ../kmers_bitwisetransform.cpp ../kmers_dictionary.cpp ../fastqpattern.cpp ../fastapattern.cpp ../barcodeextractor.cpp ../util.cpp ../binaryoutfilebuf.cpp ../binaryinfilebuf.cpp ../singleendbarcodeprocessor.cpp ../pattern.cpp ../clusteralgorithm.cpp ../dictatorcenter.cpp ../dictatorcluster.cpp ../kmer_freq.cpp ../cluster.cpp ../clusterpipeline.cpp ../clusteroutput.cpp ../testSimulation.cpp ../timepointsmerger.cpp ../multipletimepointsprocessor.cpp ../mixturebptester.cpp ../centerclustermapper.cpp ../centerclustermapperiterator.cpp ../centerhistogramgenerator.cpp ../centerrecalibrator.cpp ../errorrateestimator.cpp ../clusterloader.cpp ../clustercenterlinkgenerator.cpp ../idgenerator.cpp ../mergebycenters.cpp ../proportiontest.cpp ../clustermergertester.cpp ../hypothesistester.cpp ../meansequentialestimator.cpp ../distanceselector.cpp ../clusteringwithtest.cpp ../errorintroducer.cpp ../clustermergerpooltester.cpp ../clustermergerunpooltester.cpp ../proportionpooltest.cpp ../barcodemutationgenerator.cpp ../kmerdecoder.cpp ../clustermergeronesampletester.cpp ../meanestimator.cpp ../oneproportiontest.cpp ../barcodetabledumper.cpp ../clustertabledumper.cpp ../qualitytabledumper.cpp
COMBINEROBJECTS	= $(COMBINERSOURCES:.cpp=.o)

ONEPROPORTIONTESTSOURCES	= ./oneproportiontest_test.cpp ../oneproportiontest.cpp ../hypothesistester.cpp
ONEPROPORTIONTESTOBJECTS	= $(ONEPROPORTIONTESTSOURCES:.cpp=.o)

#ONEPROPORTIONTESTSOURCES	= ../oneproportiontest.cpp ../hypothesistester.cpp ./oneproportiontest_test.cpp 
#ONEPROPORTIONTESTOBJECTS	= $(ONEPROPORTIONTESTSOURCES:.cpp=.o)

MERGERTESTERSOURCES	= ./clustermergertester_test.cpp ../proportiontest.cpp ../clustermergertester.cpp
MERGERTESTEROBJECTS	= $(MERGERTESTERSOURCES:.cpp=.o)

ONESAMPLETESTERSOURCES	= ./onesampleclustermerger_test.cpp ../oneproportiontest.cpp ../clustermergertester.cpp ../clustermergeronesampletester.cpp ../hypothesistester.cpp
ONESAMPLETESTEROBJECTS	= $(ONESAMPLETESTERSOURCES:.cpp=.o)

MEANSEQUENTIALSOURCES	= ./meansequentialestimator_test.cpp ../meansequentialestimator.cpp
MEANSEQUENTIALOBJECTS	= $(MEANSEQUENTIALSOURCES:.cpp=.o)


DISTANCESELECTORSOURCES	= ./distance_selector_test.cpp ../distanceselector.cpp
DISTANCESELECTOROBJECTS = $(DISTANCESELECTORSOURCES:.cpp=.o)

SINGLEREADSPROCESSORSOURCES = ./singlereadsprocessor_test.cpp ../sequence.cpp ../fastqpattern.cpp ../fastapattern.cpp ../barcodeextractor.cpp ../util.cpp ../pattern.cpp ../singlereadsprocessor.cpp ../kmers_dictionary.cpp ../kmers_bitwisetransform.cpp
SINGLEREADSPROCESSOROBJECTS = $(SINGLEREADSPROCESSORSOURCES:.cpp=.o)

.cpp.o:
	$(CC)  -c $(CFLAGS) $< -o $@

singlereadsprocessor_test: $(SINGLEREADSPROCESSOROBJECTS)
	$(CC) $(SINGLEREADSPROCESSOROBJECTS) -o $@ $(LDFLAGS)

clustermerger_test: $(MERGERTESTEROBJECTS)
	$(CC) $(MERGERTESTEROBJECTS) -o $@ $(LDFLAGS)

onesample_test: $(ONESAMPLETESTEROBJECTS)
	$(CC) $(ONESAMPLETESTEROBJECTS) -o $@ $(LDFLAGS)

proportion_test: $(ONEPROPORTIONTESTOBJECTS)
	$(CC) $(ONEPROPORTIONTESTOBJECTS) -o $@ $(LDFLAGS)

distance_selector_test: $(DISTANCESELECTOROBJECTS)
	$(CC) $(DISTANCESELECTOROBJECTS) -o $@ $(LDFLAGS)
meansequentialestimator_test: $(MEANSEQUENTIALOBJECTS)
	$(CC) $(MEANSEQUENTIALOBJECTS) -o $@ $(LDFLAGS)
sequencereader_test: $(SEQUENCEOBJECTS) 
	$(CC) $(SEQUENCEOBJECTS) -o $@ $(LDFLAGS) 
dictionary_test: $(DICTIONARYOBJECTS) 
	$(CC) $(DICTIONARYOBJECTS) -o $@ $(LDFLAGS) 
bitwisetransformer_test: $(TRANSFORMOBJECTS) 
	$(CC) $(TRANSFORMOBJECTS) -o $@ $(LDFLAGS) 
patternparser_test: $(PATTERNPARSEROBJECTS) 
	$(CC) $(PATTERNPARSEROBJECTS) -o $@ $(LDFLAGS) 
barcodeextractor_test: $(BARCODEEXTRACTOROBJECTS)
	$(CC) $(BARCODEEXTRACTOROBJECTS) -o $@ $(LDFLAGS) 
barcodecounter_test: $(BARCODECOUNTEROBJECTS)
	$(CC) $(BARCODECOUNTEROBJECTS) -o $@ $(LDFLAGS) 
singleendbarcodeprocessor_test: $(SINGLEENDPROCESSOROBJECTS)
	$(CC) $(SINGLEENDPROCESSOROBJECTS) -o $@ $(LDFLAGS) 
util_test: $(UTILOBJECTS)
	$(CC) $(UTILOBJECTS) -o $@ $(LDFLAGS) 
center_test: $(CENTEROBJECTS)
	$(CC) $(CENTEROBJECTS) -o $@ $(LDFLAGS) 
cluster_test: $(CLUSTEROBJECTS)
	$(CC) $(CLUSTEROBJECTS) -o $@ $(LDFLAGS) 
cluster_output_test: $(CLUSTEROUTPUTOBJECTS)
	$(CC) $(CLUSTEROUTPUTOBJECTS) -o $@ $(LDFLAGS) 
cluster_reader_test: $(CLUSTERREADEROBJECTS)
	$(CC) $(CLUSTERREADEROBJECTS) -o $@ $(LDFLAGS) 
cluster_algorithm_test: $(CLUSTERALGORITHMOBJECTS)
	$(CC) $(CLUSTERALGORITHMOBJECTS) -o $@ $(LDFLAGS) 
cluster_pipeline_test: $(CLUSTERPIPELINEOBJECTS)
	$(CC) $(CLUSTERPIPELINEOBJECTS) -o $@ $(LDFLAGS) 
simulation_test: $(SIMULATIONOBJECTS)
	$(CC) $(SIMULATIONOBJECTS) -o $@ $(LDFLAGS) 
system_test: $(SYSTEMOBJECTS)
	$(CC) $(SYSTEMOBJECTS) -o $@ $(LDFLAGS) 
centertracker_test: $(CENTERTRACKEROBJECTS)
	$(CC) $(CENTERTRACKEROBJECTS) -o $@ $(LDFLAGS) 
csvreader_test: $(CSVREADEROBJECTS)
	$(CC) $(CSVREADEROBJECTS) -o $@ $(LDFLAGS) 
similarity_test: $(SIMILARITYSCOREOBJECTS)
	$(CC) $(SIMILARITYSCOREOBJECTS) -o $@ $(LDFLAGS) 
errorrate_estimator_test: $(ERRORESTIMATOROBJECTS)
	$(CC) $(ERRORESTIMATOROBJECTS) -o $@ $(LDFLAGS) 
center_calibrator_test: $(CENTERCALIBRATOROBJECTS)
	$(CC) $(CENTERCALIBRATOROBJECTS) -o $@ $(LDFLAGS) 
clustersplitter_test: $(CLUSTERSPLITTEROBJECTS)
	$(CC) $(CLUSTERSPLITTEROBJECTS) -o $@ $(LDFLAGS) 
load_data_csv_test: $(LOADDATACSVOBJECTS)
	$(CC) $(LOADDATACSVOBJECTS) -o $@ $(LDFLAGS)
center_cluster_mapper_test: $(CENTERCLUSTERMAPPEROBJECTS)
	$(CC) $(CENTERCLUSTERMAPPEROBJECTS) -o $@ $(LDFLAGS)
center_cluster_mapper_iterator_test: $(CENTERCLUSTERMAPPERITERATOROBJECTS)
	$(CC) $(CENTERCLUSTERMAPPERITERATOROBJECTS) -o $@ $(LDFLAGS)
link_generator_test: $(LINKGENERATOROBJECTS)
	$(CC) $(LINKGENERATOROBJECTS) -o $@ $(LDFLAGS)
timepoint_merger_test: $(TIMEPOINTSMERGEROBJECTS)
	$(CC) $(TIMEPOINTSMERGEROBJECTS) -o $@ $(LDFLAGS)
combiner_test: $(COMBINEROBJECTS)
	$(CC) $(COMBINEROBJECTS) -o $@ $(LDFLAGS)
kmerdecoder_test: $(KMERDECODEROBJECTS)
	$(CC) $(KMERDECODEROBJECTS) -o $@ $(LDFLAGS)
seedselector_test: $(SEEDSELECTOROBJECTS)
	$(CC) $(SEEDSELECTOROBJECTS) -o $@ $(LDFLAGS)
inputprocessor_test: $(INPUTPROCESSOROBJECTS)
	$(CC) $(INPUTPROCESSOROBJECTS) -o $@ $(LDFLAGS)
barcodeeditor_test: $(BARCODEEDITOROBJECTS)
	$(CC) $(BARCODEEDITOROBJECTS) -o $@ $(LDFLAGS)
barcode_analysis: $(BARCODEANALYSISOBJECTS)
	$(CC) $(BARCODEANALYSISOBJECTS) -o $@ $(LDFLAGS)
