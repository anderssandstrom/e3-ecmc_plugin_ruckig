/*************************************************************************\
* Copyright (c) 2019 European Spallation Source ERIC
* ecmc is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
*
*  ecmcPluginRuckig.cpp
*
*  Created on: Mar 21, 2020
*      Author: anderssandstrom
*
\*************************************************************************/

// Needed to get headers in ecmc right...
#define ECMC_IS_PLUGIN
#define ECMC_EXAMPLE_PLUGIN_VERSION 2

#ifdef __cplusplus
extern "C" {
#endif  // ifdef __cplusplus

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "ecmcPluginDefs.h"
#include "ecmcRuckigDefs.h"
#include "ecmcRuckigWrap.h"

static int    lastEcmcError   = 0;
static char*  lastConfStr         = NULL;

/** Optional. 
 *  Will be called once after successfull load into ecmc.
 *  Return value other than 0 will be considered error.
 *  configStr can be used for configuration parameters.
 **/
int ruckigConstruct(char *configStr)
{
  //This module is allowed to load several times so no need to check if loaded

  // create ruckig object and register data callback
  lastConfStr = strdup(configStr);
  return createRuckig(configStr);
}

/** Optional function.
 *  Will be called once at unload.
 **/
void ruckigDestruct(void)
{
  deleteAllRuckigs();
  if(lastConfStr){
    free(lastConfStr);
  }
}

/** Optional function.
 *  Will be called each realtime cycle if definded
 *  ecmcError: Error code of ecmc. Makes it posible for 
 *  this plugin to react on ecmc errors
 *  Return value other than 0 will be considered to be an error code in ecmc.
 **/
int ruckigRealtime(int ecmcError)
{ 
  lastEcmcError = ecmcError;
  return 0;
}

/** Link to data source here since all sources should be availabe at this stage
 *  (for example ecmc PLC variables are defined only at enter of realtime)
 **/
int ruckigEnterRT(){
  return 0;
}

/** Optional function.
 *  Will be called once just before leaving realtime mode
 *  Return value other than 0 will be considered error.
 **/
int ruckigExitRT(void){
  return 0;
}

// Plc function for clear of buffers
double ruckig_test(double index) {
  //return (double)test((int)index);
  return 0;
}

// Register data for plugin so ecmc know what to use
struct ecmcPluginData pluginDataDef = {
  // Allways use ECMC_PLUG_VERSION_MAGIC
  .ifVersion = ECMC_PLUG_VERSION_MAGIC, 
  // Name 
  .name = "ecmcPlugin_ruckig",
  // Description
  .desc = "ruckig plugin for use with ecmc.",
  // Option description
  .optionDesc = "\n    "ECMC_PLUGIN_DBG_PRINT_OPTION_CMD"<1/0>    : Enables/disables printouts from plugin, default = disabled.\n", 
  // Plugin version
  .version = ECMC_EXAMPLE_PLUGIN_VERSION,
  // Optional construct func, called once at load. NULL if not definded.
  .constructFnc = ruckigConstruct,
  // Optional destruct func, called once at unload. NULL if not definded.
  .destructFnc = ruckigDestruct,
  // Optional func that will be called each rt cycle. NULL if not definded.
  .realtimeFnc = ruckigRealtime,
  // Optional func that will be called once just before enter realtime mode
  .realtimeEnterFnc = ruckigEnterRT,
  // Optional func that will be called once just before exit realtime mode
  .realtimeExitFnc = ruckigExitRT,
  // PLC funcs
  .funcs[0] =
      { /*----ruckig_clear----*/
        // Function name (this is the name you use in ecmc plc-code)
        .funcName = "ruckig_clear",
        // Function description
        .funcDesc = "double ruckig_clear(index) : Clear/reset ruckig[index].",
        /**
        * 7 different prototypes allowed (only doubles since reg in plc).
        * Only funcArg${argCount} func shall be assigned the rest set to NULL.
        **/
        .funcArg0 = NULL,
        .funcArg1 = ruckig_test,
        .funcArg2 = NULL,
        .funcArg3 = NULL,
        .funcArg4 = NULL,
        .funcArg5 = NULL,
        .funcArg6 = NULL,
        .funcArg7 = NULL,
        .funcArg8 = NULL,
        .funcArg9 = NULL,
        .funcArg10 = NULL,
        .funcGenericObj = NULL,
      }
};

ecmc_plugin_register(pluginDataDef);

# ifdef __cplusplus
}
# endif  // ifdef __cplusplus
