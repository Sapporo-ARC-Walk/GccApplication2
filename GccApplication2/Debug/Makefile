################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL := cmd.exe
RM := rm -rf

USER_OBJS :=

LIBS := 
PROJ := 

O_SRCS := 
C_SRCS := 
S_SRCS := 
S_UPPER_SRCS := 
OBJ_SRCS := 
ASM_SRCS := 
PREPROCESSING_SRCS := 
OBJS := 
OBJS_AS_ARGS := 
C_DEPS := 
C_DEPS_AS_ARGS := 
EXECUTABLES := 
OUTPUT_FILE_PATH :=
OUTPUT_FILE_PATH_AS_ARGS :=
AVR_APP_PATH :=$$$AVR_APP_PATH$$$
QUOTE := "
ADDITIONAL_DEPENDENCIES:=
OUTPUT_FILE_DEP:=
LIB_DEP:=
LINKER_SCRIPT_DEP:=

# Every subdirectory with source files must be described here
SUBDIRS := 


# Add inputs and outputs from these tool invocations to the build variables 
C_SRCS +=  \
../GccApplication2.c \
../SerialManager.c


PREPROCESSING_SRCS += 


ASM_SRCS += 


OBJS +=  \
GccApplication2.o \
SerialManager.o

OBJS_AS_ARGS +=  \
GccApplication2.o \
SerialManager.o

C_DEPS +=  \
GccApplication2.d \
SerialManager.d

C_DEPS_AS_ARGS +=  \
GccApplication2.d \
SerialManager.d

OUTPUT_FILE_PATH +=GccApplication2.elf

OUTPUT_FILE_PATH_AS_ARGS +=GccApplication2.elf

ADDITIONAL_DEPENDENCIES:=

OUTPUT_FILE_DEP:= ./makedep.mk

LIB_DEP+=  \
D:/robo/embeded_c(cm700_v1.02)/lib/libdynamixel.a \
D:/robo/embeded_c(cm700_v1.02)/lib/libserial.a \
D:/robo/embeded_c(cm700_v1.02)/lib/libzigbee.a

LINKER_SCRIPT_DEP+= 


# AVR32/GNU C Compiler





./%.o: .././%.c
	@echo Building file: $<
	@echo Invoking: AVR/GNU C Compiler : 4.3.3
	$(QUOTE)C:\WinAVR-20100110\bin\avr-gcc.exe$(QUOTE)  -x c -funsigned-char -funsigned-bitfields -DDEBUG  -I"../../../../include"  -O1 -ffunction-sections -fdata-sections -fpack-struct -fshort-enums -mrelax -g2 -Wall  -mmcu=atmega2561  -c -std=gnu99 -MD -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.d)" -MT"$(@:%.o=%.o)"   -o "$@" "$<" 
	@echo Finished building: $<
	



# AVR32/GNU Preprocessing Assembler



# AVR32/GNU Assembler




ifneq ($(MAKECMDGOALS),clean)
ifneq ($(strip $(C_DEPS)),)
-include $(C_DEPS)
endif
endif

# Add inputs and outputs from these tool invocations to the build variables 

# All Target
all: $(OUTPUT_FILE_PATH) $(ADDITIONAL_DEPENDENCIES)

$(OUTPUT_FILE_PATH): $(OBJS) $(USER_OBJS) $(OUTPUT_FILE_DEP) $(LIB_DEP) $(LINKER_SCRIPT_DEP)
	@echo Building target: $@
	@echo Invoking: AVR/GNU Linker : 4.3.3
	$(QUOTE)C:\WinAVR-20100110\bin\avr-gcc.exe$(QUOTE) -o$(OUTPUT_FILE_PATH_AS_ARGS) $(OBJS_AS_ARGS) $(USER_OBJS) $(LIBS) -Wl,-Map="GccApplication2.map" -Wl,--start-group -Wl,-ldynamixel -Wl,-lserial -Wl,-lzigbee  -Wl,--end-group -Wl,-L"D:\robo\embeded_c(cm700_v1.02)\lib"  -Wl,--gc-sections -mrelax  -mmcu=atmega2561   
	@echo Finished building target: $@
	"C:\WinAVR-20100110\bin\avr-objcopy.exe" -O ihex -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures  "GccApplication2.elf" "GccApplication2.hex"
	"C:\WinAVR-20100110\bin\avr-objcopy.exe" -j .eeprom  --set-section-flags=.eeprom=alloc,load --change-section-lma .eeprom=0  --no-change-warnings -O ihex "GccApplication2.elf" "GccApplication2.eep" || exit 0
	"C:\WinAVR-20100110\bin\avr-objdump.exe" -h -S "GccApplication2.elf" > "GccApplication2.lss"
	"C:\WinAVR-20100110\bin\avr-objcopy.exe" -O srec -R .eeprom -R .fuse -R .lock -R .signature -R .user_signatures "GccApplication2.elf" "GccApplication2.srec"
	"C:\WinAVR-20100110\bin\avr-size.exe" "GccApplication2.elf"
	
	





# Other Targets
clean:
	-$(RM) $(OBJS_AS_ARGS) $(EXECUTABLES)  
	-$(RM) $(C_DEPS_AS_ARGS)   
	rm -rf "GccApplication2.elf" "GccApplication2.a" "GccApplication2.hex" "GccApplication2.lss" "GccApplication2.eep" "GccApplication2.map" "GccApplication2.srec" "GccApplication2.usersignatures"
	