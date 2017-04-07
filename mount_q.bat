@echo off
REM mount q drive 
set rel_path="%~dp0."
echo Mount q drive to following path: %rel_path%
subst Q: %rel_path%
pause