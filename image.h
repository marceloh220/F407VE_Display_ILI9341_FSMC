/**  
* @file image.h  
* @brief Image bitmap type for print
*        assci images saved. 
*  
* @author Marcelo H Moraes 
* 
* @date 09/13/2021
*
* Copyright (c) 2021, Marcelo H Moraes
* SPDX-License-Identifier: Apache-2.0
*
* Licensed under the Apache License, Version 2.0 (the "License");
* you may not use this file except in compliance with the License.
*
* You may obtain a copy of the License at http://www.apache.org/licenses/LICENSE-2.0
*
* Unless required by applicable law or agreed to in writing, software
* distributed under the License is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND,
* either express or implied.
*
* See the License for the specific language governing permissions and limitations under the License.
*/ 

#ifndef _IMAGE_H_
#define _IMAGE_H_

#include <inttypes.h>

typedef struct {
	uint16_t width;
	uint16_t height;
	uint16_t bytesPerLine;
	uint8_t bitsPerPixel;
	const uint8_t *pData;
} sImage_t;

extern const sImage_t bmSTLogo;

#endif /* _IMAGE_H_ */
