################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
targetConfigs/%.obj: ../targetConfigs/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: C2000 Compiler'
	"C:/ti/ccs1281/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/bin/cl2000" -v28 -ml -mt --cla_support=cla2 --float_support=fpu32 --tmu_support=tmu0 --vcu_support=vcu0 --include_path="C:/Users/HariharaKailasaRaman/new workspace/RNL_Tonhe/Tetra_Tower_RNLTonhe" --include_path="C:/ti/c2000/C2000Ware_5_04_00_00/libraries/flash_api/f28004x/lib" --include_path="C:/ti/c2000/C2000Ware_5_04_00_00/libraries/flash_api/f28004x/include/FlashAPI" --include_path="C:/ti/c2000/C2000Ware_5_04_00_00/libraries/flash_api/f28004x/include/FlashAPI/Constants" --include_path="C:/Users/HariharaKailasaRaman/new workspace/RNL_Tonhe/Tetra_Tower_RNLTonhe/ApplicationLayer" --include_path="C:/Users/HariharaKailasaRaman/new workspace/RNL_Tonhe/Tetra_Tower_RNLTonhe/ChargerLevel" --include_path="C:/Users/HariharaKailasaRaman/new workspace/RNL_Tonhe/Tetra_Tower_RNLTonhe/MIddleLayer" --include_path="C:/Users/HariharaKailasaRaman/new workspace/RNL_Tonhe/Tetra_Tower_RNLTonhe/MCU_HAL/include" --include_path="C:/Users/HariharaKailasaRaman/new workspace/RNL_Tonhe/Tetra_Tower_RNLTonhe/MCU_HAL/FlashAPI" --include_path="C:/Users/HariharaKailasaRaman/new workspace/RNL_Tonhe/Tetra_Tower_RNLTonhe/MCU_HAL/FlashAPI/Constants" --include_path="C:/Users/HariharaKailasaRaman/new workspace/RNL_Tonhe/Tetra_Tower_RNLTonhe/MCU_HAL/driverlib" --include_path="C:/Users/HariharaKailasaRaman/new workspace/RNL_Tonhe/Tetra_Tower_RNLTonhe/MCU_HAL/driverlib/inc" --include_path="C:/Users/HariharaKailasaRaman/new workspace/RNL_Tonhe/Tetra_Tower_RNLTonhe" --include_path="C:/ti/ccs1281/ccs/tools/compiler/ti-cgt-c2000_22.6.1.LTS/include" --define=_FLASH -g --diag_warning=225 --diag_wrap=off --display_error_number --abi=coffabi --preproc_with_compile --preproc_dependency="targetConfigs/$(basename $(<F)).d_raw" --obj_directory="targetConfigs" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


