/*************************************************************************\
* Copyright (c) 2019 European Spallation Source ERIC
* ecmc is distributed subject to a Software License Agreement found
* in file LICENSE that is included with this distribution. 
*
*  ecmcFFTWrap.h
*
*  Created on: Mar 22, 2020
*      Author: anderssandstrom
*
\*************************************************************************/
#ifndef ECMC_RUCKIG_WRAP_H_
#define ECMC_RUCKIG_WRAP_H_
#include "ecmcRuckigDefs.h"

# ifdef __cplusplus
extern "C" {
# endif  // ifdef __cplusplus

/** \brief Create new ruckig object
 *
 *  The plugin supports creation of multiple ruckig objects\n
 *  (if loaded several times).\n
 *  The different ruckig are adressed by ruckigindex (in other functions below).\n
 *  The first loaded ruckig get index 0 and then increases for each load.\n
 *  This function call will create the custom asynparameters dedicated for this plugin.\
 *  The configuration string needs to define a data source by:\n
 *  "SOURCE=<data source>;"\n
 *  Example:\n
 *  "SOURCE=ec0.s1.AI_1";\n
 *  \param[in] configStr Configuration string.\n
 *
 *  \return 0 if success or otherwise an error code.\n
 */
int         createRuckig(char *configStr);

/** \brief Deletes all created ruckig objects\n
 *
 * Should be called when destructs.\n
 */

void deleteAllRuckigs();

# ifdef __cplusplus
}
# endif  // ifdef __cplusplus

#endif  /* ECMC_RUCKIG_WRAP_H_ */
