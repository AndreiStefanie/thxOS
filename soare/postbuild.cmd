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

"C:\Program Files (x86)\VMware\VMware Virtual Disk Development Kit\bin\vmware-mount.exe" y: C:\Users\Andrei\Desktop\SOARE\soare2.vmdk
copy %2 y:\soare.bin
"C:\Program Files (x86)\VMware\VMware Virtual Disk Development Kit\bin\vmware-mount.exe" y: /d
