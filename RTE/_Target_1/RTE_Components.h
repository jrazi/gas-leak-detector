
/*
 * Auto generated Run-Time-Environment Configuration File
 *      *** Do not modify ! ***
 *
 * Project: 'main' 
 * Target:  'Target 1' 
 */

#ifndef RTE_COMPONENTS_H
#define RTE_COMPONENTS_H


/*
 * Define the Device Header File: 
 */
#define CMSIS_device_header "LPC17xx.h"

/* ARM::CMSIS:RTOS2:Keil RTX5:Library:5.5.2 */
#define RTE_CMSIS_RTOS2                 /* CMSIS-RTOS2 */
        #define RTE_CMSIS_RTOS2_RTX5            /* CMSIS-RTOS2 Keil RTX5 */
/* Arm-Packs::Test:Unity:Classic:2.5.0 */
#define RTE_Test_Unity_Classic        /* Unity Classic */
/* Keil.ARM Compiler::Compiler:Event Recorder:DAP:1.4.0 */
#define RTE_Compiler_EventRecorder
          #define RTE_Compiler_EventRecorder_DAP
/* Keil.MDK-Pro::Network:CORE:IPv4 Release:7.14.0 */
#define RTE_Network_Core                /* Network Core */
          #define RTE_Network_IPv4                /* Network IPv4 Stack */
          #define RTE_Network_Release             /* Network Release Version */
/* Keil.MDK-Pro::Network:Interface:WiFi:7.14.0 */
#define RTE_Network_Interface_WiFi_0    /* Network Interface WiFi 0 */

/* Keil.MDK-Pro::Network:Socket:TCP:7.14.0 */
#define RTE_Network_Socket_TCP          /* Network Socket TCP */
/* Keil.MDK-Pro::Network:Socket:UDP:7.14.0 */
#define RTE_Network_Socket_UDP          /* Network Socket UDP */
/* Keil::CMSIS Driver:USART:2.10.0 */
#define RTE_Drivers_USART0              /* Driver USART0 */
        #define RTE_Drivers_USART1              /* Driver USART1 */
        #define RTE_Drivers_USART2              /* Driver USART2 */
        #define RTE_Drivers_USART3              /* Driver USART3 */
/* Keil::CMSIS Driver:WiFi:ESP8266:UART:1.4.0 */
#define RTE_Drivers_WiFi_ESP8266_UART   /* Driver WiFi ESP8266 (UART) */
/* Keil::Data Exchange:JSON:Jansson:2.7.0 */
#define JANSSON
/* Keil::Device:Startup:1.0.0 */
#define RTE_DEVICE_STARTUP_LPC17XX      /* Device Startup for NXP17XX */
/* MDK-Packs::IoT Utility:http-parser:2.9.2 */
#define RTE_IoT_HTTP_PARSER             /* HTTP Parser */


#endif /* RTE_COMPONENTS_H */
