/*************************************************************************\
* Copyright (c) 2019 European Spallation Source ERIC
* ecmc is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
*
*  ecmcRuckigWrap.cpp
*
*  Created on: Mar 22, 2020
*      Author: anderssandstrom
*      Credits to  https://github.com/sgreg/dynamic-loading 
*
\*************************************************************************/

// Needed to get headers in ecmc right...
#define ECMC_IS_PLUGIN

#include <vector>
#include <stdexcept>
#include <string>
#include "ecmcRuckigWrap.h"
#include "ecmcRuckig.h"
#include "ecmcRuckigDefs.h"



static std::vector<ecmcRuckig*>  ruckigs;
static int                    ruckigObjCounter = 0;

int createRuckig(char* configStr) {

  // create new ecmcRuckig object
  ecmcRuckig* ruckig = NULL;

  // create asynport name for new object ()

  try {
    ruckig = new ecmcRuckig(ruckigObjCounter, configStr);
  }
  catch(std::exception& e) {
    if(ruckig) {
      delete ruckig;
    }
    printf("Exception: %s. Plugin will unload.\n",e.what());
    return ECMC_PLUGIN_RUCKIG_ERROR_CODE;
  }
  
  ruckigs.push_back(ruckig);
  ruckigObjCounter++;

  return 0;
}

void deleteAllFFTs() {
  for(std::vector<ecmcRuckig*>::iterator pruckig = ruckigs.begin(); pruckig != ruckigs.end(); ++pruckig) {
    if(*pruckig) {
      delete (*pruckig);
    }
  }
}
