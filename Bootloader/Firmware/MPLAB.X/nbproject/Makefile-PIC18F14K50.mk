#
# Generated Makefile - do not edit!
#
# Edit the Makefile in the project folder instead (../Makefile). Each target
# has a -pre and a -post target defined where you can add customized code.
#
# This makefile implements configuration specific macros and targets.


# Include project Makefile
ifeq "${IGNORE_LOCAL}" "TRUE"
# do not include local makefile. User is passing all local related variables already
else
include Makefile
# Include makefile containing local settings
ifeq "$(wildcard nbproject/Makefile-local-PIC18F14K50.mk)" "nbproject/Makefile-local-PIC18F14K50.mk"
include nbproject/Makefile-local-PIC18F14K50.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=PIC18F14K50
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=cof
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/MPLAB.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=cof
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/MPLAB.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Source Files Quoted if spaced
SOURCEFILES_QUOTED_IF_SPACED=../BootPIC18NonJ.c ../hid.c ../main.c ../usb9.c ../usbctrltrf.c ../usbdrv.c ../usbdsc.c ../usbmmap.c

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1472/BootPIC18NonJ.o ${OBJECTDIR}/_ext/1472/hid.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/usb9.o ${OBJECTDIR}/_ext/1472/usbctrltrf.o ${OBJECTDIR}/_ext/1472/usbdrv.o ${OBJECTDIR}/_ext/1472/usbdsc.o ${OBJECTDIR}/_ext/1472/usbmmap.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1472/BootPIC18NonJ.o.d ${OBJECTDIR}/_ext/1472/hid.o.d ${OBJECTDIR}/_ext/1472/main.o.d ${OBJECTDIR}/_ext/1472/usb9.o.d ${OBJECTDIR}/_ext/1472/usbctrltrf.o.d ${OBJECTDIR}/_ext/1472/usbdrv.o.d ${OBJECTDIR}/_ext/1472/usbdsc.o.d ${OBJECTDIR}/_ext/1472/usbmmap.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1472/BootPIC18NonJ.o ${OBJECTDIR}/_ext/1472/hid.o ${OBJECTDIR}/_ext/1472/main.o ${OBJECTDIR}/_ext/1472/usb9.o ${OBJECTDIR}/_ext/1472/usbctrltrf.o ${OBJECTDIR}/_ext/1472/usbdrv.o ${OBJECTDIR}/_ext/1472/usbdsc.o ${OBJECTDIR}/_ext/1472/usbmmap.o

# Source Files
SOURCEFILES=../BootPIC18NonJ.c ../hid.c ../main.c ../usb9.c ../usbctrltrf.c ../usbdrv.c ../usbdsc.c ../usbmmap.c


CFLAGS=
ASFLAGS=
LDLIBSOPTIONS=

############# Tool locations ##########################################
# If you copy a project from one host to another, the path where the  #
# compiler is installed may be different.                             #
# If you open this project with MPLAB X in the new host, this         #
# makefile will be regenerated and the paths will be corrected.       #
#######################################################################
# fixDeps replaces a bunch of sed/cat/printf statements that slow down the build
FIXDEPS=fixDeps

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE} ${MAKE_OPTIONS} -f nbproject/Makefile-PIC18F14K50.mk dist/${CND_CONF}/${IMAGE_TYPE}/MPLAB.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}

MP_PROCESSOR_OPTION=18F14K50
MP_PROCESSOR_OPTION_LD=18f14k50
MP_LINKER_DEBUG_OPTION= -u_DEBUGCODESTART=0x3e00 -u_DEBUGCODELEN=0x200
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1472/BootPIC18NonJ.o: ../BootPIC18NonJ.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/BootPIC18NonJ.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/BootPIC18NonJ.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip/Include" -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/BootPIC18NonJ.o   ../BootPIC18NonJ.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/BootPIC18NonJ.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/BootPIC18NonJ.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/hid.o: ../hid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/hid.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hid.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip/Include" -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/hid.o   ../hid.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/hid.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hid.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip/Include" -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/main.o   ../main.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/usb9.o: ../usb9.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/usb9.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/usb9.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip/Include" -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/usb9.o   ../usb9.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/usb9.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/usb9.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/usbctrltrf.o: ../usbctrltrf.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/usbctrltrf.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/usbctrltrf.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip/Include" -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/usbctrltrf.o   ../usbctrltrf.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/usbctrltrf.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/usbctrltrf.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/usbdrv.o: ../usbdrv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/usbdrv.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/usbdrv.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip/Include" -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/usbdrv.o   ../usbdrv.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/usbdrv.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/usbdrv.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/usbdsc.o: ../usbdsc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/usbdsc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/usbdsc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip/Include" -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/usbdsc.o   ../usbdsc.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/usbdsc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/usbdsc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/usbmmap.o: ../usbmmap.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/usbmmap.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/usbmmap.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1 -p$(MP_PROCESSOR_OPTION) -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip/Include" -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/usbmmap.o   ../usbmmap.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/usbmmap.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/usbmmap.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
else
${OBJECTDIR}/_ext/1472/BootPIC18NonJ.o: ../BootPIC18NonJ.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/BootPIC18NonJ.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/BootPIC18NonJ.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip/Include" -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/BootPIC18NonJ.o   ../BootPIC18NonJ.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/BootPIC18NonJ.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/BootPIC18NonJ.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/hid.o: ../hid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/hid.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/hid.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip/Include" -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/hid.o   ../hid.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/hid.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/hid.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/main.o: ../main.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/main.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip/Include" -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/main.o   ../main.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/main.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/main.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/usb9.o: ../usb9.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/usb9.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/usb9.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip/Include" -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/usb9.o   ../usb9.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/usb9.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/usb9.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/usbctrltrf.o: ../usbctrltrf.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/usbctrltrf.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/usbctrltrf.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip/Include" -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/usbctrltrf.o   ../usbctrltrf.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/usbctrltrf.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/usbctrltrf.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/usbdrv.o: ../usbdrv.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/usbdrv.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/usbdrv.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip/Include" -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/usbdrv.o   ../usbdrv.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/usbdrv.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/usbdrv.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/usbdsc.o: ../usbdsc.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/usbdsc.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/usbdsc.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip/Include" -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/usbdsc.o   ../usbdsc.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/usbdsc.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/usbdsc.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
${OBJECTDIR}/_ext/1472/usbmmap.o: ../usbmmap.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1472 
	@${RM} ${OBJECTDIR}/_ext/1472/usbmmap.o.d 
	@${RM} ${OBJECTDIR}/_ext/1472/usbmmap.o 
	${MP_CC} $(MP_EXTRA_CC_PRE) -p$(MP_PROCESSOR_OPTION) -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip/Include" -I"../../../../../../../microchip_solutions_v2013-06-15/Microchip" -ms -oa-  -I ${MP_CC_DIR}\\..\\h  -fo ${OBJECTDIR}/_ext/1472/usbmmap.o   ../usbmmap.c 
	@${DEP_GEN} -d ${OBJECTDIR}/_ext/1472/usbmmap.o 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1472/usbmmap.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c18 
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/MPLAB.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../BootModified.18f14k50_g.lkr
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE) "..\BootModified.18f14k50_g.lkr"  -p$(MP_PROCESSOR_OPTION_LD)  -w -x -u_DEBUG -m"${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"  -z__MPLAB_BUILD=1  -u_CRUNTIME -z__MPLAB_DEBUG=1 -z__MPLAB_DEBUGGER_PK3=1 $(MP_LINKER_DEBUG_OPTION) -l ${MP_CC_DIR}\\..\\lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/MPLAB.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
else
dist/${CND_CONF}/${IMAGE_TYPE}/MPLAB.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../BootModified.18f14k50_g.lkr
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_LD} $(MP_EXTRA_LD_PRE) "..\BootModified.18f14k50_g.lkr"  -p$(MP_PROCESSOR_OPTION_LD)  -w  -m"${DISTDIR}/${PROJECTNAME}.${IMAGE_TYPE}.map"  -z__MPLAB_BUILD=1  -u_CRUNTIME -l ${MP_CC_DIR}\\..\\lib  -o dist/${CND_CONF}/${IMAGE_TYPE}/MPLAB.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  ${OBJECTFILES_QUOTED_IF_SPACED}   
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/PIC18F14K50
	${RM} -r dist/PIC18F14K50

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
