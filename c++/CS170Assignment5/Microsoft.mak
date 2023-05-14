#*******************************************************************************
#FileName: makefile.microsoft
#author:   Matt Casanova
#e-mail:   mcasanov@digipen.edu
#Course:   CS170 - Assignment 3
#
#Brief:
#This is a microsoft makefile for Assignment 3 of cs170
#*******************************************************************************

CC=cl
CFLAGS=-W4 -EHa -WX -Za -c
OUTDIR=Microsoft/
OBJECTS=$(OUTDIR)Driver.obj $(OUTDIR)List.obj
OBJECTS2=$(OUTDIR)StressDriver.obj $(OUTDIR)List.obj
EXE=$(OUTDIR)List.exe
EXE2=$(OUTDIR)StressList.exe

#Targets =======================================================================
default: $(EXE) $(EXE2)

#Executables
$(EXE): $(OBJECTS)
	$(CC) -Fe$@ $(OBJECTS) 
	
$(EXE2): $(OBJECTS2)
	$(CC) -Fe$@ $(OBJECTS2)

#ObjectFiles
$(OUTDIR)Driver.obj: Driver.cpp List.h
	$(CC) $(CFLAGS) -Fo$@ Driver.cpp 
	
$(OUTDIR)StressDriver.obj: StressDriver.cpp List.h
	$(CC) $(CFLAGS) -Fo$@ StressDriver.cpp
	
$(OUTDIR)List.obj: List.cpp List.h 
	$(CC) $(CFLAGS) -Fo$@ List.cpp 
	
clean:
	rm $(OUTDIR)*.obj $(OUTDIR)*.exe $(OUTDIR)*.txt