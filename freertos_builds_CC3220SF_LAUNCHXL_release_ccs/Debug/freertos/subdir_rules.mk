################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Each subdirectory must supply rules for building sources it contributes
freertos/croutine.obj: C:/FreeRTOSv10.3.1/FreeRTOS/Source/croutine.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs1000/ccs/tools/compiler/ti-cgt-arm_20.2.0.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib -me --include_path="C:/Users/Yahui/Documents/2020Spring/embedded/MS3_mqttServer/freertos_builds_CC3220SF_LAUNCHXL_release_ccs" --include_path="C:/FreeRTOSv10.3.1/FreeRTOS/Source/include" --include_path="C:/FreeRTOSv10.3.1/FreeRTOS/Source/portable/CCS/ARM_CM3" --include_path="C:/ti/simplelink_cc32xx_sdk_4_10_00_07/source/ti/posix/ccs" --include_path="C:/ti/simplelink_cc32xx_sdk_4_10_00_07/source" --include_path="C:/ti/ccs1000/ccs/tools/compiler/ti-cgt-arm_20.2.0.LTS/include" --define=ccs -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="freertos/$(basename $(<F)).d_raw" --obj_directory="freertos" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

freertos/event_groups.obj: C:/FreeRTOSv10.3.1/FreeRTOS/Source/event_groups.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs1000/ccs/tools/compiler/ti-cgt-arm_20.2.0.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib -me --include_path="C:/Users/Yahui/Documents/2020Spring/embedded/MS3_mqttServer/freertos_builds_CC3220SF_LAUNCHXL_release_ccs" --include_path="C:/FreeRTOSv10.3.1/FreeRTOS/Source/include" --include_path="C:/FreeRTOSv10.3.1/FreeRTOS/Source/portable/CCS/ARM_CM3" --include_path="C:/ti/simplelink_cc32xx_sdk_4_10_00_07/source/ti/posix/ccs" --include_path="C:/ti/simplelink_cc32xx_sdk_4_10_00_07/source" --include_path="C:/ti/ccs1000/ccs/tools/compiler/ti-cgt-arm_20.2.0.LTS/include" --define=ccs -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="freertos/$(basename $(<F)).d_raw" --obj_directory="freertos" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

freertos/heap_4.obj: C:/FreeRTOSv10.3.1/FreeRTOS/Source/portable/MemMang/heap_4.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs1000/ccs/tools/compiler/ti-cgt-arm_20.2.0.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib -me --include_path="C:/Users/Yahui/Documents/2020Spring/embedded/MS3_mqttServer/freertos_builds_CC3220SF_LAUNCHXL_release_ccs" --include_path="C:/FreeRTOSv10.3.1/FreeRTOS/Source/include" --include_path="C:/FreeRTOSv10.3.1/FreeRTOS/Source/portable/CCS/ARM_CM3" --include_path="C:/ti/simplelink_cc32xx_sdk_4_10_00_07/source/ti/posix/ccs" --include_path="C:/ti/simplelink_cc32xx_sdk_4_10_00_07/source" --include_path="C:/ti/ccs1000/ccs/tools/compiler/ti-cgt-arm_20.2.0.LTS/include" --define=ccs -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="freertos/$(basename $(<F)).d_raw" --obj_directory="freertos" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

freertos/list.obj: C:/FreeRTOSv10.3.1/FreeRTOS/Source/list.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs1000/ccs/tools/compiler/ti-cgt-arm_20.2.0.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib -me --include_path="C:/Users/Yahui/Documents/2020Spring/embedded/MS3_mqttServer/freertos_builds_CC3220SF_LAUNCHXL_release_ccs" --include_path="C:/FreeRTOSv10.3.1/FreeRTOS/Source/include" --include_path="C:/FreeRTOSv10.3.1/FreeRTOS/Source/portable/CCS/ARM_CM3" --include_path="C:/ti/simplelink_cc32xx_sdk_4_10_00_07/source/ti/posix/ccs" --include_path="C:/ti/simplelink_cc32xx_sdk_4_10_00_07/source" --include_path="C:/ti/ccs1000/ccs/tools/compiler/ti-cgt-arm_20.2.0.LTS/include" --define=ccs -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="freertos/$(basename $(<F)).d_raw" --obj_directory="freertos" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

freertos/queue.obj: C:/FreeRTOSv10.3.1/FreeRTOS/Source/queue.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs1000/ccs/tools/compiler/ti-cgt-arm_20.2.0.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib -me --include_path="C:/Users/Yahui/Documents/2020Spring/embedded/MS3_mqttServer/freertos_builds_CC3220SF_LAUNCHXL_release_ccs" --include_path="C:/FreeRTOSv10.3.1/FreeRTOS/Source/include" --include_path="C:/FreeRTOSv10.3.1/FreeRTOS/Source/portable/CCS/ARM_CM3" --include_path="C:/ti/simplelink_cc32xx_sdk_4_10_00_07/source/ti/posix/ccs" --include_path="C:/ti/simplelink_cc32xx_sdk_4_10_00_07/source" --include_path="C:/ti/ccs1000/ccs/tools/compiler/ti-cgt-arm_20.2.0.LTS/include" --define=ccs -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="freertos/$(basename $(<F)).d_raw" --obj_directory="freertos" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

freertos/stream_buffer.obj: C:/FreeRTOSv10.3.1/FreeRTOS/Source/stream_buffer.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs1000/ccs/tools/compiler/ti-cgt-arm_20.2.0.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib -me --include_path="C:/Users/Yahui/Documents/2020Spring/embedded/MS3_mqttServer/freertos_builds_CC3220SF_LAUNCHXL_release_ccs" --include_path="C:/FreeRTOSv10.3.1/FreeRTOS/Source/include" --include_path="C:/FreeRTOSv10.3.1/FreeRTOS/Source/portable/CCS/ARM_CM3" --include_path="C:/ti/simplelink_cc32xx_sdk_4_10_00_07/source/ti/posix/ccs" --include_path="C:/ti/simplelink_cc32xx_sdk_4_10_00_07/source" --include_path="C:/ti/ccs1000/ccs/tools/compiler/ti-cgt-arm_20.2.0.LTS/include" --define=ccs -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="freertos/$(basename $(<F)).d_raw" --obj_directory="freertos" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

freertos/tasks.obj: C:/FreeRTOSv10.3.1/FreeRTOS/Source/tasks.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs1000/ccs/tools/compiler/ti-cgt-arm_20.2.0.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib -me --include_path="C:/Users/Yahui/Documents/2020Spring/embedded/MS3_mqttServer/freertos_builds_CC3220SF_LAUNCHXL_release_ccs" --include_path="C:/FreeRTOSv10.3.1/FreeRTOS/Source/include" --include_path="C:/FreeRTOSv10.3.1/FreeRTOS/Source/portable/CCS/ARM_CM3" --include_path="C:/ti/simplelink_cc32xx_sdk_4_10_00_07/source/ti/posix/ccs" --include_path="C:/ti/simplelink_cc32xx_sdk_4_10_00_07/source" --include_path="C:/ti/ccs1000/ccs/tools/compiler/ti-cgt-arm_20.2.0.LTS/include" --define=ccs -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="freertos/$(basename $(<F)).d_raw" --obj_directory="freertos" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '

freertos/timers.obj: C:/FreeRTOSv10.3.1/FreeRTOS/Source/timers.c $(GEN_OPTS) | $(GEN_FILES) $(GEN_MISC_FILES)
	@echo 'Building file: "$<"'
	@echo 'Invoking: ARM Compiler'
	"C:/ti/ccs1000/ccs/tools/compiler/ti-cgt-arm_20.2.0.LTS/bin/armcl" -mv7M4 --code_state=16 --float_support=vfplib -me --include_path="C:/Users/Yahui/Documents/2020Spring/embedded/MS3_mqttServer/freertos_builds_CC3220SF_LAUNCHXL_release_ccs" --include_path="C:/FreeRTOSv10.3.1/FreeRTOS/Source/include" --include_path="C:/FreeRTOSv10.3.1/FreeRTOS/Source/portable/CCS/ARM_CM3" --include_path="C:/ti/simplelink_cc32xx_sdk_4_10_00_07/source/ti/posix/ccs" --include_path="C:/ti/simplelink_cc32xx_sdk_4_10_00_07/source" --include_path="C:/ti/ccs1000/ccs/tools/compiler/ti-cgt-arm_20.2.0.LTS/include" --define=ccs -g --diag_warning=225 --diag_warning=255 --diag_wrap=off --display_error_number --gen_func_subsections=on --preproc_with_compile --preproc_dependency="freertos/$(basename $(<F)).d_raw" --obj_directory="freertos" $(GEN_OPTS__FLAG) "$<"
	@echo 'Finished building: "$<"'
	@echo ' '


