@echo off
::
:: must be called as postbuild.cmd <solutiondir> <targetbin>
::

rem :: Needed only if you use the custom VM -> delete all "rem"

rem set target_mac=00.50.56.2C.D5.76

rem echo POSTBUILD: copying %2 to c:\VM\tftpfolder\%target_mac%.bin
rem copy %2 c:\VM\tftpfolder\%target_mac%.bin

echo POSTBUILD: will insert %2 into SOARE2-FLAT.VMDK...
echo IMPORTANT: if the Disk is running under VMWARE, you MUST stop it to succeed!

"c:\Program Files (x86)\VMware\VMware Virtual Disk Development Kit\bin\vmware-mount.exe" z: C:\Projects\SISC\1_Year\Security_X86-64\SOARE\soare2.vmdk
copy %2 z:\soare.bin
"c:\Program Files (x86)\VMware\VMware Virtual Disk Development Kit\bin\vmware-mount.exe" z: /d
