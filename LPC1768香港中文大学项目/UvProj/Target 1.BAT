SET PATH=C:\Keil_v5\ARM\ARMCC\Bin;C:\ProgramData\Oracle\Java\javapath;C:\Program Files (x86)\Embarcadero\RAD Studio\7.0\bin;C:\Users\Public\Documents\RAD Studio\7.0\Bpl;C:\WINDOWS\system32;C:\WINDOWS;C:\WINDOWS\System32\Wbem;C:\WINDOWS\System32\WindowsPowerShell\v1.0\;C:\Program Files\TortoiseSVN\bin;C:\Program Files (x86)\Flash Magic;C:\Program Files (x86)\Java\jdk1.8.0_112\bin;C:\JAVA\apache-maven-3.3.9\bin;C:\Program Files (x86)\ATMEL Corporation\AT91-ISP v1.13\Library\;C:\Program Files (x86)\ATMEL Corporation\AT91-ISP v1.13\sam-ba 2.9;C:\Program Files (x86)\IDM Computer Solutions\UltraEdit\;C:\Users\Administrator\AppData\Local\Microsoft\WindowsApps
SET CPU_TYPE=LPC1768
SET CPU_VENDOR=NXP
SET UV2_TARGET=Target 1
SET CPU_CLOCK=0x00B71B00
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\core_cm3.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\system_lpc17xx.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmAsm" --Via "..\startup_lpc17xx._ia"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\drv_lcd.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\drv_uart.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\drv_voice.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\lib_public.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\mid_systick.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\mid_magically.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\mid_sysconfig.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\mid_comm.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\mid_lockers.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\mid_ziku.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\app_main.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\app_comdeal.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\app_record.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\app_card.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\app_network.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\app_gui.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmCC" --Via "..\app_keyui.__i"
"C:\Keil_v5\ARM\ARMCC\Bin\ArmLink" --Via "..\ZY-SJSJG-V2.lnp"
"C:\Keil_v5\ARM\ARMCC\Bin\fromelf.exe" "..\ZY-SJSJG-V2.axf" --i32combined --output "..\ZY-SJSJG-V2.hex"
fromelf.exe --bin ".\Objects\ZY-SJSJG-V2.axf" --output "..\Objects\ZY-SJSJG-V2.bin"
