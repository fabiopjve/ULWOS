################################################################################
# Automatically-generated file. Do not edit!
################################################################################

# Add inputs and outputs from these tool invocations to the build variables 
ASM_SRCS += \
..\src/reset_program.asm 

C_SRCS += \
..\src/hardware_setup.c \
..\src/interrupt_handlers.c \
..\src/main.c \
..\src/ulwos.c \
..\src/vector_table.c 

C_DEPS += \
./src/hardware_setup.d \
./src/interrupt_handlers.d \
./src/main.d \
./src/ulwos.d \
./src/vector_table.d 

OBJS += \
./src/hardware_setup.o \
./src/interrupt_handlers.o \
./src/main.o \
./src/reset_program.o \
./src/ulwos.o \
./src/vector_table.o 

ASM_DEPS += \
./src/reset_program.d 


# Each subdirectory must supply rules for building sources it contributes
src/hardware_setup.o: ../src/hardware_setup.c
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@rl78-elf-gcc -MM -MP -MF "src/hardware_setup.d" -MT"src/hardware_setup.o" -MT"src/hardware_setup.d" @"src/hardware_setup.depsub" "$<"
	@echo	rl78-elf-gcc -MM -MP -MF "src/hardware_setup.d" -MT"src/hardware_setup.o" -MT"src/hardware_setup.d" -x c   -nostdinc -ftree-loop-distribute-patterns -I"C:\PROGRA~2\KPIT\GNURL7~1.02-\rl78-elf/rl78-elf/optlibinc" -Wstack-usage=64 -mg13 -O2 -g2 -g -mmul=g13 "$<"
	@rl78-elf-gcc -Wa,-adlhn="$(basename $(notdir $<)).lst" @"src/hardware_setup.sub" -o "$(@:%.d=%.o)" "$<"
	@echo rl78-elf-gcc -c -x c  -Wa,-adlhn="$(basename $(notdir $<)).lst" -nostdinc -ftree-loop-distribute-patterns -I"C:\PROGRA~2\KPIT\GNURL7~1.02-\rl78-elf/rl78-elf/optlibinc" -Wstack-usage=64 -mg13 -O2 -g2 -g -mmul=g13 -o "$(@:%.d=%.o)" "$<"
	@echo 'Finished scanning and building: $<'
	@echo.

src/interrupt_handlers.o: ../src/interrupt_handlers.c
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@rl78-elf-gcc -MM -MP -MF "src/interrupt_handlers.d" -MT"src/interrupt_handlers.o" -MT"src/interrupt_handlers.d" @"src/interrupt_handlers.depsub" "$<"
	@echo	rl78-elf-gcc -MM -MP -MF "src/interrupt_handlers.d" -MT"src/interrupt_handlers.o" -MT"src/interrupt_handlers.d" -x c   -nostdinc -ftree-loop-distribute-patterns -I"C:\PROGRA~2\KPIT\GNURL7~1.02-\rl78-elf/rl78-elf/optlibinc" -Wstack-usage=64 -mg13 -O2 -g2 -g -mmul=g13 "$<"
	@rl78-elf-gcc -Wa,-adlhn="$(basename $(notdir $<)).lst" @"src/interrupt_handlers.sub" -o "$(@:%.d=%.o)" "$<"
	@echo rl78-elf-gcc -c -x c  -Wa,-adlhn="$(basename $(notdir $<)).lst" -nostdinc -ftree-loop-distribute-patterns -I"C:\PROGRA~2\KPIT\GNURL7~1.02-\rl78-elf/rl78-elf/optlibinc" -Wstack-usage=64 -mg13 -O2 -g2 -g -mmul=g13 -o "$(@:%.d=%.o)" "$<"
	@echo 'Finished scanning and building: $<'
	@echo.

src/%.o: ../src/%.c
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@rl78-elf-gcc -MM -MP -MF ""$(@:%.o=%.d)"" -MT"$(@:%.o=%.o)" -MT""$(@:%.o=%.d)"" @"src/c.depsub" "$<"
	@echo	rl78-elf-gcc -MM -MP -MF "$(@:%.o=%.d)" -MT"$(@:%.o=%.o)" -MT"$(@:%.o=%.d)" -x c   -nostdinc -ftree-loop-distribute-patterns -I"C:\PROGRA~2\KPIT\GNURL7~1.02-\rl78-elf/rl78-elf/optlibinc" -Wstack-usage=64 -mg13 -O2 -g2 -g -mmul=g13 "$<"
	@rl78-elf-gcc -Wa,-adlhn="$(basename $(notdir $<)).lst" @"src/c.sub" -o "$(@:%.d=%.o)" "$<"
	@echo rl78-elf-gcc -c -x c  -Wa,-adlhn="$(basename $(notdir $<)).lst" -nostdinc -ftree-loop-distribute-patterns -I"C:\PROGRA~2\KPIT\GNURL7~1.02-\rl78-elf/rl78-elf/optlibinc" -Wstack-usage=64 -mg13 -O2 -g2 -g -mmul=g13 -o "$(@:%.d=%.o)" "$<"
	@echo 'Finished scanning and building: $<'
	@echo.

src/reset_program.o: ../src/reset_program.asm
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@rl78-elf-gcc -MM -MP -MF "src/reset_program.d" -MT"src/reset_program.o" -MT"src/reset_program.d" @"src/reset_program.depsub" "$<"
	@echo	rl78-elf-gcc -MM -MP -MF "src/reset_program.d" -MT"src/reset_program.o" -MT"src/reset_program.d" -Wa,-gdwarf2 -x assembler-with-cpp   -nostdinc -ftree-loop-distribute-patterns -I"C:\PROGRA~2\KPIT\GNURL7~1.02-\rl78-elf/rl78-elf/optlibinc" -Wstack-usage=64 -mg13 -O2 -g2 -g -mmul=g13 "$<"
	@rl78-elf-gcc -Wa,-adlhn="$(basename $(notdir $<)).lst" @"src/reset_program.sub" -o "$(@:%.d=%.o)" "$<"
	@echo rl78-elf-gcc -Wa,-gdwarf2 -c -x assembler-with-cpp  -Wa,-adlhn="$(basename $(notdir $<)).lst" -nostdinc -ftree-loop-distribute-patterns -I"C:\PROGRA~2\KPIT\GNURL7~1.02-\rl78-elf/rl78-elf/optlibinc" -Wstack-usage=64 -mg13 -O2 -g2 -g -mmul=g13 -o "$(@:%.d=%.o)" "$<"
	@echo 'Finished scanning and building: $<'
	@echo.

src/vector_table.o: ../src/vector_table.c
	@echo 'Scanning and building file: $<'
	@echo 'Invoking: Scanner and Compiler'
	@rl78-elf-gcc -MM -MP -MF "src/vector_table.d" -MT"src/vector_table.o" -MT"src/vector_table.d" @"src/vector_table.depsub" "$<"
	@echo	rl78-elf-gcc -MM -MP -MF "src/vector_table.d" -MT"src/vector_table.o" -MT"src/vector_table.d" -x c   -nostdinc -ftree-loop-distribute-patterns -I"C:\PROGRA~2\KPIT\GNURL7~1.02-\rl78-elf/rl78-elf/optlibinc" -Wstack-usage=64 -mg13 -O2 -g2 -g -mmul=g13 "$<"
	@rl78-elf-gcc -Wa,-adlhn="$(basename $(notdir $<)).lst" @"src/vector_table.sub" -o "$(@:%.d=%.o)" "$<"
	@echo rl78-elf-gcc -c -x c  -Wa,-adlhn="$(basename $(notdir $<)).lst" -nostdinc -ftree-loop-distribute-patterns -I"C:\PROGRA~2\KPIT\GNURL7~1.02-\rl78-elf/rl78-elf/optlibinc" -Wstack-usage=64 -mg13 -O2 -g2 -g -mmul=g13 -o "$(@:%.d=%.o)" "$<"
	@echo 'Finished scanning and building: $<'
	@echo.

