## CMMPPT (Constrained Materials Management and Production Planning Tool)
CMMPPT is a software tool for optimal and near-optimal allocation of scarce resources, when the resources are utilized in pre-determined sets of material parts (as in a bill of material) and/or sets of capacities (such as machine processing capacity).

This repository contains the CMMPPT source code, testing code and data, documentation, and some application programs.

#### Directories:

* **doc**
  * Documentation
  * Status: Some files maintained; some not
* **wit**
  * CMMPPT source code, build facilities, and some test code
  * Status: Maintained
* **mcl**
  * Message Class Library
  * Used by CMMPPT for issuing messages (textual output)
  * Source code, build facilities, and test code
  * Status: Maintained
* **config**
  * Generic Makefiles used by CMMPPT, MCL, and other programs in this project
  * Status: Maintained
* **wit-sa-test**
  * Data and scripts for testing CMMPPT in Stand-Alone Executable mode.
  * Status: Maintained
* **sce**
  * Supply Capability Engine: A major CMMPPT application program
  * Source code, build facilities, and test code
  * Status: Maintained


#### Running Docker build:

Install docker for your operating system . You can install Community Edition or Enterprise
Clone project to a local folder (git clone git@github.com:IBM/CMMPPT.git)
cd CMMPPT
Build a docker image using the commond bellow. It will build and test CMMPPT.  
   command : docker build . -t cmmppt



