TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt
CONFIG += c++11

LIBS += -lboost_regex -lboost_iostreams
SOURCES += main.cpp \
    barcodecounter.cpp \
    barcodeextractor.cpp \
    binaryinfilebuf.cpp \
    binaryoutfilebuf.cpp \
    cluster.cpp \
    clusteralgorithm.cpp \
    clusterinput.cpp \
    clusteroutput.cpp \
    clusterpipeline.cpp \
    csvoutput.cpp \
    dictatorcenter.cpp \
    dictatorcluster.cpp \
    fastapattern.cpp \
    fastqpattern.cpp \
    kmer_freq.cpp \
    kmers_binaryreader.cpp \
    kmers_bitwisetransform.cpp \
    kmers_dictionary.cpp \
    pattern.cpp \
    sequence.cpp \
    singleendbarcodeprocessor.cpp \
    testSimulation.cpp \
    util.cpp \
    errorrateestimator.cpp \
    resultevaluation.cpp \
    csvreader.cpp \
    clusterloader.cpp \
    centerrecalibrator.cpp \
    mixturebptester.cpp \
    centerclustermapper.cpp \
    clustercenterlinkgenerator.cpp \
    centerhistogramgenerator.cpp \
    timepointsmerger.cpp \
    singletimepointdata.cpp \
    singletimepointdataiterator.cpp \
    centerclustermapperiterator.cpp \
    multipletimepointsprocessor.cpp \
    idgenerator.cpp \
    mergebycenters.cpp \
    proportiontest.cpp \
    clustermergertester.cpp \
    proportionpooltest.cpp \
    hypothesistester.cpp \
    clustermergerunpooltester.cpp \
    clustermergerpooltester.cpp \
    barcodemutationgenerator.cpp \
    meansequentialestimator.cpp \
    distanceselector.cpp \
    errorintroducer.cpp \
    clusteringwithtest.cpp \
    clustersplitter.cpp

HEADERS += \
    assert_helpers.h \
    barcodecounter.h \
    barcodeextractor.h \
    binaryinfilebuf.h \
    binaryoutfilebuf.h \
    cluster.h \
    clusteralgorithm.h \
    clusterinput.h \
    clusteroutput.h \
    clusterpipeline.h \
    csvoutput.h \
    dictatorcenter.h \
    dictatorcluster.h \
    endian_swap.h \
    fastapattern.h \
    fastqpattern.h \
    filebuf.h \
    formats.h \
    kmer_freq.h \
    kmers_binaryreader.h \
    kmers_bitwisetransform.h \
    kmers_dictionary.h \
    lockfreequeue.h \
    pattern.h \
    patternparser.h \
    sequence.h \
    singleendbarcodeprocessor.h \
    suffix_commons.h \
    testSimulation.h \
    timer.h \
    typdefine.h \
    util.h \
    centerstracker.h \
    errorrateestimator.h \
    resultevaluation.h \
    csvreader.h \
    minicsv.h \
    clusterloader.h \
    centerrecalibrator.h \
    mixturebptester.h \
    centerclustermapper.h \
    clustercenterlinkgenerator.h \
    centerhistogramgenerator.h \
    timepointsmerger.h \
    singletimepointdata.h \
    singletimepointdataiterator.h \
    centerclustermapperiterator.h \
    multipletimepointsprocessor.h \
    idgenerator.h \
    mergebycenters.h \
    split_util.h \
    proportiontest.h \
    clustermergertester.h \
    proportionpooltest.h \
    hypothesistester.h \
    clustermergerunpooltester.h \
    clustermergerpooltester.h \
    barcodemutationgenerator.h \
    meansequentialestimator.h \
    distanceselector.h \
    errorintroducer.h \
    clusteringwithtest.h \
    clustersplitter.h

