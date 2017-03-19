#ifndef TARTGLOBAL_HH
#define TARTGLOBAL_HH

#include <string>

class TDirectory;
class TCollection;

class TArtGlobal
{
public:
  static std::string IncrementName(std::string name);
  static std::string IncrementName(std::string name, TDirectory* directory);
  static std::string IncrementName(std::string name, TCollection* collection);
};

#endif
