/*
    ESFree V1.0 - Copyright (C) 2016 Robin Kase
    All rights reserved

    This file is part of ESFree.

    ESFree is free software; you can redistribute it and/or modify it under
    the terms of the GNU General Public licence (version 2) as published by the
    Free Software Foundation AND MODIFIED BY one exception.

    ***************************************************************************
    >>!   NOTE: The modification to the GPL is included to allow you to     !<<
    >>!   distribute a combined work that includes ESFree without being     !<<
    >>!   obliged to provide the source code for proprietary components     !<<
    >>!   outside of ESFree.                                                !<<
    ***************************************************************************

    ESFree is distributed in the hope that it will be useful, but WITHOUT ANY
    WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
    FOR A PARTICULAR PURPOSE. Full license text can be found on license.txt.
*/

#ifndef SCHEDULER_H_
#define SCHEDULER_H_

/* FreeRTOS includes. */
#include "FreeRTOS.h"
#include "task.h"

/* The scheduling policy can be chosen from one of these. */
#define schedSCHEDULING_POLICY_MANUAL 0	 	/* The priority of tasks are set by the user. */
#define schedSCHEDULING_POLICY_RMS 1 		/* Rate-monotonic scheduling */
#define schedSCHEDULING_POLICY_DMS 2		/* Deadline-monotonic scheduling */
#define schedSCHEDULING_POLICY_EDF 3 		/* Earliest deadline first */

/* Configure scheduling policy by setting this define to the appropriate one. */
#define schedSCHEDULING_POLICY schedSCHEDULING_POLICY_EDF

/* If the scheduling policy is EDF, the implementation can be chosen between
 * naive implementation or efficient implementation.
 *
 * Naive implementation: Large overhead during context switch.
 * Efficient implementation: Lower overhead during context switch, however trace macros needs to be configured. */
#if( schedSCHEDULING_POLICY == schedSCHEDULING_POLICY_EDF)
	/* One of these defines must be set to 1, and the other one must be set to 0. */
	#define schedEDF_NAIVE 1 			/* Naive EDF implementation. */
	#define schedEDF_EFFICIENT 0 		/* Efficient EDF implementation. */
#endif

/* Maximum number of periodic tasks that can be created. (Scheduler task is
 * not included, but Polling Server is included) */
#define schedMAX_NUMBER_OF_PERIODIC_TASKS 10

/* Set this define to 1 to enable aperiodic jobs. */
#define schedUSE_APERIODIC_JOBS 1
/* Set this define to 1 to enable sporadic jobs. */
#define schedUSE_SPORADIC_JOBS 1

#if( schedUSE_APERIODIC_JOBS == 1 || schedUSE_SPORADIC_JOBS == 1 )
	/* Enable Polling Server. */
	#define schedUSE_POLLING_SERVER 1
#else
	/* Disable Polling Server. */
	#define schedUSE_POLLING_SERVER 0
#endif /* schedUSE_APERIODIC_JOBS || schedUSE_SPORADIC_JOBS */

#if ( schedUSE_APERIODIC_JOBS == 1 )
	/* Maximum number of aperiodic jobs. */
	#define schedMAX_NUMBER_OF_APERIODIC_JOBS 4
#endif /* schedUSE_APERIODIC_JOBS */

#if ( schedUSE_SPORADIC_JOBS == 1 )
	/* Maximum number of sporadic jobs. */
	#define schedMAX_NUMBER_OF_SPORADIC_JOBS 4
#endif /* schedUSE_SPORADIC_JOBS */

/* Set this define to 1 to enable Timing-Error-Detection for detecting tasks
 * that have missed their deadlines. Tasks that have missed their deadlines
 * will be deleted, recreated and restarted during next period. */
#define schedUSE_TIMING_ERROR_DETECTION_DEADLINE 1

/* Set this define to 1 to enable Timing-Error-Detection for detecting tasks
 * that have exceeded their worst-case execution time. Tasks that have exceeded
 * their worst-case execution time will be preempted until next period. */
#define schedUSE_TIMING_ERROR_DETECTION_EXECUTION_TIME 1

#if( schedUSE_POLLING_SERVER == 1 || schedSCHEDULING_POLICY == schedSCHEDULING_POLICY_EDF || schedUSE_TIMING_ERROR_DETECTION_DEADLINE == 1 || schedMAX_NUMBER_OF_SPORADIC_JOBS == 1)
	/* Set this define to 1 to enable the scheduler task. This define must be set to 1
 	 * when using following features:
 	 * EDF scheduling policy, Timing-Error-Detection of execution time,
 	 * Timing-Error-Detection of deadline, Polling Server. */
	#define schedUSE_SCHEDULER_TASK 1
#else
	#define schedUSE_SCHEDULER_TASK 0
#endif

#if( schedUSE_SCHEDULER_TASK == 1 )
	/* Priority of the scheduler task. */
	#define schedSCHEDULER_PRIORITY ( configMAX_PRIORITIES - 1 )
	/* Stack size of the scheduler task. */
	#define schedSCHEDULER_TASK_STACK_SIZE 1000
	/* The period of the scheduler task in software ticks. */
	#define schedSCHEDULER_TASK_PERIOD pdMS_TO_TICKS( 100 )
	
	/* This define needs to be configured port specifically. For some ports
	 * it is portYIELD_FROM_ISR and for others it is portEND_SWITCHING_ISR. */
	#define schedYIELD_FROM_ISR( xSwitchingRequired ) portEND_SWITCHING_ISR( xSwitchingRequired )
#endif /* schedUSE_SCHEDULER_TASK */

#if( schedUSE_POLLING_SERVER == 1 )
	/* The period of the Polling Server. */
	#define schedPOLLING_SERVER_PERIOD pdMS_TO_TICKS( 600 )
	/* Deadline of Polling Server will only be used for setting priority if
	 * scheduling policy is DMS or EDF. Polling Server will not be preempted
	 * when exceeding deadline if Timing-Error-Detection for deadline is
	 * enabled. */
	#define schedPOLLING_SERVER_DEADLINE pdMS_TO_TICKS( 600 )
	/* Stack size of the Polling Server. */
	#define schedPOLLING_SERVER_STACK_SIZE 2000
	/* Execution budget of the Polling Server. */
	#define schedPOLLING_SERVER_MAX_EXECUTION_TIME pdMS_TO_TICKS( 100 )
	#if( schedSCHEDULING_POLICY == schedSCHEDULING_POLICY_MANUAL )
		/* Priority of the Polling Server if scheduling policy is manual. */
		#define schedPOLLING_SERVER_PRIORITY 5
	#endif /* schedSCHEDULING_POLICY_MANUAL */
#endif /* schedUSE_POLLING_SERVER */

/* Functions that must be defined by trace macros if efficient EDF is used. */
#if( schedEDF_EFFICIENT == 1 )
	/* traceTASK_DELAY_UNTIL must define this function.  */
	void vSchedulerSuspendTrace( TaskHandle_t xTaskHandle );
	/* traceBLOCKING_ON_QUEUE_RECEIVE, traceBLOCKING_ON_QUEUE_SEND and
	 * traceTASK_DELAY_UNTIL must define this function. */
	void vSchedulerBlockTrace( void );
	/* traceTASK_SWITCHED_IN must define this function. */
	void vSchedulerReadyTrace( TaskHandle_t xTaskHandle );
#endif /* schedEDF_EFFICIENT */

/* This function must be called before any other function call from scheduler.h. */
void vSchedulerInit( void );

/* Creates a periodic task.
 *
 * pvTaskCode: The task function.
 * pcName: Name of the task.
 * usStackDepth: Stack size of the task in words, not bytes.
 * pvParameters: Parameters to the task function.
 * uxPriority: Priority of the task. (Only used when scheduling policy is set to manual)
 * pxCreatedTask: Pointer to the task handle.
 * xPhaseTick: Phase given in software ticks. Counted from when vSchedulerStart is called.
 * xPeriodTick: Period given in software ticks.
 * xMaxExecTimeTick: Worst-case execution time given in software ticks.
 * xDeadlineTick: Relative deadline given in software ticks.
 * */
void vSchedulerPeriodicTaskCreate( TaskFunction_t pvTaskCode, const char *pcName, UBaseType_t uxStackDepth, void *pvParameters, UBaseType_t uxPriority,
		TaskHandle_t *pxCreatedTask, TickType_t xPhaseTick, TickType_t xPeriodTick, TickType_t xMaxExecTimeTick, TickType_t xDeadlineTick );

/* Deletes a periodic task associated with the given task handle. */
void vSchedulerPeriodicTaskDelete( TaskHandle_t xTaskHandle );

/* Starts scheduling tasks. */
void vSchedulerStart( void );

#if ( schedUSE_APERIODIC_JOBS == 1 )
	/* Creates an aperiodic job.
	 *
	 * pvTaskCode: The job function.
	 * pcName: Name of the job.
	 * pvParameters: Parameters to the job function.
	 * xMaxExecTimeTick: Worst-case execution time given in software ticks.
	 * */
	void vSchedulerAperiodicJobCreate( TaskFunction_t pvTaskCode, const char *pcName, void *pvParameters, TickType_t xMaxExecTimeTick );
#endif /* schedUSE_APERIODIC_JOBS */

#if ( schedUSE_SPORADIC_JOBS == 1 )
	/* Creates a sporadic job.
	 *
	 * pvTaskCode: The job function.
	 * pcName: Name of the job.
	 * pvParameters: Parameters to the job function.
	 * xMaxExecTimeTick: Worst-case execution time given in software ticks.
	 * xDeadlineTick: Relative deadline given in software ticks.
	 * */
	BaseType_t xSchedulerSporadicJobCreate( TaskFunction_t pvTaskCode, const char *pcName, void *pvParameters, TickType_t xMaxExecTimeTick, TickType_t xDeadlineTick );
#endif /* schedUSE_SPORADIC_JOBS */

#endif /* SCHEDULER_H_ */
