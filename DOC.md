UCNB_Analyzer Documentation
===========================

This software processes data taken using the Nab/UCNB NI PXIe-5171R digitizer-based data acquisition system.  It converts the raw binary files into ROOT-friendly TTrees and provides analysis tools for processing waveforms and analyzing event data.

This documentation describes the classes contained in this distribution.

Main program
------------

The main program source code is in src/Analyzer.cpp.  The software is invoked by running the executable Analyzer and passing parameters by flags:

```
-f # [#]:   		(required) File # or range of files # # to be processed
-p or -path "":		Path "" to the files to be read/written
-raw:       		Read (raw) events in binary files into TTree in a ROOT TFile
-format #:  		Numerical indicator of which binary format version in use (current = 1)
-trig:    			Read trig events in trig binary files into TTree in a ROOT TFile
-trap #:    		Filter waveforms for events using linear trapezoid
-fit #:     		Fit waveforms to find real detector events above threshold #
-decay #:   		Set linear trap decay constant #
-shaping #: 		Set linear trap shaping time #
-top #:     		Set linear trap flat top length #
-coll #:    		Collect detector events into coincidences within a window of # samples
-ave #:     		Build an average pulse shape from processed events above threshold #
-cal #:     		Perform an 'automatic' calibration #
-calibfile "":		Calibration dat file (including path)
-shapescan #:		Perform trap filter shaping scan on specific lines sources #
```

See ExampleUsage.sh for example on how to run the code.

ROOT Scripts
------------

ROOT scripts are located in the scripts/ folder and loaded automatically using the .rootlogon.C file in the main directory.

#### BiPlot.C

Plots a saved 207Bi spectrum (originally produced using option -cal on UCNB January source data set).

#### CheckCal.C

Used in development of calibration and shaping scan routine (options -cal and -shapescan).  Plots/compares energy spectra, fits specific peaks in spectra, plots/fits shaping time scan results.

#### Coincs.C

Used in development of coincidence routine (option -coll).  Simple checks of coincidence statistics.

#### Example.C

Example functions using Analyzer output:  plot waveform with trapezoidal response and fit function, fit energy spectra peak, compare energy spectra for Dec15 UCNB data.

#### LeakageCurrentPlot.C

Plots leakage current vs. temperature data file provided by S. Sjue, LANL.

#### Linearity.C

Plots saved results of linearity study of preamplifiers (originally produced using separate unpublished code by L. Broussard, LANL/ORNL and A. Sprow, UKy)

#### RiseTimePlot.C

Plots rise time of a waveform, overlayed with saved simulation of waveform produced by E. Frlez, UVA.

#### ShapeTable.C

Histograms saved results of noise coefficients from fits of 113Sn or 139Ce x-rays shaping time scan, as determined using CheckCal.C.

#### UCNPlot.C

Produces electron-proton coincidence plots and associated electron energy spectra with/without HV/background, including cuts on noisy time periods for specific pixels, for January and February UCNB data sets.

Classes
------------

#### BinFile

Base class for raw binary file wrappers, with basic methods for file operations.

##### NIFeb2015BinFile

Reads events from binary files created using the February 2015 file format (separate file for each board).

##### NIJune2015BinFile

Reads events from binary files created using the June 2015 or later file format (single file per run).

#### TreeFile

Base class for ROOT TFiles, with basic methods for file and TTree operations.

##### RawTreeFile

Handles file with TTree of waveforms.

##### TrigTreeFile

Handles file with TTree of events from DAQ .trig files.

##### TrapTreeFile

Handles file with TTree of events found from trapezoidal filtering.

##### FitTreeFile

Handles file with TTree of events found from pulse fitting.

##### EventTreeFile

Handles file with TTree of events from coincident channels.

#### WaveformAnalyzer

Manages processing/fitting/filtering of waveforms.

#### SiCalibrator

Calibrates detector using known sources

##### CalibSource

Contains source line definitions