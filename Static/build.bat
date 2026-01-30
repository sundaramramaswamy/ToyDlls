cl /c /Iinc src\add.cpp src\sub.cpp

lib /OUT:StaticMath.lib add.obj sub.obj

cl /c /Iinc /EHsc example\main.cpp

link /OUT:Example.exe main.obj StaticMath.lib
