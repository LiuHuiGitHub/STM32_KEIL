SET PATH=C:\Program Files\Keil for ARM\ARM\ARMCC\bin\;C:\Program Files\NVIDIA Corporation\PhysX\Common;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Program Files\Windows7Master
SET CPU_TYPE=STM32F107VC
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=DebugInFlash
SET CPU_CLOCK=0x017D7840
"C:\Program Files\Keil for ARM\ARM\ARMCC\bin\ArmCC" --Via ".\output\main.__i"
"C:\Program Files\Keil for ARM\ARM\ARMCC\bin\ArmCC" --Via ".\output\stm32f10x_it.__i"
"C:\Program Files\Keil for ARM\ARM\ARMCC\bin\ArmCC" --Via ".\output\system.__i"
"C:\Program Files\Keil for ARM\ARM\ARMCC\bin\ArmCC" --Via ".\output\spi_flash.__i"
"C:\Program Files\Keil for ARM\ARM\ARMCC\bin\ArmCC" --Via ".\output\stm32f10x_gpio.__i"
"C:\Program Files\Keil for ARM\ARM\ARMCC\bin\ArmCC" --Via ".\output\stm32f10x_rcc.__i"
"C:\Program Files\Keil for ARM\ARM\ARMCC\bin\ArmCC" --Via ".\output\stm32f10x_exti.__i"
"C:\Program Files\Keil for ARM\ARM\ARMCC\bin\ArmCC" --Via ".\output\misc.__i"
"C:\Program Files\Keil for ARM\ARM\ARMCC\bin\ArmCC" --Via ".\output\stm32f10x_usart.__i"
"C:\Program Files\Keil for ARM\ARM\ARMCC\bin\ArmCC" --Via ".\output\stm32f10x_dma.__i"
"C:\Program Files\Keil for ARM\ARM\ARMCC\bin\ArmCC" --Via ".\output\stm32f10x_spi.__i"
"C:\Program Files\Keil for ARM\ARM\ARMCC\bin\ArmCC" --Via ".\output\core_cm3.__i"
"C:\Program Files\Keil for ARM\ARM\ARMCC\bin\ArmCC" --Via ".\output\system_stm32f10x.__i"
"C:\Program Files\Keil for ARM\ARM\ARMCC\bin\ArmAsm" --Via ".\output\startup_stm32f10x_cl._ia"
"C:\Program Files\Keil for ARM\ARM\ARMCC\bin\ArmLink" --Via ".\Output\DEMO.lnp"
C:\Program Files\Keil for ARM\ARM\ARMCC\bin\fromelf.exe ".\Output\DEMO.axf" --i32combined --output ".\Output\DEMO.hex"
