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
#include <ruckig/ruckig.hpp>
using namespace ruckig;

class ecmcRuckig  {
 public:

  /** ecmcRuckig class
   * This object can throw: 
   *    - invalid_argument
  */
  ecmcRuckig(int   ruckigIndex,    // index of this object  
             char* configStr);
  ~ecmcRuckig();  
  int test();
  int test2();

 private:
  void                  parseConfigStr(char *configStr);
  static std::string    to_string(int value);
  char*                 cfgDataSourceStr_;   // Config: data source string
  int                   cfgDbgMode_;         // Config: allow dbg printouts
  int                   cfgDOFs_;            // Config: degrees of freedom, defaults to 1
  double                cfgRateHz_;          // Config: update rate [Hz], defaults to ecmc rate 
  double                ecmcSampleRateHz_;   // ecmc sample rete in Hz
  
  Ruckig<DynamicDOFs>          *otg_;
  InputParameter<DynamicDOFs>  *input_;
  OutputParameter<DynamicDOFs> *output_;

};

#endif  /* ECMC_RUCKIG_H_ */
