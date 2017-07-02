CC	= g++
CFLAGS	= -O3 -std=c++11 -I${BOOST_INSTALL_DIR}/include -pthread
EXTRACTORLDFLAGS	= -L${BOOST_INSTALL_DIR}/lib -lboost_regex -lboost_iostreams
SINGLELDFLAGS	= -pthread

SINGLE	= bartender_single
EXTRACTOR = bartender_extractor
COMBINER = bartender_combiner
SINGLEWRAPPER	= bartender_single_com
EXTRACTORWRAPPER	= bartender_extractor_com
COMBINERWRAPPER	= bartender_combiner_com
INSTALLDIR	= /usr/local/bin

# command

RM	=rm
CP	=cp
CD	=cd
MV	=mv

	
BARTENDERSINGLESOURCES	= ./src/barcodemutationgenerator.cpp ./src/barcodetabledumper.cpp ./src/centerclustermapper.cpp ./src/centerhistogramgenerator.cpp ./src/centerrecalibrator.cpp ./src/barcodecluster.cpp ./src/clusteralgorithm.cpp ./src/clustercenterlinkgenerator.cpp ./src/clusteringwithtest.cpp ./src/clustermergerpooltester.cpp ./src/clustermergertester.cpp ./src/clustermergerunpooltester.cpp ./src/clusteroutput.cpp ./src/clustersplitter.cpp ./src/clustertabledumper.cpp ./src/distanceselector.cpp ./src/errorrateestimator.cpp ./src/hypothesistester.cpp ./src/idgenerator.cpp ./src/kmers_dictionary.cpp ./src/bartender_single.cpp ./src/meansequentialestimator.cpp ./src/mergebycenters.cpp ./src/mixturebptester.cpp ./src/proportionpooltest.cpp ./src/proportiontest.cpp ./src/qualitytabledumper.cpp ./src/seedselector.cpp ./src/util.cpp ./src/centerclustermapperiterator.cpp ./src/clustermergeronesampletester.cpp ./src/oneproportiontest.cpp ./src/clusterpruner.cpp ./src/meanestimator.cpp ./src/pcrprocessor.cpp ./src/rawbarcodeloader.cpp ./src/barcodepool.cpp ./src/clusteringdriver.cpp ./src/clustersbucketer.cpp ./src/barcodeindexer.cpp ./src/threadwrapper.cpp ./src/clusterThreadBatcher.cpp ./src/barcodepoolstatistics.cpp ./src/errorestimator.cpp ./src/bpfrequencytracker.cpp ./src/entropyseedselector.cpp ./src/jointentropyseedselector.cpp ./src/miseedselector.cpp ./src/pairwisemeasurement.cpp ./src/pairwisemi.cpp ./src/mutualinformationcalculator.cpp ./src/seedpositionpool.cpp ./src/jointentropy.cpp
BARTENDERSINGLEOBJECTS=$(BARTENDERSINGLESOURCES:.cpp=.o)

BARTENDEREXTRACTORSOURCES	= ./src/kmerdecoder.cpp ./src/kmers_bitwisetransform.cpp ./src/kmers_dictionary.cpp ./src/sequence.cpp ./src/util.cpp ./src/singlereadsprocessor.cpp ./src/bartender_extractor.cpp ./src/barcodeextractor.cpp ./src/pattern.cpp ./src/fastapattern.cpp ./src/fastqpattern.cpp ./src/UmiExtractor.cpp
BARTENDEREXTRACTOROBJECTS=$(BARTENDEREXTRACTORSOURCES:.cpp=.o)


COMBINERSOURCES	= ./src/bartender_combiner.cpp ./src/kmers_dictionary.cpp ./src/util.cpp ./src/barcodecluster.cpp ./src/clusteroutput.cpp ./src/testSimulation.cpp ./src/timepointsmerger.cpp ./src/multipletimepointsprocessor.cpp ./src/centerclustermapper.cpp ./src/centerclustermapperiterator.cpp ./src/errorrateestimator.cpp ./src/clusterloader.cpp ./src/clustercenterlinkgenerator.cpp ./src/idgenerator.cpp ./src/mergebycenters.cpp ./src/barcodemutationgenerator.cpp ./src/barcodetabledumper.cpp ./src/clustertabledumper.cpp ./src/qualitytabledumper.cpp ./src/barcodepool.cpp ./src/kmers_bitwisetransform.cpp
COMBINEROBJECTS	= $(COMBINERSOURCES:.cpp=.o)

all: $(BARTENDERSINGLESOURCES) $(SINGLE) $(COMBINER) $(EXTRACTOR)
install:
	
	$(CP) $(SINGLE)	$(INSTALLDIR)/ 
	$(CP) $(EXTRACTOR)	$(INSTALLDIR)/ 
	$(CP) $(COMBINER) $(INSTALLDIR)/
	$(CP) $(SINGLEWRAPPER) $(INSTALLDIR)/
	$(CP) $(EXTRACTORWRAPPER) $(INSTALLDIR)/
	$(CP) $(COMBINERWRAPPER) $(INSTALLDIR)/
uninstall:
	$(RM) -r $(INSTALLDIR)/$(SINGLE)
	$(RM) -r $(INSTALLDIR)/$(EXTRACTOR)
	$(RM) -r $(INSTALLDIR)/$(SINGLEWRAPPER)
	$(RM) -r $(INSTALLDIR)/$(EXTRACTORWRAPPER)
	$(RM) -r $(INSTALLDIR)/$(COMBINER)
	$(RM) -r $(INSTALLDIR)/$(COMBINERWRAPPER)
clean:
	$(RM) $(BARTENDERSINGLEOBJECTS)
	$(RM) $(BARTENDEREXTRACTOROBJECTS)
	$(RM) $(SINGLE)
	$(RM) $(EXTRACTOR)
	$(RM) $(COMBINER)
$(SINGLE): $(BARTENDERSINGLEOBJECTS) 
	$(CC) $(BARTENDERSINGLEOBJECTS) -o $@ $(SINGLELDFLAGS)
$(EXTRACTOR): $(BARTENDEREXTRACTOROBJECTS) 
	$(CC) $(BARTENDEREXTRACTOROBJECTS) -o $@ $(EXTRACTORLDFLAGS) 
$(COMBINER): $(COMBINEROBJECTS) 
	$(CC) $(COMBINEROBJECTS) -o $@ $(COMBINERLDFLAGS) 
.cpp.o:
	$(CC)  -c $(CFLAGS) $< -o $@

