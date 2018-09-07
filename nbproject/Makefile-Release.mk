#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Environment
MKDIR=mkdir
CP=cp
GREP=grep
NM=nm
CCADMIN=CCadmin
RANLIB=ranlib
CC=arm-brcm-linux-gnueabi-gcc
CCC=arm-brcm-linux-gnueabi-g++
CXX=arm-brcm-linux-gnueabi-g++
FC=gfortran.exe
AS=as

# Macros
CND_PLATFORM=Gnueabi-Windows
CND_DLIB_EXT=dll
CND_CONF=Release
CND_DISTDIR=dist
CND_BUILDDIR=build

# Include project Makefile
include NbMakefile

# Object Directory
OBJECTDIR=${CND_BUILDDIR}/${CND_CONF}/${CND_PLATFORM}

# Object Files
OBJECTFILES= \
	${OBJECTDIR}/appmain.o \
	${OBJECTDIR}/deduct.o \
	${OBJECTDIR}/getRNinfo.o \
	${OBJECTDIR}/loading.o \
	${OBJECTDIR}/showscreen.o


# C Compiler Flags
CFLAGS="-I${SDKV5SINC}" -fsigned-char -Wundef -Wstrict-prototypes -Wno-trigraphs -Wimplicit -Wformat 

# CC Compiler Flags
CCFLAGS="-I${SDKV5SINC}" -fsigned-char -Wundef -Wno-trigraphs -Wimplicit -Wformat 
CXXFLAGS="-I${SDKV5SINC}" -fsigned-char -Wundef -Wno-trigraphs -Wimplicit -Wformat 

# Fortran Compiler Flags
FFLAGS=

# Assembler Flags
ASFLAGS=

# Link Libraries and Options
LDLIBSOPTIONS=-lcaethernet -lcafont -lcafs -lcakms -lcalcd -lcamodem -lcapmodem -lcaprt -lcartc -lcauart -lcauldpm -lcausbh -lcagsm -lcabarcode -lpthread -ldl -lcaclvw -lcatls -lctosapi -lcrypto -lcurl -lssl -lz -lfreetype

# Build Targets
.build-conf: ${BUILD_SUBPROJECTS}
	"${MAKE}"  -f nbproject/Makefile-${CND_CONF}.mk dist/V5S/App/V5S_SmartCard.exe

dist/V5S/App/V5S_SmartCard.exe: ${OBJECTFILES}
	${MKDIR} -p dist/V5S/App
	arm-brcm-linux-gnueabi-g++ -o dist/V5S/App/V5S_SmartCard ${OBJECTFILES} ${LDLIBSOPTIONS} -L . "-L${SDKV5SLIB}" "-L${SDKV5SLIBN}"

${OBJECTDIR}/appmain.o: appmain.c
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.c) -O2 -I/cygdrive/C/Program\ Files/Castles/VEGA5000S/include -I/cygdrive/C/Program\ Files\ \(x86\)/Castles/VEGA5000S/include -IOpenSSL/include -o ${OBJECTDIR}/appmain.o appmain.c

${OBJECTDIR}/deduct.o: deduct.c
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.c) -O2 -I/cygdrive/C/Program\ Files/Castles/VEGA5000S/include -I/cygdrive/C/Program\ Files\ \(x86\)/Castles/VEGA5000S/include -IOpenSSL/include -o ${OBJECTDIR}/deduct.o deduct.c

${OBJECTDIR}/getRNinfo.o: getRNinfo.c
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.c) -O2 -I/cygdrive/C/Program\ Files/Castles/VEGA5000S/include -I/cygdrive/C/Program\ Files\ \(x86\)/Castles/VEGA5000S/include -IOpenSSL/include -o ${OBJECTDIR}/getRNinfo.o getRNinfo.c

${OBJECTDIR}/loading.o: loading.c
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.c) -O2 -I/cygdrive/C/Program\ Files/Castles/VEGA5000S/include -I/cygdrive/C/Program\ Files\ \(x86\)/Castles/VEGA5000S/include -IOpenSSL/include -o ${OBJECTDIR}/loading.o loading.c

${OBJECTDIR}/showscreen.o: showscreen.c
	${MKDIR} -p ${OBJECTDIR}
	$(COMPILE.c) -O2 -I/cygdrive/C/Program\ Files/Castles/VEGA5000S/include -I/cygdrive/C/Program\ Files\ \(x86\)/Castles/VEGA5000S/include -IOpenSSL/include -o ${OBJECTDIR}/showscreen.o showscreen.c

# Subprojects
.build-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r ${CND_BUILDDIR}/${CND_CONF}

# Subprojects
.clean-subprojects:
