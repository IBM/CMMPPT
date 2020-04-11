#ifndef VISORproblem1_h
#define VISORproblem1_h


#include "visorProblem.h"


class VISORproblem1 : public VISORproblem
{
public:


  //----------------------
  // Methods for material.
  //----------------------
  void addMaterial(const std::string &location, const std::string &filamentSize, const std::string &plasticType,
                   float quantity, int sharePercent);

  bool materialExists(const std::string &location, const std::string &filamentSize, const std::string &plasticType);

  // Get list of all materials added
  void getMaterials(std::vector<std::string> &location, std::vector<std::string> &filamentSize,
                    std::vector<std::string> &plasticType);

  std::vector<float> getOwnSupply(const std::string &loc, const std::string &nSize, const std::string &pType);

  std::vector<float> getSharedSupply(const std::string &loc, const std::string &nSize, const std::string &pType);


  //-------------------------------------------------------------------------
  // material Name Methods
  //-------------------------------------------------------------------------
  std::string
  ownMaterialName(const std::string &location, const std::string &filamentSize, const std::string &plasticType);

  std::string
  shrMaterialName(const std::string &location, const std::string &filamentSize, const std::string &plasticType);

  std::string
  baseMaterialName(const std::string &location, const std::string &filamentSize, const std::string &plasticType);

  std::string locationFromMaterialName(const std::string &matName);

  std::string filamentSizeFromMaterialName(const std::string &matName);

  std::string plasticTypeFromMaterailName(const std::string &matName);

  bool ownSupply(const std::string &matName);

  //----------------------
  // Printer Methods
  //----------------------
  void addPrinter(const std::string &name, const std::string &location,
                  float prodRate,
                  bool n175, bool n285,
                  bool petg, bool pla, bool abs, bool onyx);

  void getPrinters(std::vector<std::string> &names, std::vector<std::string> &locs);

  bool printerExists(const std::string &pNam, std::string &pLoc);

  std::vector<float> getPrinterProdRate(const std::string &name, const std::string &loc);

  std::vector<float> getPrinterShipVol(const std::string &name, const std::string &loc);

  std::vector<float> getPrinterProdVol(const std::string &name, const std::string &loc);


  //-------------------------------------------------------------------------
  // printer Name Methods
  //-------------------------------------------------------------------------
  std::string printerName(const std::string &name, const std::string &location);

  std::string printerOperName(const std::string &name, const std::string &location);

  std::string noSupplyPartName(const std::string &name, const std::string &location);

  std::string visorPartName(const std::string &name, const std::string &location);

  std::string basePrinterName(const std::string &name, const std::string &location);

  std::string printerFromPrinterName(const std::string &baseName);

  std::string locationFromPrinterName(const std::string &baseName);


  //------------------------------------------
  // Subs Bom Entry methods
  //-----------------------------------------
  void getSubVol(
      std::vector<std::string> &printerName, std::vector<std::string> &printerLoc,
      std::vector<std::string> &matLoc, std::vector<std::string> &matSize, std::vector<std::string> &matType,
      std::vector<std::vector<float>> &subVol, std::vector<std::string> &own);


  //--------------------
  // Solve Methods.
  //--------------------
  void solve();



  //----------------------------------------------------------------------

  // default constructor
  VISORproblem1();

  // copy constructor
  VISORproblem1(const VISORproblem1 &source);

  // assignment operator
  VISORproblem1 &
  operator=(const VISORproblem1 &rhs);

  // destructor
  ~VISORproblem1();


  // Self-test
  static void test();


public:


private:


  void gutsOfCopy(const VISORproblem1 &source);

  void gutsOfDestructor();


  std::set<std::string> materialBaseNames_;
  std::set<std::string> printerBaseNames_;

};


#endif
