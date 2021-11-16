/*************************************************************************\
* Copyright (c) 2019 European Spallation Source ERIC
* ecmc is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
*
*  ecmcRuckig.cpp
*
*  Created on: Mar 22, 2020
*      Author: anderssandstrom
*      Credits to  https://github.com/sgreg/dynamic-loading 
*
\*************************************************************************/

// Needed to get headers in ecmc right...
#define ECMC_IS_PLUGIN


#include <sstream>
#include "ecmcRuckig.h"
#include "ecmcPluginClient.h"
#include <string.h>


/** ecmc Ruckig class
 * This object can throw: 
 *    - bad_alloc
 *    - invalid_argument
 *    - runtime_error
*/
ecmcRuckig::ecmcRuckig(int ruckigIndex,       // index of this object (if several is created)
                       char* configStr) {
  cfgDataSourceStr_ = configStr;
  ecmcSampleRateHz_    = getEcmcSampleRate();
  cfgDbgMode_       = 0;


  parseConfigStr(configStr); // Assigns all configs

  // Check valid sample rate
  if(ecmcSampleRateHz_ <= 0) {
    throw std::out_of_range("ecmc ruckig: Invalid sample rate"); 
  }
  
}

ecmcRuckig::~ecmcRuckig() {
  
}

void ecmcRuckig::parseConfigStr(char *configStr) {

  // check config parameters
  if (configStr && configStr[0]) {    
    char *pOptions = strdup(configStr);
    char *pThisOption = pOptions;
    char *pNextOption = pOptions;
    
    while (pNextOption && pNextOption[0]) {
      pNextOption = strchr(pNextOption, ';');
      if (pNextOption) {
        *pNextOption = '\0'; /* Terminate */
        pNextOption++;       /* Jump to (possible) next */
      }
      
      // ECMC_PLUGIN_DBG_PRINT_OPTION_CMD (1/0)
      if (!strncmp(pThisOption, ECMC_PLUGIN_DBG_PRINT_OPTION_CMD, strlen(ECMC_PLUGIN_DBG_PRINT_OPTION_CMD))) {
        pThisOption += strlen(ECMC_PLUGIN_DBG_PRINT_OPTION_CMD);
        cfgDbgMode_ = atoi(pThisOption);
      } 
      
//      // ECMC_PLUGIN_SOURCE_OPTION_CMD (Source string)
//      else if (!strncmp(pThisOption, ECMC_PLUGIN_SOURCE_OPTION_CMD, strlen(ECMC_PLUGIN_SOURCE_OPTION_CMD))) {
//        pThisOption += strlen(ECMC_PLUGIN_SOURCE_OPTION_CMD);
//        cfgDataSourceStr_=strdup(pThisOption);
//      }
//
//      // ECMC_PLUGIN_RATE_OPTION_CMD rate in HZ
//      else if (!strncmp(pThisOption, ECMC_PLUGIN_RATE_OPTION_CMD, strlen(ECMC_PLUGIN_RATE_OPTION_CMD))) {
//        pThisOption += strlen(ECMC_PLUGIN_RATE_OPTION_CMD);
//        cfgFFTSampleRateHz_ = atof(pThisOption);
//      }

      pThisOption = pNextOption;
    }    
    free(pOptions);
  }
}

// Avoid issues with std:to_string()
std::string ecmcRuckig::to_string(int value) {
  std::ostringstream os;
  os << value;
  return os.str();
}
