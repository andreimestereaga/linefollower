Line follower project

How to:
1. 	Using mount_q to mount your project root to Q drive. This drive is using by Diptrace.
	There are some library that use hard paths instead of relative paths, so this is the
	reason of using that batch file. The reset_q batch file is used to remove Q drive.
2. 	Hardware directory contains all files and diptrace projects necesary for hardware
	development. At this level, the line follower project was splitted in to parts:
	A) line follower: contains two diptrace files, with the following extensions:
	sch and dch. The line follower robot main hardware arhitecture is defined in 
	line_follower_schematic.sch file and its design is defined in line_follower_layout.dch
	B) Sensors: contains two diptrace files too, sensors.sch used for hardware arhictecture 
	definitions and sensors.dch used for sensors bar design.
3.	The hardware\common directory contains all diptrace library that are used in this project
	To use these correctly, you have to set the absolute paths from Library setup in both, sch
	and dch files. It is recomandded to use Q drive to perform this action. In this way, every
	modification at diptrace library is saved, otherwise all modification will be lost. 
4.	The hardware\resources directory contains other files like pictures or .txt files, which
	have project related information.
	