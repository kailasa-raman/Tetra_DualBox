################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../Tetra_Flash_lnk.cmd 

LIB_SRCS += \
C:/ti/c2000/C2000Ware_5_04_00_00/libraries/flash_api/f28004x/lib/F021_API_F28004x_FPU32.lib 

ASM_SRCS += \
../f28004x_codestartbranch.asm 

C_SRCS += \
../InterruptHandling.c \
../main.c 

C_DEPS += \
./InterruptHandling.d \
./main.d 

OBJS += \
./InterruptHandling.obj \
./f28004x_codestartbranch.obj \
./main.obj 

ASM_DEPS += \
./f28004x_codestartbranch.d 

OBJS__QUOTED += \
"InterruptHandling.obj" \
"f28004x_codestartbranch.obj" \
"main.obj" 

C_DEPS__QUOTED += \
"InterruptHandling.d" \
"main.d" 

ASM_DEPS__QUOTED += \
"f28004x_codestartbranch.d" 

C_SRCS__QUOTED += \
"../InterruptHandling.c" \
"../main.c" 

ASM_SRCS__QUOTED += \
"../f28004x_codestartbranch.asm" 


