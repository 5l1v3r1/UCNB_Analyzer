UCNB_Analyzer Documentation
===========================

This software processes data taken using the Nab/UCNB NI PXIe-5171R digitizer-based data acquisition system.  It converts the raw binary files into ROOT-friendly TTrees and provides analysis tools for processing waveforms and analyzing event data.

This documentation describes the classes contained in this distribution.

Main program
------------

The main program source code is in src/Analyzer.cpp.  The software is invoked by running the executable Analyzer and passing parameters by flags:

* -f # [#]:  (required) File # or range of files # # to be processed
* -format #: Numerical indicator of which binary format version in use (current = 1)
* -p "":     Path "" to the files to be read/written
* -raw:      Read (raw) events in binary files into TTree in a ROOT TFiles
* -sort:     Sort events by time (necessary if boards aren't synchronized)
* -fit #:    Fit waveforms to find real detector events above threshold #
* -ave #:    Build an average pulse shape from processed events above threshold #
* -coll #:   (In development) Collect detector events into coincidences within a window of # samples

The program DoTrapFilter (source code src/DoTrapFilter.cpp) uses a trapezoidal filter to find events, will eventually be moved to `$ ./Analyzer -trap`.

### Typical use of software

* Process raw binary file
[BinFile] -> `$ ./Analyzer -raw` -> [RawTreeFile]

* Correct time-ordering of events
[RawTreeFile] -> `$ ./Analyzer -sort` -> [RawTreeFile]

* Find triggers using fit to pulse shape
[RawTreeFile] -> `$ ./Analyzer -fit` -> [TrigTreeFile]

* Build average pulse shape
[RawTreeFile, TrigTreeFile] -> `$ ./Analyzer -ave` -> [Ave.root]

* Find triggers using trapezoidal filter algorithm
[RawTreeFile] -> `$ ./DoTrapFilter` -> [TrapTreeFile]

* Collect events into coincidences
[TrigTreeFile] -> `$ ./Analyzer -coll` -> [EventTreeFile]

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

Handles file with TTree of events found from pulse fitting.

##### TrapTreeFile

Handles file with TTree of events found from trapezoidal filtering.

##### EventTreeFile

Handles file with TTree of events from coincident channels.

#### Sorter

Holds a time-sorted linked list of RawTreeFile events.

#### WaveformAnalyzer

Manages processing/fitting/filtering of waveforms.

#### TriggerList

Stores/sorts list of triggers in an event waveform.  Somewhat deprecated, useful in abandoned raw data format with multiple channel waveforms per raw event.

