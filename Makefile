CC=mipsel-unknown-elf-gcc
PSX_ROOT=/usr/local/psxsdk
LIC_FILE=${PSX_ROOT}/share/licenses/infoeur.dat
LIB=-L${PSX_ROOT}/lib
INCLUDE=-I${PSX_ROOT}/include
LDSCRIPT=-T ${PSX_ROOT}/mipsel-unknown-elf/lib/ldscripts/playstation.x
CCFLAGS= -fsigned-char -msoft-float -mno-gpopt -fno-builtin -G0 ${LIB} $(INCLUDE) ${LDSCRIPT}

all: build fight.hsf
	mkpsxiso fight.hsf fight.bin ${LIC_FILE}
fight.hsf: 
	mkisofs -o fight.hsf -V FIGHT -sysid PLAYSTATION cd_root
build: libfight.a
	${CC} ${CCFLAGS} -o fight.elf src/main.c -L. -lfight
	elf2exe fight.elf fight.exe 
	cp fight.exe cd_root
	systemcnf fight.exe > cd_root/system.cnf


	bmp2tim img/LOGO.bmp cd_root/LOGO.tim 16 -org=320,0 -noblack
	bmp2tim img/LOGO2.bmp cd_root/LOGO2.tim 16 -org=320,0 -noblack
libfight.a: libfight.o
	ar -src libfight.a libfight.o
libfight.o:
	${CC} ${CCFLAGS} -c -o libfight.o lib/fight.c
clean:
	rm -rf ping*.*
	rm -rf libfight.*
	rm -rf fight.*
	rm -rf cd_root/*
	rm -rf cd_root/system.cnf
