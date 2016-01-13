UCNB_Analyzer
=============

This software analyzes data taken using the UCNB/Nab detection system. These experiments will perform precise measurements of the neutron decay correlations B, a, and b.  This software processes the raw event waveforms using trapezoidal filters or pulse fitting, collects coincidences from charge-sharing, backscattering, or electron-proton events, and produces timing and energy spectra.

Requirements
------------

This code is written in C++ using ROOT libraries (cern.root.ch).  It was tested using GNU make version 3.81, gcc version 4.8.4, and ROOT v5-34.  The software is intended for use in Linux-based distributions.

Directions
----------

To build the code, simple run

```
$ make
```

from the top directory of the distribution.  The executables are run from this directory.  You can run ROOT scripts within the ROOT framework from this directory.

Acknowledgements
----------------

UCNB Analyzer is based on work supported by the U.S. Department of Energy, Office of Science, Office of Nuclear Physics.

Copyright and license
---------------------
Los Alamos National Security, LLC (LANS) owns the copyright to UCNB Analyzer.  It is internally identified as LA-CC-15-062.  The license is GNU GPL v2.  See LICENSE.md for full details.