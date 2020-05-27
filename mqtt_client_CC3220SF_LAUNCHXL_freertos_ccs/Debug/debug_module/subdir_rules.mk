################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
debug_module/%.obj: ../debug_module/%.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs1000/ccs/tools/compiler/ti-cgt-arm_20.2.0.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib -me --include_path="C:/Users/Yahui/Documents/2020Spring/embedded/MS3_mqttServer/mqtt_client_CC3220SF_LAUNCHXL_freertos_ccs" --include_path="C:/Users/Yahui/Documents/2020Spring/embedded/MS3_mqttServer/mqtt_client_CC3220SF_LAUNCHXL_freertos_ccs/Debug" --include_path="C:/ti/simplelink_cc32xx_sdk_4_10_00_07/source" --include_path="C:/ti/simplelink_cc32xx_sdk_4_10_00_07/source/ti/posix/ccs" --include_path="C:/FreeRTOSv10.3.1/FreeRTOS/Source/include" --include_path="C:/FreeRTOSv10.3.1/FreeRTOS/Source/portable/CCS/ARM_CM3" --include_path="C:/Users/Yahui/Documents/2020Spring/embedded/MS3_mqttServer/freertos_builds_CC3220SF_LAUNCHXL_release_ccs" --include_path="C:/ti/ccs1000/ccs/tools/compiler/ti-cgt-arm_20.2.0.LTS/include" -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="debug_module/$(basename $(<F)).d_raw" --include_path="C:/Users/Yahui/Documents/2020Spring/embedded/MS3_mqttServer/mqtt_client_CC3220SF_LAUNCHXL_freertos_ccs/Debug/syscfg" --obj_directory="debug_module" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


