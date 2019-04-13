ESFree is an open source scheduling library providing theoretical real-time concepts for FreeRTOS.
As ESFree does not modify the FreeRTOS kernel, the library should work on any platform that runs
FreeRTOS. ESFree has been tested and verified on following platforms:
* Digilent ZedBoard (Xilinx Zynq) with FreeRTOS V8.2.3
* Digilent Nexys 4 DDR (Xilinx MicroBlaze) with FreeRTOS V8.2.3
* STMicroelectronics STM32 Nucleo-F401RE (ARM Cortex-M4F) with FreeRTOS V8.2.1

ESFree is simple to use, just move scheduler.h and scheduler.c to the project folder and include
scheduler.h.

Features provided by ESFree V1.0:
* Periodic tasks with phases
* Aperiodic jobs
* Sporadic jobs
* Polling server
* RMS scheduling policy
* DMS scheduling policy
* EDF scheduling policy (the user can choose between two implementations, efficient EDF or naive EDF)
* Timing error detection and handling for worst-case execution time excess
* Timing error detection and handling for deadline miss

Documentation of ESFree is available at: (http://www.diva-portal.org/smash/get/diva2:1085303/FULLTEXT01.pdf)

