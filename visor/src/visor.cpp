
#include <map>
#include <vector>

#include <cassert>
#include <iostream>
#include <float.h>
#include <math.h>
#include "wit.h"



//#include "CoinFloatEqual.h"
#include "visor.h"
#include "printer.h"
#include "requestQuantity.h"
#include "onHandMaterial.h"
#include "visorProblem1.h"
#include "visorProblem2.h"
#include "VisorFloatEqual.h"
#include "globalAttrData.h"


typedef std::vector<std::string> StdStringVec;


int
main(int argc, char *argv[]) {

    std::cout << "visor Build date: " << __DATE__ << std::endl;
    {

        // Put Parms into a vector
        std::vector<std::string> args;
        for (int i = 0; i < argc; i++)
            args.push_back(argv[i]);

        {
            // Check parm passed to program
            bool parmError = false;

            if (args.size() > 1) {
                if (args[1] == "?") {
                    parmError = true; // set flag to provide usage info
                }
            }

            if (!parmError) {
                if (args.size() != 3) {
                    // Wrong number of parms. Write message
                    std::cerr
                            << "-------------------------------------------------------" << std::endl
                            << "Wrong number of parmeters specified" << std::endl
                            << "  Number parameters specified: " << (int) args.size() - 1 << std::endl
                            << "  Expected number of parameters: 2" << std::endl
                            << "-------------------------------------------------------" << std::endl;
                    parmError = true;
                }
            }


            if (parmError) {
                // Write correct usage
                std::cerr
                        << "-------------------------------------------------------" << std::endl
                        << argv[0] << " dataDirectory  solveType " << std::endl
                        << "  where:" << std::endl
                        << "    dataDirectory:" << std::endl
                        << "      Directory where input files are located and" << std::endl
                        << "      output files are written." << std::endl
                        << "    solveType:" << std::endl
                        << "      If heur, then heuristicImplode, " << std::endl
                        << "      otherwise optImplode." << std::endl
                        << "  example usage:" << std::endl
                        << "    " << argv[0] << " ../data/BrendaData heur" << std::endl
                        << "-------------------------------------------------------" << std::endl;
                return 1;
            }
        }

        std::string outputDirectory = args[1];
        std::string inputDirectory = args[1];
        std::cout << "inputDirectory: " << inputDirectory << std::endl;

        bool useOptImplode = true;
        if (std::string(args[2]) == "heur") useOptImplode = false;
        VISORproblem1 printingProb;
        VISORproblem2 allocProb;
        int nPeriods;
        VisorRelFltEq eq(1.e-05);

        int nWfmvScaleFactors;
        bool writeZeros = true;


        printingProb.setSolverLogFileName(outputDirectory + "/cplexMesgFile.txt");
        //createModel(eso2Prob, inputDirectory, useGlobalAttrFileSettings);
        nPeriods = printingProb.getNPeriods();
        allocProb.setNPeriods(nPeriods);


        // Read Material file and add to model
        {
            std::string onHandMaterialFileName = inputDirectory + "/onHandMaterial.csv";
            VISORonHandMaterial onHandMaterialFile(onHandMaterialFileName);
            VISORonHandMaterialIterator onHandMaterialFileIter(onHandMaterialFile);
            bool dupKeyWarningPrinted = false;

            // loop once for each record in materail file
            int recNo = 1;
            for (; onHandMaterialFileIter() != NULL;) {
                recNo++;
                std::string matLoc = onHandMaterialFileIter.location();
                if (matLoc == "") {
                    std::cout << "WARNING: location not specifed\n";
                    std::cout << "   Filename: " + onHandMaterialFileName + "\n";
                    std::cout << "   Record number: " << recNo << "\n";
                    std::cout << "   Record is ignored\n";
                    continue;
                }

                std::string filSze = onHandMaterialFileIter.filamentSize();
                std::string pType = onHandMaterialFileIter.plasticType();
                float qty = onHandMaterialFileIter.quantityAsFloat();
                int shrPer = onHandMaterialFileIter.shareAsInt();

                // Convert strings to lower/upper case. see https://www.geeksforgeeks.org/conversion-whole-string-uppercase-lowercase-using-stl-c/
                transform(filSze.begin(), filSze.end(), filSze.begin(), ::tolower);
                transform(pType.begin(), pType.end(), pType.begin(), ::toupper);

                if (!(filSze == "1.75mm" || filSze == "2.85mm")) {
                    //std::cout <<"---------------------------------------------\n";
                    std::cout << "WARNING: Unrecognized filament size: " + filSze + "\n";
                    std::cout << "   Expected values: 1.75mm 2.85mm\n";
                    std::cout << "   Filename: " << onHandMaterialFileName << "\n";
                    std::cout << "   Record number: " << recNo << "\n";
                    std::cout << "   location: " + matLoc + "\n";
                    std::cout << "   plasticType: " + pType + "\n";
                    std::cout << "   quantity: " << qty << +"\n";
                    std::cout << "   share: " << shrPer << +"\n";
                    std::cout << "   Record is ignored\n";
                    //std::cout <<"---------------------------------------------\n";
                    continue;
                }

                if (!(pType == "PETG" || pType == "PLA" || pType == "ABS" || pType == "ONYX")) {
                    //std::cout <<"---------------------------------------------\n";
                    std::cout << "WARNING: Unrecognized plasticType: " + pType + "\n";
                    std::cout << "   Expected values: PETG PLA ABS ONYX\n";
                    std::cout << "   Filename: " << onHandMaterialFileName << "\n";
                    std::cout << "   Record number: " << recNo << "\n";
                    std::cout << "   location: " + matLoc + "\n";
                    std::cout << "   filamentSize: " << filSze << "\n";
                    std::cout << "   quantity: " << qty << +"\n";
                    std::cout << "   share: :" << shrPer << +"\n";
                    std::cout << "   Record is ignored\n";
                    //std::cout <<"---------------------------------------------\n";
                    continue;
                }

                if (printingProb.materialExists(matLoc, filSze, pType)) {
                    if (!dupKeyWarningPrinted) {
                        std::cout << "   Cannot have two materials with same location, filamentSize and plasticType\n";
                        std::cout << "   Only first instance used. Others are ignored\n";
                        dupKeyWarningPrinted = true;
                    }
                    continue;
                }


                printingProb.addMaterial(matLoc, filSze, pType, qty, shrPer);
            }
        }

        // Read printer file and add to model
        {
            std::string printerFileName = inputDirectory + "/printer.csv";
            VISORprinter printerFile(printerFileName);
            VISORprinterIterator printerFileIter(printerFile);
            bool dupKeyWarningPrinted = false;

            // loop once for each record in printer file
            int recNo = 1;
            for (; printerFileIter() != NULL;) {
                recNo++;
                std::string pNam = printerFileIter.name();

                if (pNam == "") {
                    std::cout << "WARNING: name not specifed\n";
                    std::cout << "   Filename: " + printerFileName + "\n";
                    std::cout << "   Record number: " << recNo << "\n";
                    std::cout << "   Record is ignored\n";
                    continue;
                }

                std::string pLoc = printerFileIter.location();
                int prodRate = printerFileIter.prodRateAsInt();
                bool f175 = printerFileIter.F175asBool();
                bool f285 = printerFileIter.F285asBool();
                bool petg = printerFileIter.PETGasBool();
                bool pla = printerFileIter.PLAasBool();
                bool abs = printerFileIter.ABSasBool();
                bool onyx = printerFileIter.ONYXasBool();

                if (printingProb.printerExists(pNam, pLoc)) {
                    if (!dupKeyWarningPrinted) {
                        //std::cout <<"---------------------------------------------\n";
                        std::cout << "   Cannot have two printers with same name & location\n";
                        std::cout << "   Only first instance used. Others are ignored\n";
                        //std::cout <<"---------------------------------------------\n";
                        dupKeyWarningPrinted = true;
                    }
                    continue;
                }
                printingProb.addPrinter(pNam, pLoc, prodRate, f175, f285, petg, pla, abs, onyx);
                allocProb.addVisor(pNam, pLoc);
            }
        }

        // Read visor request file
        {
            std::string requestQuantityFileName = inputDirectory + "/requestQuantity.csv";
            VISORrequestQuantity requestQuantityFile(requestQuantityFileName);
            VISORrequestQuantityIterator requestQuantityFileIter(requestQuantityFile);

            // loop once for each record in printer file
            int recNo = 1;
            for (; requestQuantityFileIter() != NULL;) {
                recNo++;
                std::string loc = requestQuantityFileIter.location();

                if (loc == "") {
                    //std::cout <<"---------------------------------------------\n";
                    std::cout << "WARNING: location not specifed\n";
                    std::cout << "   Filename: " + requestQuantityFileName + "\n";
                    std::cout << "   Record number: " << recNo << "\n";
                    std::cout << "   Record is ignored\n";
                    //std::cout <<"---------------------------------------------\n";
                    continue;
                }


                int per = requestQuantityFileIter.dateAsInt();
                int quan = requestQuantityFileIter.requestedQuantityAsInt();
                //std::cout <<recNo <<" "+loc <<"\n";

                allocProb.addVisorRequest(loc, per, quan);
            }
        }

        // Read global attribute file
        {
            std::string globalAttrFileName = inputDirectory + "/globalAttrData.csv";
            if ( SCOfile::fileExist(globalAttrFileName) ) {
                VISORglobalAttrData globalAttrFile(globalAttrFileName);

                // Is eqAllocPerBnd present
                if ( globalAttrFile.keyExists("eqAllocPerBnd") )
                {
                    // Set EqAllocPerBnd
                    int bnd = globalAttrFile.fieldAsInt("eqAllocPerBnd","attrValue");

                    if ( bnd<0 || bnd>=nPeriods )
                    {
                        std::cout << "WARNING: Out of range eqAllocPerBnd: " <<bnd <<"\n";
                        std::cout << "   Expected values: [1," <<nPeriods <<")\n";

                        std::cout << "   Filename: " + globalAttrFileName + "\n";
                        std::cout << "   Record is ignored\n";
                    } else {
                        allocProb.setEqAlloPerBnd(bnd);
                        std::cout << "eqAllocPerBnd set to: " << bnd << std::endl;
                    }
                }


            }
        }

        printingProb.writeWitData(outputDirectory + "/wit1.data");
        allocProb.writeWitData(outputDirectory + "/wit2.data");

        printingProb.solve(useOptImplode);

        //----------------------------------
        // Get soln from first implosoin and set supply for 2nd
        //----------------------------------
        {
            std::vector<std::string> printerName, printerLoc;
            printingProb.getPrinters(printerName, printerLoc);

            // Loop once for each printer
            for (int p = 0; p < printerName.size(); p++) {
                // Get ShipVol from printer
                std::vector<float> sv = printingProb.getPrinterShipVol(printerName[p], printerLoc[p]);

                // Set Supply Vol of 2nd implosion to 1st implosion shipVol
                allocProb.setVisorSupplyVol(printerName[p], printerLoc[p], sv);
            }
        }

        //Do second implosion
        allocProb.solve();

        // --------------------------------------------
        // write subVol file
        // --------------------------------------------
        bool writeHeader = true;
        {
            std::string subVolFileName = outputDirectory + "/subVol.csv";
            FILE *subVolFilePtr = fopen(subVolFileName.c_str(), "w");

            std::string heading;
            heading = "\"printerName\",\"printerLocation\",\"materialLocation\",\"filamentSize\",\"plasticType\",\"period\",\"quantity\",\"own\"";
            if (writeHeader) fprintf(subVolFilePtr, "%s\n", heading.c_str());

            std::vector<std::string> printerName, printerLoc;
            std::vector<std::string> matLoc, matSize, matType, own;
            std::vector<std::vector<float>> subVol;
            printingProb.getSubVol(
                    printerName, printerLoc,
                    matLoc, matSize, matType,
                    subVol, own);

            // Loop once for each sub bom
            for (int s = 0; s < printerName.size(); s++) {
                //std::cout <<printerName[s]+" "+printerLoc[s]+" "+matLoc[s]+" "+matSize[s]+" "+matType[s]+" "+own[s]+"\n";
                for (int t = 0; t < nPeriods; t++) {
                    if (eq(subVol[s][t], 0.0)) continue;

                    fprintf(subVolFilePtr,
                            "\"%s\",\"%s\",\"%s\",\"%s\",\"%s\",%d,%f,\"%s\"\n",
                            printerName[s].c_str(), printerLoc[s].c_str(), matLoc[s].c_str(), matSize[s].c_str(),
                            matType[s].c_str(), t, subVol[s][t], own[s].c_str());
                }
            }

            fclose(subVolFilePtr);
        } // finished writing subVol file

        // --------------------------------------------
        // write prodVol file
        // --------------------------------------------
        {
            std::string prodVolFileName = outputDirectory + "/prodVol.csv";
            FILE *prodVolFilePtr = fopen(prodVolFileName.c_str(), "w");

            std::string heading;
            heading = "\"printerName\",\"printerLocation\",\"period\",\"quantity\"";
            if (writeHeader) fprintf(prodVolFilePtr, "%s\n", heading.c_str());

            std::vector<std::string> printerName, printerLoc;
            printingProb.getPrinters(printerName, printerLoc);

            // Loop once for each printer
            for (int p = 0; p < printerName.size(); p++) {
                // Get printers witOpExecVol
                std::vector<float> ev = printingProb.getPrinterProdVol(printerName[p], printerLoc[p]);
                //std::vector<float> sv=printingProb.getPrinterShipVol(printerName[p],printerLoc[p]);
                for (int t = 0; t < nPeriods; t++) {
                    //assert( eq(ev[t],sv[t]) );
                    if (eq(ev[t], 0.0)) continue;

                    fprintf(prodVolFilePtr,
                            "\"%s\",\"%s\",%d,%f\n",
                            printerName[p].c_str(), printerLoc[p].c_str(), t, ev[t]);
                }
            }

            fclose(prodVolFilePtr);
        } // finished writing prodVol file

        // --------------------------------------------
        // write shipQuantity file
        // --------------------------------------------
        {
            std::string shipQuantityFileName = outputDirectory + "/shipQuantity.csv";
            FILE *shipQuantityFilePtr = fopen(shipQuantityFileName.c_str(), "w");

            std::string heading;
            heading = "\"requestingLocation\",\"producingLocation\",\"period\",\"shipQuantity\"";
            if (writeHeader) fprintf(shipQuantityFilePtr, "%s\n", heading.c_str());

            std::vector<std::string> demands = allocProb.getDemands();

            // Loop once for each demand
            for (int d = 0; d < demands.size(); d++) {
                // Get shipVol to hospital
                std::string dn = demands[d];

                //get the witSubVol for each location making visors
                std::vector<std::string> partLoc;
                std::vector<std::vector<float>> sv;
                allocProb.getSubVols(dn, partLoc, sv);

                for (int l = 0; l < partLoc.size(); l++) {

                    std::string pn = partLoc[l];

                    for (int t = 0; t < nPeriods; t++) {
                        if (eq(sv[l][t], 0.0)) continue;

                        fprintf(shipQuantityFilePtr,
                                "\"%s\",\"%s\",%d,%f\n",
                                dn.c_str(), pn.c_str(), t, sv[l][t]);
                    }
                }
            }

            fclose(shipQuantityFilePtr);
        } // finished writing prodVol file


        //---------------------------------------------------------



    }

    std::cout << "visor application is finished." << std::endl;
    return 0;
}
