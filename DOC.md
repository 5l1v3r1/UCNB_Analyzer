UCNB_Analyzer Documentation
===========================

This software processes data taken using the Nab/UCNB NI PXIe-5171R digitizer-based data acquisition system.  It converts the raw binary files into ROOT-friendly TTrees and provides analysis tools for processing waveforms and analyzing event data.

This documentation describes the classes contained in this distribution.

Main program
------------

The main program source code is in src/Analyzer.cpp.  The software is invoked by running the executable Analyzer and passing parameters by flags:

```
-f # [#]:   	(required) File # or range of files # # to be processed
-p "":    		Path "" to the files to be read/written
-raw:       	Read (raw) events in binary files into TTree in a ROOT TFiles
-format #:  	Numerical indicator of which binary format version in use (current = 1)
-fit #:     	Fit waveforms to find real detector events above threshold #
-trap #:    	Filter waveforms for events using linear trapezoid
-decay #:   	Set linear trap decay constant #
-shaping #: 	Set linear trap shaping time #
-top #:     	Set linear trap flat top length #
-coll #:    	(In development) Collect detector events into coincidences within a window of # samples
-ave #:     	Build an average pulse shape from processed events above threshold #
-cal #:     	Perform an 'automatic' calibration #
-shapescan #:	Perform trap filter shaping scan on specific lines sources #
```

See ExampleUsage.sh and scripts/Example.C

### Classes

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