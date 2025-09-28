SET PATH=D:\Embedded System\Project\KeilC\ARM\ARMCLANG\Bin;C:\Windows\system32;C:\Windows;C:\Windows\System32\Wbem;C:\Windows\System32\WindowsPowerShell\v1.0\;C:\Windows\System32\OpenSSH\;C:\Program Files\Microsoft SQL Server\Client SDK\ODBC\110\Tools\Binn\;C:\Program Files (x86)\Microsoft SQL Server\120\Tools\Binn\;C:\Program Files\Microsoft SQL Server\120\Tools\Binn\;C:\Program Files\Microsoft SQL Server\120\DTS\Binn\;C:\Program Files (x86)\Windows Kits\8.1\Windows Performance Toolkit\;C:\Program Files (x86)\STMicroelectronics\STM32 ST-LINK Utility\ST-LINK Utility;C:\Users\Admin\AppData\Local\Programs\Python\Python313;C:\Users\Admin\AppData\Local\Programs\Python\Python313\Scripts;C:\Users\Admin\AppData\Local\Programs\Python\Launcher\;C:\Users\Admin\AppData\Local\Microsoft\WindowsApps;C:\FE\HTML\Microsoft VS Code\bin;C:\Users\Admin\Desktop\Nhat\ktmt\BTL;C:\Users\Admin\Desktop\Nhat\ctdlgt\mingw64\bin;C:\FE\HTML\Microsoft VS Code\MinGW\mingw64\bin;D:\ModelSim\modelsim_ase\win32aloem;C:\Users\Admin\AppData\Local\Programs\Python\Python313;C:\Users\Admin\AppData\Local\Programs\Python\Python313\Scripts;;D:\ModelSim(works)\modelsim_ase\win32aloem;C:\intelFPGA\17.1\modelsim_ase\win32aloem;C:\intelFPGA\20.1\modelsim_ase\win32aloem
SET CPU_TYPE=STM32F103C8
SET CPU_VENDOR=STMicroelectronics
SET UV2_TARGET=Target 1
SET CPU_CLOCK=0x00B71B00
"D:\Embedded System\Project\KeilC\ARM\ARMCLANG\Bin\ArmClang.exe" @".\objects\main.__i"
"D:\Embedded System\Project\KeilC\ARM\ARMCLANG\Bin\ArmClang.exe" @".\objects\startup_stm32f10x_md._ac"
"D:\Embedded System\Project\KeilC\ARM\ARMCLANG\Bin\ArmClang.exe" @".\objects\system_stm32f10x.__i"
"D:\Embedded System\Project\KeilC\ARM\ARMCLANG\Bin\ArmLink" --Via ".\Objects\B7_ADC.lnp"
"D:\Embedded System\Project\KeilC\ARM\ARMCLANG\Bin\fromelf.exe" ".\Objects\B7_ADC.axf" --i32combined --output ".\Objects\B7_ADC.hex"
