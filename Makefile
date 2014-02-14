#******************************************************************************
#
# Makefile - Rules for building the driver library and examples.
#
# Copyright (c) 2005-2013 Texas Instruments Incorporated.  All rights reserved.
# Software License Agreement
# 
#   Redistribution and use in source and binary forms, with or without
#   modification, are permitted provided that the following conditions
#   are met:
# 
#   Redistributions of source code must retain the above copyright
#   notice, this list of conditions and the following disclaimer.
# 
#   Redistributions in binary form must reproduce the above copyright
#   notice, this list of conditions and the following disclaimer in the
#   documentation and/or other materials provided with the  
#   distribution.
# 
#   Neither the name of Texas Instruments Incorporated nor the names of
#   its contributors may be used to endorse or promote products derived
#   from this software without specific prior written permission.
# 
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
# 
# This is part of revision 2.0.1.11577 of the Tiva Firmware Development Package.
#
#******************************************************************************



################################################################################
# Files, Directories, Other
################################################################################

# A lot of what is in these first two sections is duplicated in the file
# makedefs, but we'll leave the duplicates for now as we know that they are
# working.  

# DIRS list is used to call call make in each sub folder. 
DIRS       = driverlib \
             grlib     \
             sensorlib \
             usblib    \
             examples  \
             lm4flash

# All of the source files for the project should be listed here (not headers)
FILES      = startup_gcc.c \
             quad-main.c

# Every listing in FILES above should have a corresponding '.o' entry here.  
# There are "better" ways to do this, but this was the simplist.
OBJS       = startup_gcc.o \
             quad-main.o

# NAME is the name of the project which is assumed to be the basename of the 
# current directory.  This will be used to name the .bin file after linking.
NAME       = ${shell basename ${CURDIR}}

# DEV_SERIAL is used by the lm4flash tool.  The serial code should be printed
# on a sticker on the Tiva C board.  If unsure, or it is not working, simply
# plug in the Tiva C, run 'sudo make load' and look at the serial number that is
# printed out.  It should look something like:
#        Found ICDI device with serial: XXXXXXXX
#        Unable to find any ICDI devices
# where XXXXXXXX is the serial number.  
DEV_SERIAL  = 0E200CC6

# LIB_DIRS should contain a list of the library directories that are generated 
# by the additional Makefiles.  Add any additional libraries here as needed.
LIB_DIRS    = ./driverlib/gcc


# LIBS is a list of the names of the individual library files in the above 
# directories.  Don't forget to remove the 'lib' prefix, and remove the '.a' 
# suffix.
LIBS        = driver



################################################################################
# Toolchain
################################################################################

CC = arm-none-eabi-gcc
LD = arm-none-eabi-ld
OB = arm-none-eabi-objcopy



################################################################################
# Flags (Hope they all work :P)
################################################################################

COMPILE_FLAGS = -g -mthumb -mcpu=cortex-m4 -mfpu=fpv4-sp-d16 -mfloat-abi=softfp\
                -Os -ffunction-sections -fdata-sections -MD -std=c99 -Wall     \
                -pedantic -DPART_TM4C123GH6PM -c -I${CURDIR}                   \
                -DTARGET_IS_BLIZZARD_RA1
                
LINKER_FLAGS  = -T project.ld --entry ResetISR --gc-sections -L${LIB_DIRS}     \
                -l${LIBS}

OBJCOPY_FLAGS = -O binary



################################################################################
# Targets
################################################################################

all: directories
	echo "...Compiling..."
	${CC} ${COMPILE_FLAGS} ${FILES}
	echo "...Linking..."
	${LD} ${OBJS} ${LINKER_FLAGS}
	echo "...Copying Objects..."
	${OB} ${OBJCOPY_FLAGS} a.out ${NAME}.bin

directories:
	@for i in ${DIRS};                  \
	 do                                 \
	     if [ -f $${i}/Makefile ];      \
	     then                           \
	         make -C $${i} || exit $$?; \
	     fi;                            \
	 done

load:
	./lm4flash/lm4flash -s ${DEV_SERIAL} ${NAME}.bin

clean::
	@rm -f core/*.o               \
	@rm -f *.o                    \
	@rm -f *.bin                  \
	@rm -f a.out                  \
	@rm -f *.d                    \
	@rm -f ${wildcard *~} __dummy__
	@rm -f ${wildcard utils/*~} __dummy__
	@for i in ${DIRS};             \
	 do                            \
	     if [ -f $${i}/Makefile ]; \
	     then                      \
	         make -C $${i} clean;  \
	     fi;                       \
	 done
