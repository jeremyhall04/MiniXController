REM delete intermediate files from project
attrib -h *.suo /s
del .\*.ncb /f /s /q
del .\*.suo /f /s /q
del .\*.aps /f /s /q
del .\*.user /f /s /q
REM del .\*.manifest /f /s /q
del .\*.res /f /s /q
del .\*.dep /f /s /q
del .\BuildLog.* /f /s /q
del .\*.lastbuildstate /f /s /q
del .\*.sdf /f /s /q
del .\*.log /f /s /q
del .\*.ipch /f /s /q
del .\*_manifest.rc /f /s /q
