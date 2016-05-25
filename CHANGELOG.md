UCNB_Analyzer 1.1.0
===========================

Version 1.0.0: initial release.  
Replays waveform data from multiple data formats into ROOT TTrees with waveforms, then into energy/time event TTrees from fitting to a preamp pulse shape or from a trapezoid filter, then events from fitting into event coincidences with defined window.  Includes tool to build an average waveform for a given run.  Includes example script to analyze waveforms and events from fitting or trap filter.

Version 1.1.0:
Automatically calibrates detectors: requires hard-coding run/detector info and reasonably well-behaved source spectra.  Performs shaping time scan from single trap filter on specific lines in resulting calibration spectra: requires reasonably well-behaved source spectra.  Includes script to debug calibration and shaping routines in ROOT, and to process results from UCNB_Analyzer.