TOOLPATH = ../../z_tools/
INCPATH  = ../include/

MAKE     = $(TOOLPATH)make.exe -r
NASK     = $(TOOLPATH)nask.exe
CC1      = $(TOOLPATH)cc1.exe -I$(INCPATH) -Os -Wall -quiet
GAS2NASK = $(TOOLPATH)gas2nask.exe -a
OBJ2BIM  = $(TOOLPATH)obj2bim.exe
BIM2HRB  = $(TOOLPATH)bim2hrb.exe
RULEFILE = ../RulFile/haribote.rul
DEL      = del


%.gas : %.c  Makefile
	$(CC1) -o $*.gas $*.c

%.nas : %.gas Makefile
	$(GAS2NASK) $*.gas $*.nas

%.obj : %.nas Makefile
	$(NASK) $*.nas $*.obj $*.lst

%.nxe : %.nas Makefile
	$(NASK) $*.nas $*.exe $*.lst

API.obj:
	$(MAKE) -C ..\API API.obj

%.bim : %.obj API.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:$*.bim map:$*.map $*.obj ../API/API.obj

%.exe : %.bim Makefile
	$(BIM2HRB) $*.bim $*.exe 0


%.bim_s : %.obj API.obj Makefile
	$(OBJ2BIM) @$(RULEFILE) out:$*.bim stack:1k map:$*.map $*.obj ../API/API.obj

%.exe_s : %.bim_s Makefile
	$(BIM2HRB) $*.bim $*.exe 0



clean:
	$(DEL) a\a.gas
	$(DEL) a\a.nas
	$(DEL) a\a.obj
	$(DEL) a\a.lst
	$(DEL) a\a.exe

