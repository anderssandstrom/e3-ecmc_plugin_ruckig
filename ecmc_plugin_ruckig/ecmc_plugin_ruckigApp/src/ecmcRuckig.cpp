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
#include <iostream>



/** ecmc Ruckig class
 * This object can throw: 
 *    - bad_alloc
 *    - invalid_argument
 *    - runtime_error
*/
ecmcRuckig::ecmcRuckig(int ruckigIndex,       // index of this object (if several is created)
                       char* configStr) {

  otg_              = NULL;
  input_            = NULL;
  output_           = NULL;
  cfgDataSourceStr_ = configStr;
  ecmcSampleRateHz_ = 0;
  cfgDbgMode_       = 0;
  cfgDOFs_          = 1 ;
  cfgRateHz_        = getEcmcSampleRate();
  parseConfigStr(configStr); // Assigns all configs

  ecmcSampleRateHz_ = cfgRateHz_;

  // Check valid sample rate
  if(ecmcSampleRateHz_ <= 0) {
    throw
     std::out_of_range("ecmc ruckig: Invalid sample rate"); 
  }
  
  // create ruckig objects
  cfgDOFs_ = 3;
  otg_  = new Ruckig<DynamicDOFs>(cfgDOFs_,1/ecmcSampleRateHz_);
  input_ = new InputParameter<DynamicDOFs>(cfgDOFs_);
  output_ = new OutputParameter<DynamicDOFs>(cfgDOFs_);
  std::cout << "#################################################HEPP 1" << std::endl;
  test(); 
  std::cout << "#################################################HEPP 2" << std::endl;
  test2(); 
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
      else if (!strncmp(pThisOption, ECMC_PLUGIN_DBG_PRINT_OPTION_CMD, strlen(ECMC_PLUGIN_DBG_PRINT_OPTION_CMD))) {
        pThisOption += strlen(ECMC_PLUGIN_DBG_PRINT_OPTION_CMD);
        cfgDbgMode_ = atoi(pThisOption);
      }

      // ECMC_PLUGIN_DOFS_OPTION_CMD
      else if (!strncmp(pThisOption, ECMC_PLUGIN_DOFS_OPTION_CMD, strlen(ECMC_PLUGIN_DOFS_OPTION_CMD))) {
        pThisOption += strlen(ECMC_PLUGIN_DOFS_OPTION_CMD);
        cfgDOFs_ = atoi(pThisOption);
      } 

      // ECMC_PLUGIN_RATE_OPTION_CMD rate in HZ
      else if (!strncmp(pThisOption, ECMC_PLUGIN_RATE_OPTION_CMD, strlen(ECMC_PLUGIN_RATE_OPTION_CMD))) {
        pThisOption += strlen(ECMC_PLUGIN_RATE_OPTION_CMD);
        cfgRateHz_ = atof(pThisOption);
      }

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

int ecmcRuckig::test() {
 // Create instances: the Ruckig OTG as well as input and output parameters
  Ruckig<3> otg {0.01};  // control cycle
  InputParameter<3> input;
  OutputParameter<3> output;
 // Set input parameters
  input.current_position = {0.0, 0.0, 0.5};
  input.current_velocity = {0.0, -2.2, -0.5};
  input.current_acceleration = {0.0, 2.5, -0.5};
  input.target_position = {-5.0, -2.0, -3.5};
  input.target_velocity = {0.0, -0.5, -2.0};
  input.target_acceleration = {0.0, 0.0, 0.5};
  input.max_velocity = {3.0, 1.0, 3.0};
  input.max_acceleration = {3.0, 2.0, 1.0};
  input.max_jerk = {4.0, 3.0, 2.0};
  // Generate the trajectory within the control loop
  std::cout << "t | p1 | p2 | p3" << std::endl;
  while (otg.update(input, output) == Result::Working) {
      auto& p = output.new_position;
      std::cout << output.time << " " << p[0] << " " << p[1] << " " << p[2] << " " << std::endl;
     output.pass_to_input(input);
  }
 std::cout << "Trajectory duration: " << output.trajectory.get_duration() << " [s]." << std::endl;
 
 return 0;
}

int ecmcRuckig::test2() {

  input_->current_position = {0.0, 0.0, 0.5};
  input_->current_velocity = {0.0, -2.2, -0.5};
  input_->current_acceleration = {0.0, 2.5, -0.5};
  input_->target_position = {-5.0, -2.0, -3.5};
  input_->target_velocity = {0.0, -0.5, -2.0};
  input_->target_acceleration = {0.0, 0.0, 0.5};
  input_->max_velocity = {3.0, 1.0, 3.0};
  input_->max_acceleration = {3.0, 2.0, 1.0};
  input_->max_jerk = {4.0, 3.0, 2.0};
  // Generate the trajectory within the control loop
  std::cout << "t | p1 | p2 | p3" << std::endl;
  while (otg_->update(*input_, *output_) == Result::Working) {
      auto& p = output_->new_position;
      std::cout << output_->time << " " << p[0] << " " << p[1] << " " << p[2] << " " << std::endl;
     output_->pass_to_input(*input_);
  }
 std::cout << "Trajectory duration: " << output_->trajectory.get_duration() << " [s]." << std::endl;
 
 return 0;
}
