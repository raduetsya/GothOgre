SET SEVENZIP=C:\Program Files\7-Zip
SET PDATE=C:\Program Files\pDate

"%PDATE%\pDate.exe" "\s\e\t \D\M\=b-d-Y" > tmp.bat
call tmp.bat
del tmp.bat
SET FILENAME=archives\GothOgre-source-code-%DM%.zip

"%SEVENZIP%\7z.exe" a %FILENAME% -r -tzip InsideGothOgre GothOgreResources GothOgreInfo Debug Release -x!.svn -x!*.ncb -x!*.suo -x!*.user -x!*.db -x!*.zip -x!*.7z -x!*.exe -x!*.dll -x!*.cfg -x!*.log -x!*.obj -x!*.pdb -x!*.pch -x!*.ilk -x!*.lib -x!*.bsc -x!*.sbr -x!*.exp -x!*.idb -pshadowbeast