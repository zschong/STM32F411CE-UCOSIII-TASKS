# STM32F411CE-UCOSIII-TASKS
stm32f411ce uc/os tasks
# 移植uC/OS-III 修改 startup_stm32f411xe.s

```s
                DCD     OS_CPU_PendSVHandler             ; PendSV Handler
                DCD     OS_CPU_SysTickHandler            ; SysTick Handler
```

```s
        IMPORT  __main

                IF {FPU} != "SoftVFP"
                                                ; Enable Floating Point Support at reset for FPU
                LDR.W   R0, =0xE000ED88         ; Load address of CPACR register
                LDR     R1, [R0]                ; Read value at CPACR
                ORR     R1,  R1, #(0xF <<20)    ; Set bits 20-23 to enable CP10 and CP11 coprocessors
                                                ; Write back the modified CPACR value
                STR     R1, [R0]                ; Wait for store to complete
                DSB
                
                                                ; Disable automatic FP register content
                                                ; Disable lazy context switch
                LDR.W   R0, =0xE000EF34         ; Load address to FPCCR register
                LDR     R1, [R0]
                AND     R1,  R1, #(0x3FFFFFFF)  ; Clear the LSPEN and ASPEN bits
                STR     R1, [R0]
                ISB                             ; Reset pipeline now the FPU is enabled
                ENDIF

```

```s

OS_CPU_PendSVHandler  PROC
                EXPORT  OS_CPU_PendSVHandler             [WEAK]
                B       .
                ENDP
OS_CPU_SysTickHandler PROC
                EXPORT  OS_CPU_SysTickHandler            [WEAK]
                B       .
                ENDP

```
