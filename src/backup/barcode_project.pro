TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

LIBS += -lboost_regex -lboost_iostreams
QMAKE_CXXFLAGS += -std=c++0x -I$(/home/Download/boost_1_58_0) -w

SOURCES += main.cpp \
    cluster.cpp \
    center.cpp \
    fastapattern.cpp \
    fastqpattern.cpp \
    sequence.cpp \
    dnastring.cpp \
    random_util.cpp \
    random_source.cpp \
    qual.cpp \
    ds.cpp \
    alphabet.cpp \
    rawpattern.cpp \
    util.cpp \
    kmers_binaryreader.cpp \
    kmers_dictionary.cpp \
    binaryoutfilebuf.cpp \
    binaryinfilebuf.cpp \
    barcodecounting.cpp \
    kmers_bitwisetransform.cpp \
    dictatorcenter.cpp \
    kmer_freq.cpp \
    dictatorcluster.cpp \
    clusteralgorithm.cpp \
    clusterpipeline.cpp \
    barcodeextractor.cpp \
    testSimulation.cpp \
    consensuscluster.cpp \
    clusteroutput.cpp \
    clusterinput.cpp \
    pairbarcodeextractor.cpp \
    singleendreader.cpp

HEADERS += \
    cluster.h \
    center.h \
    fastqpattern.h \
    fastapattern.h \
    patternparser.h \
    sequence.h \
    timer.h \
    pattern.h \
    sstring.h \
    dnastring.h \
    typdefine.h \
    suffix_commons.h \
    qual.h \
    filebuf.h \
    ds.h \
    util.h \
    tokenize.h \
    random_util.h \
    random_source.h \
    endian_swap.h \
    assert_helpers.h \
    alphabet.h \
    formats.h \
    read.h \
    treenode.h \
    rawpattern.h \
    regularExpression.h \
    kmers_binaryreader.h \
    binaryoutfilebuf.h \
    kmers_hashset.h \
    lockfreequeue.h \
    kmers_dictionary.h \
    binaryinfilebuf.h \
    barcodecounting.h \
    kmers_bitwisetransform.h \
    dictatorcenter.h \
    kmer_freq.h \
    dictatorcluster.h \
    clusterpipeline.h \
    clusteralgorithm.h \
    barcodeextractor.h \
    testSimulation.h \
    consensuscluster.h \
    clusteroutput.h \
    clusterinput.h \
    pairbarcodeextractor.h \
    singleendreader.h

