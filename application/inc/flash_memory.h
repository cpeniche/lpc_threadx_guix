/*
 * FLASH_MEMORY.h
 *
 *  Created on: Feb 28, 2024
 *      Author: carlo
 */

#ifndef _FLASH_MEMORY_H_
#define _FLASH_MEMORY_H_

class FLASH_MEMORY
{
public:
	FLASH_MEMORY ();
	virtual
	~FLASH_MEMORY ();
   void Init(unsigned int (*delay)(uint32_t time));
   void IO_config();
   void Clear(uint32_t *start, uint32_t *end);
};

#endif /* _FLASH_MEMORY_H_ */
