OBJS_BOOTPACK = bootpack.obj naskfunc.obj Font/hankaku.obj Graphic/graphic.obj Dsctbl/dsctbl.obj Int/int.obj Keyboard/Keyboard.obj FIFO/FIFO.obj \
	memory/memory.obj sheet/sheet.obj timer/timer.obj task/task.obj command/command.obj file/file.obj
TOOLPATH = ../z_tools/
INCPATH  = ../z_tools/haribote/

MAKE     = $(TOOLPATH)make.exe -r
NASK     = $(TOOLPATH)nask.exe
CC1      = $(TOOLPATH)cc1.exe -I$(INCPATH) -Os -Wall -quiet
GAS2NASK = $(TOOLPATH)gas2nask.exe -a
OBJ2BIM  = $(TOOLPATH)obj2bim.exe
MAKEFONT = $(TOOLPATH)makefont.exe
BIN2OBJ  = $(TOOLPATH)bin2obj.exe
BIM2HRB  = $(TOOLPATH)bim2hrb.exe
RULEFILE = $(TOOLPATH)haribote/haribote.rul
EDIMG    = $(TOOLPATH)edimg.exe
IMGTOL   = $(TOOLPATH)imgtol.com
COPY     = copy
DEL      = del

# �f�t�H���g����

default :
	$(MAKE) img

# �t�@�C�������K��

ipl10.bin : ipl10.nas Makefile
	$(NASK) ipl10.nas ipl10.bin ipl10.lst

asmhead.bin : asmhead.nas Makefile
	$(NASK) asmhead.nas asmhead.bin asmhead.lst

#%.gas : %/%.c Makefile
#	$(CC1) -o $(*/*).gas $(*/*).c

#%.nas : %.gas Makefile
#	$(GAS2NASK) $*.gas $*.nas

#%.obj: %.nas Makefile
#	$(NASK) $*.nas $*.obj $*.lst

Keyboard/Keyboard.gas: Keyboard/Keyboard.c Makefile
	$(CC1) -o Keyboard/Keyboard.gas Keyboard/Keyboard.c

Keyboard/Keyboard.nas : Keyboard/Keyboard.gas Makefile
	$(GAS2NASK) Keyboard/Keyboard.gas Keyboard/Keyboard.nas

Keyboard/Keyboard.obj: Keyboard/Keyboard.nas Makefile
	$(NASK) Keyboard/Keyboard.nas Keyboard/Keyboard.obj Keyboard/Keyboard.lst


Graphic/graphic.gas: Graphic/graphic.c Makefile
	$(CC1) -o Graphic/graphic.gas Graphic/graphic.c

Graphic/graphic.nas : Graphic/graphic.gas Makefile
	$(GAS2NASK) Graphic/graphic.gas Graphic/graphic.nas

Graphic/graphic.obj: Graphic/graphic.nas Makefile
	$(NASK) Graphic/graphic.nas Graphic/graphic.obj Graphic/graphic.lst


Dsctbl/dsctbl.gas: Dsctbl/dsctbl.c Makefile
	$(CC1) -o Dsctbl/dsctbl.gas Dsctbl/dsctbl.c

Dsctbl/dsctbl.nas : Dsctbl/dsctbl.gas Makefile
	$(GAS2NASK) Dsctbl/dsctbl.gas Dsctbl/dsctbl.nas

Dsctbl/dsctbl.obj: Dsctbl/dsctbl.nas Makefile
	$(NASK) Dsctbl/dsctbl.nas Dsctbl/dsctbl.obj Dsctbl/dsctbl.lst


Int/int.gas: Int/int.c Makefile
	$(CC1) -o Int/int.gas Int/int.c

Int/int.nas : Int/int.gas Makefile
	$(GAS2NASK) Int/int.gas Int/int.nas

Int/int.obj: Int/int.nas Makefile
	$(NASK) Int/int.nas Int/int.obj Int/int.lst



FIFO/FIFO.gas: FIFO/FIFO.c Makefile
	$(CC1) -o FIFO/FIFO.gas FIFO/FIFO.c

FIFO/FIFO.nas : FIFO/FIFO.gas Makefile
	$(GAS2NASK) FIFO/FIFO.gas FIFO/FIFO.nas

FIFO/FIFO.obj: FIFO/FIFO.nas Makefile
	$(NASK) FIFO/FIFO.nas FIFO/FIFO.obj FIFO/FIFO.lst


memory/memory.gas: memory/memory.c Makefile
	$(CC1) -o memory/memory.gas memory/memory.c

memory/memory.nas : memory/memory.gas Makefile
	$(GAS2NASK) memory/memory.gas memory/memory.nas

memory/memory.obj: memory/memory.nas Makefile
	$(NASK) memory/memory.nas memory/memory.obj memory/memory.lst


#sheet
sheet/sheet.gas: sheet/sheet.c Makefile
	$(CC1) -o sheet/sheet.gas sheet/sheet.c

sheet/sheet.nas : sheet/sheet.gas Makefile
	$(GAS2NASK) sheet/sheet.gas sheet/sheet.nas

sheet/sheet.obj: sheet/sheet.nas Makefile
	$(NASK) sheet/sheet.nas sheet/sheet.obj sheet/sheet.lst

#timer
timer/timer.gas: timer/timer.c Makefile
	$(CC1) -o timer/timer.gas timer/timer.c

timer/timer.nas : timer/timer.gas Makefile
	$(GAS2NASK) timer/timer.gas timer/timer.nas

timer/timer.obj: timer/timer.nas Makefile
	$(NASK) timer/timer.nas timer/timer.obj timer/timer.lst

#command
command/command.gas: command/command.c Makefile
	$(CC1) -o command/command.gas command/command.c

command/command.nas : command/command.gas Makefile
	$(GAS2NASK) command/command.gas command/command.nas

command/command.obj: command/command.nas Makefile
	$(NASK) command/command.nas command/command.obj command/command.lst

#task
task/task.gas: task/task.c Makefile
	$(CC1) -o task/task.gas task/task.c

task/task.nas : task/task.gas Makefile
	$(GAS2NASK) task/task.gas task/task.nas

task/task.obj: task/task.nas Makefile
	$(NASK) task/task.nas task/task.obj task/task.lst


#file
file/file.gas: file/file.c Makefile
	$(CC1) -o file/file.gas file/file.c

file/file.nas : file/file.gas Makefile
	$(GAS2NASK) file/file.gas file/file.nas

file/file.obj: file/file.nas Makefile
	$(NASK) file/file.nas file/file.obj file/file.lst



Font/hankaku.bin : Font/hankaku.txt Makefile
	$(MAKEFONT) Font/hankaku.txt Font/hankaku.bin

Font/hankaku.obj : Font/hankaku.bin Makefile
	$(BIN2OBJ) Font/hankaku.bin Font/hankaku.obj _hankaku


bootpack.gas: bootpack.c Makefile
	$(CC1) -o bootpack.gas bootpack.c

bootpack.nas : bootpack.gas Makefile
	$(GAS2NASK) bootpack.gas bootpack.nas

bootpack.obj: bootpack.nas Makefile
	$(NASK) bootpack.nas bootpack.obj bootpack.lst

naskfunc.obj: naskfunc.nas Makefile
	$(NASK) naskfunc.nas naskfunc.obj naskfunc.lst


bootpack.bim : $(OBJS_BOOTPACK) Makefile
	$(OBJ2BIM) @$(RULEFILE) out:bootpack.bim stack:3136k map:bootpack.map \
		$(OBJS_BOOTPACK)
# 3MB+64KB=3136KB

bootpack.hrb : bootpack.bim Makefile
	$(BIM2HRB) bootpack.bim bootpack.hrb 0



alpha.sys : asmhead.bin bootpack.hrb Makefile
	copy /B asmhead.bin+bootpack.hrb alpha.sys

hello.nxe:
	$(MAKE) -C APP hello/hello.nxe

a.exe:
	$(MAKE) -C APP a/a.exe
	
hello3.exe:
	$(MAKE) -C APP hello3/hello3.exe

crack1.exe:
	$(MAKE) -C APP crack1/crack1.exe

crack2.nxe:
	$(MAKE) -C APP crack2/crack2.nxe

bug.exe:
	$(MAKE) -C APP bug/bug.exe

hello4.exe:
	$(MAKE) -C APP hello4/hello4.exe_s

winhelo.exe:
	$(MAKE) -C APP winhelo/winhelo.exe

winhelo2.exe:
	$(MAKE) -C APP winhelo2/winhelo2.exe

alpha.img : ipl10.bin alpha.sys Makefile hello.nxe a.exe hello3.exe crack1.exe \
			crack2.nxe bug.exe hello4.exe winhelo.exe winhelo2.exe
	$(EDIMG)   imgin:../z_tools/fdimg0at.tek \
		wbinimg src:ipl10.bin len:512 from:0 to:0 \
		copy from:alpha.sys to:@: \
		copy from:ipl10.nas to:@: \
		copy from:make.bat to:@: \
		copy from:App/hello/hello.exe to:@: \
		copy from:App/a/a.exe to:@: \
		copy from:App/hello3/hello3.exe to:@: \
		copy from:App/crack1/crack1.exe to:@: \
		copy from:App/crack2/crack2.exe to:@: \
		copy from:App/bug/bug.exe to:@: \
		copy from:App/hello4/hello4.exe to:@: \
		copy from:App/winhelo/winhelo.exe to:@: \
		copy from:App/winhelo2/winhelo2.exe to:@: \
		imgout:alpha.img

# �R�}���h

img :
	$(MAKE) alpha.img

run :
	$(MAKE) img
	$(COPY) alpha.img ..\z_tools\qemu\fdimage0.bin
	$(MAKE) -C ../z_tools/qemu

install :
	$(MAKE) img
	$(IMGTOL) w a: alpha.img

clean :
	-$(DEL) *.bin
	-$(DEL) *.lst
	-$(DEL) *.gas
	-$(DEL) *.obj
	-$(DEL) bootpack.nas
	-$(DEL) bootpack.map
	-$(DEL) bootpack.bim
	-$(DEL) bootpack.hrb
	-$(DEL) alpha.sys
	$(MAKE) del_timer
	$(MAKE) del_sheet
	$(MAKE) del_memory
	$(MAKE) del_FIFO
	$(MAKE) del_int
	$(MAKE) del_dsctbl
	$(MAKE) del_graphic
	$(MAKE) del_Keyboard
	$(MAKE) del_task
	$(MAKE) del_command
	$(MAKE) del_file

del_timer:
	-$(DEL) timer\timer.obj
	-$(DEL) timer\timer.nas
	-$(DEL) timer\timer.gas
	-$(DEL) timer\timer.lst

del_sheet:
	-$(DEL) sheet\sheet.obj
	-$(DEL) sheet\sheet.nas
	-$(DEL) sheet\sheet.gas
	-$(DEL) sheet\sheet.lst
del_memory:
	-$(DEL) memory\memory.obj
	-$(DEL) memory\memory.nas
	-$(DEL) memory\memory.gas
	-$(DEL) memory\memory.lst
del_FIFO:
	-$(DEL) FIFO\FIFO.obj
	-$(DEL) FIFO\FIFO.nas
	-$(DEL) FIFO\FIFO.gas
	-$(DEL) FIFO\FIFO.lst
del_int:
	-$(DEL) Int\int.obj
	-$(DEL) Int\int.nas
	-$(DEL) Int\int.gas
	-$(DEL) Int\int.lst
del_dsctbl:
	-$(DEL) Dsctbl\dsctbl.obj
	-$(DEL) Dsctbl\dsctbl.nas
	-$(DEL) Dsctbl\dsctbl.gas
	-$(DEL) Dsctbl\dsctbl.lst
del_graphic:
	-$(DEL) Graphic\graphic.obj
	-$(DEL) Graphic\graphic.nas
	-$(DEL) Graphic\graphic.gas
	-$(DEL) Graphic\graphic.lst
del_Keyboard:
	-$(DEL) Keyboard\Keyboard.obj
	-$(DEL) Keyboard\Keyboard.nas
	-$(DEL) Keyboard\Keyboard.gas
	-$(DEL) Keyboard\Keyboard.lst

del_task:
	-$(DEL) task\task.obj
	-$(DEL) task\task.nas
	-$(DEL) task\task.gas
	-$(DEL) task\task.lst

del_command:
	-$(DEL) command\command.obj
	-$(DEL) command\command.nas
	-$(DEL) command\command.gas
	-$(DEL) command\command.lst

del_file:
	-$(DEL) file\file.obj
	-$(DEL) file\file.nas
	-$(DEL) file\file.gas
	-$(DEL) file\file.lst

src_only :
	$(MAKE) clean
	-$(DEL) alpha.img

