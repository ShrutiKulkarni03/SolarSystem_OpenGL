cls

cl.exe /c /EHsc /MDd /I "D:\SHRUTI\AstroMediComp\RTR\RTR2020\MyCodes\_Extra\00-Demos\TheMilkyWay\src\main\inc" /I "inc\freetype\include" main.cpp

rc.exe Resources.rc

link.exe main.obj Resources.res user32.lib gdi32.lib /LIBPATH:"lib" /SUBSYSTEM:WINDOWS /NODEFAULTLIB:library msvcrtd.lib /NODEFAULTLIB:library libcmt.lib 

del main.obj
del Resources.res

main


