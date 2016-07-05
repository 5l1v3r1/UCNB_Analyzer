UCNB_Analyzer
=============

This software analyzes data taken using the UCNB/Nab detection system. These experiments will perform precise measurements of the neutron decay correlations B, a, and b.  This software processes the raw event waveforms using trapezoidal filters or pulse fitting, collects coincidences from charge-sharing, backscattering, or electron-proton events, and produces timing and energy spectra.

See DOC.md for more information.

Collaboration
-------------
is welcome.  Contact Leah Broussard (broussardlj@ornl.gov).

Requirements
------------

This code is written in C++ using ROOT libraries (cern.root.ch).  It was tested using GNU make version 3.81, gcc version 4.8.4, and ROOT v5-34/34.  The software is intended for use in Linux-based distributions.

Directions
----------

To build the code, simple run

```
$ make clean
$ make
```

from the top directory of the distribution.  The executables are run from this directory.  Header files are in the inc/ directory and source code is in the src/ directory.  You can run ROOT scripts within the ROOT framework from the top directory.  An example script is included in scripts/ and a .rootlogon.C file is included in the top directory.

I haven't debugged all the dependency failures from switching commits, so it's safest to run make clean first.

Acknowledgements
----------------

UCNB_Analyzer is based on work supported by the U.S. Department of Energy, Office of Science, Office of Nuclear Physics.

Copyright and license
---------------------
Copyright 2016 UT-Battelle, LLC. Copyright 2016 Los Alamos National Security, LLC. The license is GNU GPL v2. See LICENSE.md for details.