/*
 * config.h
 *
 *  Created on: Apr 1, 2014
 *      Author: Studnicki
 */

#ifndef CONFIG_H_
#define CONFIG_H_

#define SMALL_BITS (28)
#define SMALL_DIGITS (8)


#define MAX_OPCODE 154
#define SPECIAL_OPCODES 1
#define ALL_OPCODES MAX_OPCODE+SPECIAL_OPCODES+1
#define REDUCTIONS 3
#define MAX_PROCESSES 25
#define ATOM_TABLE_SIZE 100
#define EXPORT_TABLE_SIZE 100
#define CODE_BUFFER_SIZE 1024
#define MAX_REG 255
#define X_REGS_ALLOCATED (MAX_REG+3)

#endif /* CONFIG_H_ */
