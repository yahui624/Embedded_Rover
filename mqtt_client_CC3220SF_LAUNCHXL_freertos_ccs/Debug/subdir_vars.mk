################################################################################
# Automatically-generated file. Do not edit!
################################################################################

SHELL = cmd.exe

# Add inputs and outputs from these tool invocations to the build variables 
CMD_SRCS += \
../CC3220SF_LAUNCHXL_FREERTOS.cmd 

SYSCFG_SRCS += \
../common.syscfg 

C_SRCS += \
./syscfg/ti_net_config.c \
./syscfg/ti_drivers_net_wifi_config.c \
./syscfg/ti_drivers_config.c \
../main_freertos.c 

GEN_FILES += \
./syscfg/ti_net_config.c \
./syscfg/ti_drivers_net_wifi_config.c \
./syscfg/ti_drivers_config.c 

GEN_MISC_DIRS += \
./syscfg/ 

C_DEPS += \
./syscfg/ti_net_config.d \
./syscfg/ti_drivers_net_wifi_config.d \
./syscfg/ti_drivers_config.d \
./main_freertos.d 

OBJS += \
./syscfg/ti_net_config.obj \
./syscfg/ti_drivers_net_wifi_config.obj \
./syscfg/ti_drivers_config.obj \
./main_freertos.obj 

GEN_MISC_FILES += \
./syscfg/ti_drivers_config.h \
./syscfg/syscfg_c.rov.xs 

GEN_MISC_DIRS__QUOTED += \
"syscfg\" 

OBJS__QUOTED += \
"syscfg\ti_net_config.obj" \
"syscfg\ti_drivers_net_wifi_config.obj" \
"syscfg\ti_drivers_config.obj" \
"main_freertos.obj" 

GEN_MISC_FILES__QUOTED += \
"syscfg\ti_drivers_config.h" \
"syscfg\syscfg_c.rov.xs" 

C_DEPS__QUOTED += \
"syscfg\ti_net_config.d" \
"syscfg\ti_drivers_net_wifi_config.d" \
"syscfg\ti_drivers_config.d" \
"main_freertos.d" 

GEN_FILES__QUOTED += \
"syscfg\ti_net_config.c" \
"syscfg\ti_drivers_net_wifi_config.c" \
"syscfg\ti_drivers_config.c" 

SYSCFG_SRCS__QUOTED += \
"../common.syscfg" 

C_SRCS__QUOTED += \
"./syscfg/ti_net_config.c" \
"./syscfg/ti_drivers_net_wifi_config.c" \
"./syscfg/ti_drivers_config.c" \
"../main_freertos.c" 


