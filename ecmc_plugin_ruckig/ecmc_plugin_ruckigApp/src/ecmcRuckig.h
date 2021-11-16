/*************************************************************************\
* Copyright (c) 2019 European Spallation Source ERIC
* ecmc is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
*
*  ecmcRuckig.h
*
*  Created on: Mar 22, 2020
*      Author: anderssandstrom
*
\*************************************************************************/
#ifndef ECMC_RUCKIG_H_
#define ECMC_RUCKIG_H_

#include <stdexcept>
#include "ecmcRuckigDefs.h"
#include "inttypes.h"
#include <string>
//#include "kissfft/kissfft.hh"

class ecmcRuckig  {
 public:

  /** ecmcRuckig class
   * This object can throw: 
   *    - invalid_argument
  */
  ecmcRuckig(int   ruckigIndex,    // index of this object  
             char* configStr);
  ~ecmcRuckig();  

 private:
  void                  parseConfigStr(char *configStr);
  static std::string    to_string(int value);
  char*                 cfgDataSourceStr_;   // Config: data source string
  int                   cfgDbgMode_;         // Config: allow dbg printouts
  int                   ecmcSampleRateHz_;   // ecmc sample rete in Hz
};

#endif  /* ECMC_RUCKIG_H_ */
