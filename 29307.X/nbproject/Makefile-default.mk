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
ifeq "$(wildcard nbproject/Makefile-local-default.mk)" "nbproject/Makefile-local-default.mk"
include nbproject/Makefile-local-default.mk
endif
endif

# Environment
MKDIR=gnumkdir -p
RM=rm -f 
MV=mv 
CP=cp 

# Macros
CND_CONF=default
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
IMAGE_TYPE=debug
OUTPUT_SUFFIX=elf
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/29307.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
else
IMAGE_TYPE=production
OUTPUT_SUFFIX=hex
DEBUGGABLE_SUFFIX=elf
FINAL_IMAGE=dist/${CND_CONF}/${IMAGE_TYPE}/29307.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
endif

# Object Directory
OBJECTDIR=build/${CND_CONF}/${IMAGE_TYPE}

# Distribution Directory
DISTDIR=dist/${CND_CONF}/${IMAGE_TYPE}

# Object Files Quoted if spaced
OBJECTFILES_QUOTED_IF_SPACED=${OBJECTDIR}/_ext/1270477542/BmpDecoder.o ${OBJECTDIR}/_ext/1270477542/Button.o ${OBJECTDIR}/_ext/1270477542/Chart.o ${OBJECTDIR}/_ext/1270477542/CheckBox.o ${OBJECTDIR}/_ext/1270477542/Commands.o ${OBJECTDIR}/_ext/1270477542/Conversions.o ${OBJECTDIR}/_ext/1270477542/Delay.o ${OBJECTDIR}/_ext/1270477542/Detector.o ${OBJECTDIR}/_ext/1270477542/EEPROM_Functions.o ${OBJECTDIR}/_ext/1270477542/EditBox.o ${OBJECTDIR}/_ext/1270477542/Elements.o ${OBJECTDIR}/_ext/1270477542/FSIO.o ${OBJECTDIR}/_ext/1270477542/Font_Files.o ${OBJECTDIR}/_ext/1270477542/Framed_Icon_Images.o ${OBJECTDIR}/_ext/1270477542/Functions.o ${OBJECTDIR}/_ext/1270477542/GOL.o ${OBJECTDIR}/_ext/1270477542/GifDecoder.o ${OBJECTDIR}/_ext/1270477542/Grid.o ${OBJECTDIR}/_ext/1270477542/GroupBox.o ${OBJECTDIR}/_ext/1270477542/I2C.o ${OBJECTDIR}/_ext/1270477542/Icons.o ${OBJECTDIR}/_ext/1270477542/ImageDecoder.o ${OBJECTDIR}/_ext/1270477542/JpegDecoder.o ${OBJECTDIR}/_ext/1270477542/Keyboard_Menu.o ${OBJECTDIR}/_ext/1270477542/LCD-SST39VF1601v001.o ${OBJECTDIR}/_ext/1270477542/LMI_Icon.o ${OBJECTDIR}/_ext/1270477542/ListBox.o ${OBJECTDIR}/_ext/1270477542/Listbox_Arrow_Icons.o ${OBJECTDIR}/_ext/1270477542/Message_Box.o ${OBJECTDIR}/_ext/1270477542/Primitive.o ${OBJECTDIR}/_ext/1270477542/ProgressBar.o ${OBJECTDIR}/_ext/1270477542/RadioButton.o ${OBJECTDIR}/_ext/1270477542/SSD1906.o ${OBJECTDIR}/_ext/1270477542/Screens.o ${OBJECTDIR}/_ext/1270477542/Simplified_Operations.o ${OBJECTDIR}/_ext/1270477542/Slider.o ${OBJECTDIR}/_ext/1270477542/StaticText.o ${OBJECTDIR}/_ext/1270477542/System.o ${OBJECTDIR}/_ext/1270477542/UART.o ${OBJECTDIR}/_ext/1270477542/Window.o ${OBJECTDIR}/_ext/1270477542/beep.o ${OBJECTDIR}/_ext/1270477542/grc_compass_icon.o ${OBJECTDIR}/_ext/1270477542/i2cConfig.o ${OBJECTDIR}/_ext/1270477542/jidctint.o ${OBJECTDIR}/_ext/1270477542/uart2.o ${OBJECTDIR}/_ext/1270477542/usb_config.o ${OBJECTDIR}/_ext/1270477542/usb_host.o ${OBJECTDIR}/_ext/1270477542/usb_host_generic.o ${OBJECTDIR}/_ext/1270477542/usb_host_hid.o ${OBJECTDIR}/_ext/1270477542/usb_host_hid_parser.o ${OBJECTDIR}/_ext/1270477542/usb_host_msd.o ${OBJECTDIR}/_ext/1270477542/usb_host_msd_scsi.o ${OBJECTDIR}/_ext/812168374/29307.o ${OBJECTDIR}/_ext/812168374/RunningAverage.o ${OBJECTDIR}/_ext/812168374/instrument_constants.o ${OBJECTDIR}/_ext/812168374/reading_calculation.o ${OBJECTDIR}/_ext/812168374/electrometer.o ${OBJECTDIR}/_ext/812168374/models.o ${OBJECTDIR}/_ext/812168374/interrupt_routines.o ${OBJECTDIR}/_ext/812168374/time_constants.o ${OBJECTDIR}/_ext/812168374/model_constants.o
POSSIBLE_DEPFILES=${OBJECTDIR}/_ext/1270477542/BmpDecoder.o.d ${OBJECTDIR}/_ext/1270477542/Button.o.d ${OBJECTDIR}/_ext/1270477542/Chart.o.d ${OBJECTDIR}/_ext/1270477542/CheckBox.o.d ${OBJECTDIR}/_ext/1270477542/Commands.o.d ${OBJECTDIR}/_ext/1270477542/Conversions.o.d ${OBJECTDIR}/_ext/1270477542/Delay.o.d ${OBJECTDIR}/_ext/1270477542/Detector.o.d ${OBJECTDIR}/_ext/1270477542/EEPROM_Functions.o.d ${OBJECTDIR}/_ext/1270477542/EditBox.o.d ${OBJECTDIR}/_ext/1270477542/Elements.o.d ${OBJECTDIR}/_ext/1270477542/FSIO.o.d ${OBJECTDIR}/_ext/1270477542/Font_Files.o.d ${OBJECTDIR}/_ext/1270477542/Framed_Icon_Images.o.d ${OBJECTDIR}/_ext/1270477542/Functions.o.d ${OBJECTDIR}/_ext/1270477542/GOL.o.d ${OBJECTDIR}/_ext/1270477542/GifDecoder.o.d ${OBJECTDIR}/_ext/1270477542/Grid.o.d ${OBJECTDIR}/_ext/1270477542/GroupBox.o.d ${OBJECTDIR}/_ext/1270477542/I2C.o.d ${OBJECTDIR}/_ext/1270477542/Icons.o.d ${OBJECTDIR}/_ext/1270477542/ImageDecoder.o.d ${OBJECTDIR}/_ext/1270477542/JpegDecoder.o.d ${OBJECTDIR}/_ext/1270477542/Keyboard_Menu.o.d ${OBJECTDIR}/_ext/1270477542/LCD-SST39VF1601v001.o.d ${OBJECTDIR}/_ext/1270477542/LMI_Icon.o.d ${OBJECTDIR}/_ext/1270477542/ListBox.o.d ${OBJECTDIR}/_ext/1270477542/Listbox_Arrow_Icons.o.d ${OBJECTDIR}/_ext/1270477542/Message_Box.o.d ${OBJECTDIR}/_ext/1270477542/Primitive.o.d ${OBJECTDIR}/_ext/1270477542/ProgressBar.o.d ${OBJECTDIR}/_ext/1270477542/RadioButton.o.d ${OBJECTDIR}/_ext/1270477542/SSD1906.o.d ${OBJECTDIR}/_ext/1270477542/Screens.o.d ${OBJECTDIR}/_ext/1270477542/Simplified_Operations.o.d ${OBJECTDIR}/_ext/1270477542/Slider.o.d ${OBJECTDIR}/_ext/1270477542/StaticText.o.d ${OBJECTDIR}/_ext/1270477542/System.o.d ${OBJECTDIR}/_ext/1270477542/UART.o.d ${OBJECTDIR}/_ext/1270477542/Window.o.d ${OBJECTDIR}/_ext/1270477542/beep.o.d ${OBJECTDIR}/_ext/1270477542/grc_compass_icon.o.d ${OBJECTDIR}/_ext/1270477542/i2cConfig.o.d ${OBJECTDIR}/_ext/1270477542/jidctint.o.d ${OBJECTDIR}/_ext/1270477542/uart2.o.d ${OBJECTDIR}/_ext/1270477542/usb_config.o.d ${OBJECTDIR}/_ext/1270477542/usb_host.o.d ${OBJECTDIR}/_ext/1270477542/usb_host_generic.o.d ${OBJECTDIR}/_ext/1270477542/usb_host_hid.o.d ${OBJECTDIR}/_ext/1270477542/usb_host_hid_parser.o.d ${OBJECTDIR}/_ext/1270477542/usb_host_msd.o.d ${OBJECTDIR}/_ext/1270477542/usb_host_msd_scsi.o.d ${OBJECTDIR}/_ext/812168374/29307.o.d ${OBJECTDIR}/_ext/812168374/RunningAverage.o.d ${OBJECTDIR}/_ext/812168374/instrument_constants.o.d ${OBJECTDIR}/_ext/812168374/reading_calculation.o.d ${OBJECTDIR}/_ext/812168374/electrometer.o.d ${OBJECTDIR}/_ext/812168374/models.o.d ${OBJECTDIR}/_ext/812168374/interrupt_routines.o.d ${OBJECTDIR}/_ext/812168374/time_constants.o.d ${OBJECTDIR}/_ext/812168374/model_constants.o.d

# Object Files
OBJECTFILES=${OBJECTDIR}/_ext/1270477542/BmpDecoder.o ${OBJECTDIR}/_ext/1270477542/Button.o ${OBJECTDIR}/_ext/1270477542/Chart.o ${OBJECTDIR}/_ext/1270477542/CheckBox.o ${OBJECTDIR}/_ext/1270477542/Commands.o ${OBJECTDIR}/_ext/1270477542/Conversions.o ${OBJECTDIR}/_ext/1270477542/Delay.o ${OBJECTDIR}/_ext/1270477542/Detector.o ${OBJECTDIR}/_ext/1270477542/EEPROM_Functions.o ${OBJECTDIR}/_ext/1270477542/EditBox.o ${OBJECTDIR}/_ext/1270477542/Elements.o ${OBJECTDIR}/_ext/1270477542/FSIO.o ${OBJECTDIR}/_ext/1270477542/Font_Files.o ${OBJECTDIR}/_ext/1270477542/Framed_Icon_Images.o ${OBJECTDIR}/_ext/1270477542/Functions.o ${OBJECTDIR}/_ext/1270477542/GOL.o ${OBJECTDIR}/_ext/1270477542/GifDecoder.o ${OBJECTDIR}/_ext/1270477542/Grid.o ${OBJECTDIR}/_ext/1270477542/GroupBox.o ${OBJECTDIR}/_ext/1270477542/I2C.o ${OBJECTDIR}/_ext/1270477542/Icons.o ${OBJECTDIR}/_ext/1270477542/ImageDecoder.o ${OBJECTDIR}/_ext/1270477542/JpegDecoder.o ${OBJECTDIR}/_ext/1270477542/Keyboard_Menu.o ${OBJECTDIR}/_ext/1270477542/LCD-SST39VF1601v001.o ${OBJECTDIR}/_ext/1270477542/LMI_Icon.o ${OBJECTDIR}/_ext/1270477542/ListBox.o ${OBJECTDIR}/_ext/1270477542/Listbox_Arrow_Icons.o ${OBJECTDIR}/_ext/1270477542/Message_Box.o ${OBJECTDIR}/_ext/1270477542/Primitive.o ${OBJECTDIR}/_ext/1270477542/ProgressBar.o ${OBJECTDIR}/_ext/1270477542/RadioButton.o ${OBJECTDIR}/_ext/1270477542/SSD1906.o ${OBJECTDIR}/_ext/1270477542/Screens.o ${OBJECTDIR}/_ext/1270477542/Simplified_Operations.o ${OBJECTDIR}/_ext/1270477542/Slider.o ${OBJECTDIR}/_ext/1270477542/StaticText.o ${OBJECTDIR}/_ext/1270477542/System.o ${OBJECTDIR}/_ext/1270477542/UART.o ${OBJECTDIR}/_ext/1270477542/Window.o ${OBJECTDIR}/_ext/1270477542/beep.o ${OBJECTDIR}/_ext/1270477542/grc_compass_icon.o ${OBJECTDIR}/_ext/1270477542/i2cConfig.o ${OBJECTDIR}/_ext/1270477542/jidctint.o ${OBJECTDIR}/_ext/1270477542/uart2.o ${OBJECTDIR}/_ext/1270477542/usb_config.o ${OBJECTDIR}/_ext/1270477542/usb_host.o ${OBJECTDIR}/_ext/1270477542/usb_host_generic.o ${OBJECTDIR}/_ext/1270477542/usb_host_hid.o ${OBJECTDIR}/_ext/1270477542/usb_host_hid_parser.o ${OBJECTDIR}/_ext/1270477542/usb_host_msd.o ${OBJECTDIR}/_ext/1270477542/usb_host_msd_scsi.o ${OBJECTDIR}/_ext/812168374/29307.o ${OBJECTDIR}/_ext/812168374/RunningAverage.o ${OBJECTDIR}/_ext/812168374/instrument_constants.o ${OBJECTDIR}/_ext/812168374/reading_calculation.o ${OBJECTDIR}/_ext/812168374/electrometer.o ${OBJECTDIR}/_ext/812168374/models.o ${OBJECTDIR}/_ext/812168374/interrupt_routines.o ${OBJECTDIR}/_ext/812168374/time_constants.o ${OBJECTDIR}/_ext/812168374/model_constants.o


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

# The following macros may be used in the pre and post step lines
Device=PIC32MX460F512L
ProjectDir="C:\Projects\29307\branches\Ceedling\29307.X"
ConfName=default
ImagePath="dist\default\${IMAGE_TYPE}\29307.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}"
ImageDir="dist\default\${IMAGE_TYPE}"
ImageName="29307.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}"

.build-conf:  ${BUILD_SUBPROJECTS}
	${MAKE} ${MAKE_OPTIONS} -f nbproject/Makefile-default.mk dist/${CND_CONF}/${IMAGE_TYPE}/29307.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}
	@echo "--------------------------------------"
	@echo "User defined post-build step: [cp ${ImagePath} E:\firmware.hex]"
	@cp ${ImagePath} E:\firmware.hex
	@echo "--------------------------------------"

MP_PROCESSOR_OPTION=32MX460F512L
MP_LINKER_FILE_OPTION=,--script="..\elf32pic32mx_MSD_Boot.ld"
# ------------------------------------------------------------------------------------
# Rules for buildStep: assemble
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: assembleWithPreprocess
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
else
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: compile
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
${OBJECTDIR}/_ext/1270477542/BmpDecoder.o: ../common/BmpDecoder.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/BmpDecoder.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/BmpDecoder.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/BmpDecoder.o.d" -o ${OBJECTDIR}/_ext/1270477542/BmpDecoder.o ../common/BmpDecoder.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Button.o: ../common/Button.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Button.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Button.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Button.o.d" -o ${OBJECTDIR}/_ext/1270477542/Button.o ../common/Button.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Chart.o: ../common/Chart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Chart.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Chart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Chart.o.d" -o ${OBJECTDIR}/_ext/1270477542/Chart.o ../common/Chart.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/CheckBox.o: ../common/CheckBox.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/CheckBox.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/CheckBox.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/CheckBox.o.d" -o ${OBJECTDIR}/_ext/1270477542/CheckBox.o ../common/CheckBox.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Commands.o: ../common/Commands.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Commands.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Commands.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Commands.o.d" -o ${OBJECTDIR}/_ext/1270477542/Commands.o ../common/Commands.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Conversions.o: ../common/Conversions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Conversions.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Conversions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Conversions.o.d" -o ${OBJECTDIR}/_ext/1270477542/Conversions.o ../common/Conversions.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Delay.o: ../common/Delay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Delay.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Delay.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Delay.o.d" -o ${OBJECTDIR}/_ext/1270477542/Delay.o ../common/Delay.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Detector.o: ../common/Detector.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Detector.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Detector.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Detector.o.d" -o ${OBJECTDIR}/_ext/1270477542/Detector.o ../common/Detector.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/EEPROM_Functions.o: ../common/EEPROM_Functions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/EEPROM_Functions.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/EEPROM_Functions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/EEPROM_Functions.o.d" -o ${OBJECTDIR}/_ext/1270477542/EEPROM_Functions.o ../common/EEPROM_Functions.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/EditBox.o: ../common/EditBox.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/EditBox.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/EditBox.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/EditBox.o.d" -o ${OBJECTDIR}/_ext/1270477542/EditBox.o ../common/EditBox.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Elements.o: ../common/Elements.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Elements.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Elements.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Elements.o.d" -o ${OBJECTDIR}/_ext/1270477542/Elements.o ../common/Elements.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/FSIO.o: ../common/FSIO.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/FSIO.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/FSIO.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/FSIO.o.d" -o ${OBJECTDIR}/_ext/1270477542/FSIO.o ../common/FSIO.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Font_Files.o: ../common/Font_Files.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Font_Files.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Font_Files.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Font_Files.o.d" -o ${OBJECTDIR}/_ext/1270477542/Font_Files.o ../common/Font_Files.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Framed_Icon_Images.o: ../common/Framed_Icon_Images.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Framed_Icon_Images.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Framed_Icon_Images.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Framed_Icon_Images.o.d" -o ${OBJECTDIR}/_ext/1270477542/Framed_Icon_Images.o ../common/Framed_Icon_Images.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Functions.o: ../common/Functions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Functions.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Functions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Functions.o.d" -o ${OBJECTDIR}/_ext/1270477542/Functions.o ../common/Functions.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/GOL.o: ../common/GOL.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/GOL.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/GOL.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/GOL.o.d" -o ${OBJECTDIR}/_ext/1270477542/GOL.o ../common/GOL.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/GifDecoder.o: ../common/GifDecoder.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/GifDecoder.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/GifDecoder.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/GifDecoder.o.d" -o ${OBJECTDIR}/_ext/1270477542/GifDecoder.o ../common/GifDecoder.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Grid.o: ../common/Grid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Grid.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Grid.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Grid.o.d" -o ${OBJECTDIR}/_ext/1270477542/Grid.o ../common/Grid.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/GroupBox.o: ../common/GroupBox.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/GroupBox.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/GroupBox.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/GroupBox.o.d" -o ${OBJECTDIR}/_ext/1270477542/GroupBox.o ../common/GroupBox.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/I2C.o: ../common/I2C.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/I2C.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/I2C.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/I2C.o.d" -o ${OBJECTDIR}/_ext/1270477542/I2C.o ../common/I2C.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Icons.o: ../common/Icons.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Icons.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Icons.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Icons.o.d" -o ${OBJECTDIR}/_ext/1270477542/Icons.o ../common/Icons.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/ImageDecoder.o: ../common/ImageDecoder.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/ImageDecoder.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/ImageDecoder.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/ImageDecoder.o.d" -o ${OBJECTDIR}/_ext/1270477542/ImageDecoder.o ../common/ImageDecoder.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/JpegDecoder.o: ../common/JpegDecoder.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/JpegDecoder.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/JpegDecoder.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/JpegDecoder.o.d" -o ${OBJECTDIR}/_ext/1270477542/JpegDecoder.o ../common/JpegDecoder.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Keyboard_Menu.o: ../common/Keyboard_Menu.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Keyboard_Menu.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Keyboard_Menu.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Keyboard_Menu.o.d" -o ${OBJECTDIR}/_ext/1270477542/Keyboard_Menu.o ../common/Keyboard_Menu.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/LCD-SST39VF1601v001.o: ../common/LCD-SST39VF1601v001.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/LCD-SST39VF1601v001.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/LCD-SST39VF1601v001.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/LCD-SST39VF1601v001.o.d" -o ${OBJECTDIR}/_ext/1270477542/LCD-SST39VF1601v001.o ../common/LCD-SST39VF1601v001.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/LMI_Icon.o: ../common/LMI_Icon.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/LMI_Icon.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/LMI_Icon.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/LMI_Icon.o.d" -o ${OBJECTDIR}/_ext/1270477542/LMI_Icon.o ../common/LMI_Icon.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/ListBox.o: ../common/ListBox.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/ListBox.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/ListBox.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/ListBox.o.d" -o ${OBJECTDIR}/_ext/1270477542/ListBox.o ../common/ListBox.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Listbox_Arrow_Icons.o: ../common/Listbox_Arrow_Icons.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Listbox_Arrow_Icons.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Listbox_Arrow_Icons.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Listbox_Arrow_Icons.o.d" -o ${OBJECTDIR}/_ext/1270477542/Listbox_Arrow_Icons.o ../common/Listbox_Arrow_Icons.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Message_Box.o: ../common/Message_Box.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Message_Box.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Message_Box.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Message_Box.o.d" -o ${OBJECTDIR}/_ext/1270477542/Message_Box.o ../common/Message_Box.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Primitive.o: ../common/Primitive.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Primitive.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Primitive.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Primitive.o.d" -o ${OBJECTDIR}/_ext/1270477542/Primitive.o ../common/Primitive.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/ProgressBar.o: ../common/ProgressBar.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/ProgressBar.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/ProgressBar.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/ProgressBar.o.d" -o ${OBJECTDIR}/_ext/1270477542/ProgressBar.o ../common/ProgressBar.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/RadioButton.o: ../common/RadioButton.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/RadioButton.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/RadioButton.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/RadioButton.o.d" -o ${OBJECTDIR}/_ext/1270477542/RadioButton.o ../common/RadioButton.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/SSD1906.o: ../common/SSD1906.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/SSD1906.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/SSD1906.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/SSD1906.o.d" -o ${OBJECTDIR}/_ext/1270477542/SSD1906.o ../common/SSD1906.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Screens.o: ../common/Screens.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Screens.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Screens.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Screens.o.d" -o ${OBJECTDIR}/_ext/1270477542/Screens.o ../common/Screens.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Simplified_Operations.o: ../common/Simplified_Operations.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Simplified_Operations.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Simplified_Operations.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Simplified_Operations.o.d" -o ${OBJECTDIR}/_ext/1270477542/Simplified_Operations.o ../common/Simplified_Operations.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Slider.o: ../common/Slider.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Slider.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Slider.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Slider.o.d" -o ${OBJECTDIR}/_ext/1270477542/Slider.o ../common/Slider.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/StaticText.o: ../common/StaticText.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/StaticText.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/StaticText.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/StaticText.o.d" -o ${OBJECTDIR}/_ext/1270477542/StaticText.o ../common/StaticText.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/System.o: ../common/System.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/System.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/System.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/System.o.d" -o ${OBJECTDIR}/_ext/1270477542/System.o ../common/System.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/UART.o: ../common/UART.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/UART.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/UART.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/UART.o.d" -o ${OBJECTDIR}/_ext/1270477542/UART.o ../common/UART.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Window.o: ../common/Window.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Window.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Window.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Window.o.d" -o ${OBJECTDIR}/_ext/1270477542/Window.o ../common/Window.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/beep.o: ../common/beep.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/beep.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/beep.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/beep.o.d" -o ${OBJECTDIR}/_ext/1270477542/beep.o ../common/beep.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/grc_compass_icon.o: ../common/grc_compass_icon.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/grc_compass_icon.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/grc_compass_icon.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/grc_compass_icon.o.d" -o ${OBJECTDIR}/_ext/1270477542/grc_compass_icon.o ../common/grc_compass_icon.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/i2cConfig.o: ../common/i2cConfig.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/i2cConfig.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/i2cConfig.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/i2cConfig.o.d" -o ${OBJECTDIR}/_ext/1270477542/i2cConfig.o ../common/i2cConfig.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/jidctint.o: ../common/jidctint.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/jidctint.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/jidctint.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/jidctint.o.d" -o ${OBJECTDIR}/_ext/1270477542/jidctint.o ../common/jidctint.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/uart2.o: ../common/uart2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/uart2.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/uart2.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/uart2.o.d" -o ${OBJECTDIR}/_ext/1270477542/uart2.o ../common/uart2.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/usb_config.o: ../common/usb_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/usb_config.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/usb_config.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/usb_config.o.d" -o ${OBJECTDIR}/_ext/1270477542/usb_config.o ../common/usb_config.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/usb_host.o: ../common/usb_host.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/usb_host.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/usb_host.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/usb_host.o.d" -o ${OBJECTDIR}/_ext/1270477542/usb_host.o ../common/usb_host.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/usb_host_generic.o: ../common/usb_host_generic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/usb_host_generic.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/usb_host_generic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/usb_host_generic.o.d" -o ${OBJECTDIR}/_ext/1270477542/usb_host_generic.o ../common/usb_host_generic.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/usb_host_hid.o: ../common/usb_host_hid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/usb_host_hid.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/usb_host_hid.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/usb_host_hid.o.d" -o ${OBJECTDIR}/_ext/1270477542/usb_host_hid.o ../common/usb_host_hid.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/usb_host_hid_parser.o: ../common/usb_host_hid_parser.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/usb_host_hid_parser.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/usb_host_hid_parser.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/usb_host_hid_parser.o.d" -o ${OBJECTDIR}/_ext/1270477542/usb_host_hid_parser.o ../common/usb_host_hid_parser.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/usb_host_msd.o: ../common/usb_host_msd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/usb_host_msd.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/usb_host_msd.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/usb_host_msd.o.d" -o ${OBJECTDIR}/_ext/1270477542/usb_host_msd.o ../common/usb_host_msd.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/usb_host_msd_scsi.o: ../common/usb_host_msd_scsi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/usb_host_msd_scsi.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/usb_host_msd_scsi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/usb_host_msd_scsi.o.d" -o ${OBJECTDIR}/_ext/1270477542/usb_host_msd_scsi.o ../common/usb_host_msd_scsi.c   -fshort-enums
	
${OBJECTDIR}/_ext/812168374/29307.o: ../source/29307.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/812168374 
	@${RM} ${OBJECTDIR}/_ext/812168374/29307.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/812168374/29307.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/812168374/29307.o.d" -o ${OBJECTDIR}/_ext/812168374/29307.o ../source/29307.c   -fshort-enums
	
${OBJECTDIR}/_ext/812168374/RunningAverage.o: ../source/RunningAverage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/812168374 
	@${RM} ${OBJECTDIR}/_ext/812168374/RunningAverage.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/812168374/RunningAverage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/812168374/RunningAverage.o.d" -o ${OBJECTDIR}/_ext/812168374/RunningAverage.o ../source/RunningAverage.c   -fshort-enums
	
${OBJECTDIR}/_ext/812168374/instrument_constants.o: ../source/instrument_constants.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/812168374 
	@${RM} ${OBJECTDIR}/_ext/812168374/instrument_constants.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/812168374/instrument_constants.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/812168374/instrument_constants.o.d" -o ${OBJECTDIR}/_ext/812168374/instrument_constants.o ../source/instrument_constants.c   -fshort-enums
	
${OBJECTDIR}/_ext/812168374/reading_calculation.o: ../source/reading_calculation.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/812168374 
	@${RM} ${OBJECTDIR}/_ext/812168374/reading_calculation.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/812168374/reading_calculation.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/812168374/reading_calculation.o.d" -o ${OBJECTDIR}/_ext/812168374/reading_calculation.o ../source/reading_calculation.c   -fshort-enums
	
${OBJECTDIR}/_ext/812168374/electrometer.o: ../source/electrometer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/812168374 
	@${RM} ${OBJECTDIR}/_ext/812168374/electrometer.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/812168374/electrometer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/812168374/electrometer.o.d" -o ${OBJECTDIR}/_ext/812168374/electrometer.o ../source/electrometer.c   -fshort-enums
	
${OBJECTDIR}/_ext/812168374/models.o: ../source/models.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/812168374 
	@${RM} ${OBJECTDIR}/_ext/812168374/models.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/812168374/models.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/812168374/models.o.d" -o ${OBJECTDIR}/_ext/812168374/models.o ../source/models.c   -fshort-enums
	
${OBJECTDIR}/_ext/812168374/interrupt_routines.o: ../source/interrupt_routines.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/812168374 
	@${RM} ${OBJECTDIR}/_ext/812168374/interrupt_routines.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/812168374/interrupt_routines.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/812168374/interrupt_routines.o.d" -o ${OBJECTDIR}/_ext/812168374/interrupt_routines.o ../source/interrupt_routines.c   -fshort-enums
	
${OBJECTDIR}/_ext/812168374/time_constants.o: ../source/time_constants.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/812168374 
	@${RM} ${OBJECTDIR}/_ext/812168374/time_constants.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/812168374/time_constants.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/812168374/time_constants.o.d" -o ${OBJECTDIR}/_ext/812168374/time_constants.o ../source/time_constants.c   -fshort-enums
	
${OBJECTDIR}/_ext/812168374/model_constants.o: ../source/model_constants.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/812168374 
	@${RM} ${OBJECTDIR}/_ext/812168374/model_constants.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/812168374/model_constants.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE) -g -D__DEBUG -D__MPLAB_DEBUGGER_PK3=1   -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/812168374/model_constants.o.d" -o ${OBJECTDIR}/_ext/812168374/model_constants.o ../source/model_constants.c   -fshort-enums
	
else
${OBJECTDIR}/_ext/1270477542/BmpDecoder.o: ../common/BmpDecoder.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/BmpDecoder.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/BmpDecoder.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/BmpDecoder.o.d" -o ${OBJECTDIR}/_ext/1270477542/BmpDecoder.o ../common/BmpDecoder.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Button.o: ../common/Button.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Button.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Button.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Button.o.d" -o ${OBJECTDIR}/_ext/1270477542/Button.o ../common/Button.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Chart.o: ../common/Chart.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Chart.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Chart.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Chart.o.d" -o ${OBJECTDIR}/_ext/1270477542/Chart.o ../common/Chart.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/CheckBox.o: ../common/CheckBox.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/CheckBox.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/CheckBox.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/CheckBox.o.d" -o ${OBJECTDIR}/_ext/1270477542/CheckBox.o ../common/CheckBox.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Commands.o: ../common/Commands.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Commands.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Commands.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Commands.o.d" -o ${OBJECTDIR}/_ext/1270477542/Commands.o ../common/Commands.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Conversions.o: ../common/Conversions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Conversions.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Conversions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Conversions.o.d" -o ${OBJECTDIR}/_ext/1270477542/Conversions.o ../common/Conversions.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Delay.o: ../common/Delay.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Delay.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Delay.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Delay.o.d" -o ${OBJECTDIR}/_ext/1270477542/Delay.o ../common/Delay.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Detector.o: ../common/Detector.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Detector.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Detector.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Detector.o.d" -o ${OBJECTDIR}/_ext/1270477542/Detector.o ../common/Detector.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/EEPROM_Functions.o: ../common/EEPROM_Functions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/EEPROM_Functions.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/EEPROM_Functions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/EEPROM_Functions.o.d" -o ${OBJECTDIR}/_ext/1270477542/EEPROM_Functions.o ../common/EEPROM_Functions.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/EditBox.o: ../common/EditBox.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/EditBox.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/EditBox.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/EditBox.o.d" -o ${OBJECTDIR}/_ext/1270477542/EditBox.o ../common/EditBox.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Elements.o: ../common/Elements.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Elements.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Elements.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Elements.o.d" -o ${OBJECTDIR}/_ext/1270477542/Elements.o ../common/Elements.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/FSIO.o: ../common/FSIO.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/FSIO.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/FSIO.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/FSIO.o.d" -o ${OBJECTDIR}/_ext/1270477542/FSIO.o ../common/FSIO.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Font_Files.o: ../common/Font_Files.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Font_Files.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Font_Files.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Font_Files.o.d" -o ${OBJECTDIR}/_ext/1270477542/Font_Files.o ../common/Font_Files.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Framed_Icon_Images.o: ../common/Framed_Icon_Images.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Framed_Icon_Images.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Framed_Icon_Images.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Framed_Icon_Images.o.d" -o ${OBJECTDIR}/_ext/1270477542/Framed_Icon_Images.o ../common/Framed_Icon_Images.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Functions.o: ../common/Functions.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Functions.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Functions.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Functions.o.d" -o ${OBJECTDIR}/_ext/1270477542/Functions.o ../common/Functions.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/GOL.o: ../common/GOL.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/GOL.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/GOL.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/GOL.o.d" -o ${OBJECTDIR}/_ext/1270477542/GOL.o ../common/GOL.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/GifDecoder.o: ../common/GifDecoder.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/GifDecoder.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/GifDecoder.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/GifDecoder.o.d" -o ${OBJECTDIR}/_ext/1270477542/GifDecoder.o ../common/GifDecoder.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Grid.o: ../common/Grid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Grid.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Grid.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Grid.o.d" -o ${OBJECTDIR}/_ext/1270477542/Grid.o ../common/Grid.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/GroupBox.o: ../common/GroupBox.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/GroupBox.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/GroupBox.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/GroupBox.o.d" -o ${OBJECTDIR}/_ext/1270477542/GroupBox.o ../common/GroupBox.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/I2C.o: ../common/I2C.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/I2C.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/I2C.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/I2C.o.d" -o ${OBJECTDIR}/_ext/1270477542/I2C.o ../common/I2C.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Icons.o: ../common/Icons.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Icons.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Icons.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Icons.o.d" -o ${OBJECTDIR}/_ext/1270477542/Icons.o ../common/Icons.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/ImageDecoder.o: ../common/ImageDecoder.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/ImageDecoder.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/ImageDecoder.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/ImageDecoder.o.d" -o ${OBJECTDIR}/_ext/1270477542/ImageDecoder.o ../common/ImageDecoder.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/JpegDecoder.o: ../common/JpegDecoder.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/JpegDecoder.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/JpegDecoder.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/JpegDecoder.o.d" -o ${OBJECTDIR}/_ext/1270477542/JpegDecoder.o ../common/JpegDecoder.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Keyboard_Menu.o: ../common/Keyboard_Menu.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Keyboard_Menu.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Keyboard_Menu.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Keyboard_Menu.o.d" -o ${OBJECTDIR}/_ext/1270477542/Keyboard_Menu.o ../common/Keyboard_Menu.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/LCD-SST39VF1601v001.o: ../common/LCD-SST39VF1601v001.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/LCD-SST39VF1601v001.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/LCD-SST39VF1601v001.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/LCD-SST39VF1601v001.o.d" -o ${OBJECTDIR}/_ext/1270477542/LCD-SST39VF1601v001.o ../common/LCD-SST39VF1601v001.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/LMI_Icon.o: ../common/LMI_Icon.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/LMI_Icon.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/LMI_Icon.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/LMI_Icon.o.d" -o ${OBJECTDIR}/_ext/1270477542/LMI_Icon.o ../common/LMI_Icon.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/ListBox.o: ../common/ListBox.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/ListBox.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/ListBox.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/ListBox.o.d" -o ${OBJECTDIR}/_ext/1270477542/ListBox.o ../common/ListBox.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Listbox_Arrow_Icons.o: ../common/Listbox_Arrow_Icons.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Listbox_Arrow_Icons.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Listbox_Arrow_Icons.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Listbox_Arrow_Icons.o.d" -o ${OBJECTDIR}/_ext/1270477542/Listbox_Arrow_Icons.o ../common/Listbox_Arrow_Icons.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Message_Box.o: ../common/Message_Box.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Message_Box.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Message_Box.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Message_Box.o.d" -o ${OBJECTDIR}/_ext/1270477542/Message_Box.o ../common/Message_Box.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Primitive.o: ../common/Primitive.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Primitive.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Primitive.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Primitive.o.d" -o ${OBJECTDIR}/_ext/1270477542/Primitive.o ../common/Primitive.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/ProgressBar.o: ../common/ProgressBar.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/ProgressBar.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/ProgressBar.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/ProgressBar.o.d" -o ${OBJECTDIR}/_ext/1270477542/ProgressBar.o ../common/ProgressBar.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/RadioButton.o: ../common/RadioButton.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/RadioButton.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/RadioButton.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/RadioButton.o.d" -o ${OBJECTDIR}/_ext/1270477542/RadioButton.o ../common/RadioButton.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/SSD1906.o: ../common/SSD1906.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/SSD1906.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/SSD1906.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/SSD1906.o.d" -o ${OBJECTDIR}/_ext/1270477542/SSD1906.o ../common/SSD1906.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Screens.o: ../common/Screens.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Screens.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Screens.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Screens.o.d" -o ${OBJECTDIR}/_ext/1270477542/Screens.o ../common/Screens.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Simplified_Operations.o: ../common/Simplified_Operations.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Simplified_Operations.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Simplified_Operations.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Simplified_Operations.o.d" -o ${OBJECTDIR}/_ext/1270477542/Simplified_Operations.o ../common/Simplified_Operations.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Slider.o: ../common/Slider.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Slider.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Slider.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Slider.o.d" -o ${OBJECTDIR}/_ext/1270477542/Slider.o ../common/Slider.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/StaticText.o: ../common/StaticText.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/StaticText.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/StaticText.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/StaticText.o.d" -o ${OBJECTDIR}/_ext/1270477542/StaticText.o ../common/StaticText.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/System.o: ../common/System.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/System.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/System.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/System.o.d" -o ${OBJECTDIR}/_ext/1270477542/System.o ../common/System.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/UART.o: ../common/UART.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/UART.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/UART.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/UART.o.d" -o ${OBJECTDIR}/_ext/1270477542/UART.o ../common/UART.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/Window.o: ../common/Window.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/Window.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/Window.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/Window.o.d" -o ${OBJECTDIR}/_ext/1270477542/Window.o ../common/Window.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/beep.o: ../common/beep.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/beep.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/beep.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/beep.o.d" -o ${OBJECTDIR}/_ext/1270477542/beep.o ../common/beep.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/grc_compass_icon.o: ../common/grc_compass_icon.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/grc_compass_icon.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/grc_compass_icon.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/grc_compass_icon.o.d" -o ${OBJECTDIR}/_ext/1270477542/grc_compass_icon.o ../common/grc_compass_icon.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/i2cConfig.o: ../common/i2cConfig.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/i2cConfig.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/i2cConfig.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/i2cConfig.o.d" -o ${OBJECTDIR}/_ext/1270477542/i2cConfig.o ../common/i2cConfig.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/jidctint.o: ../common/jidctint.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/jidctint.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/jidctint.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/jidctint.o.d" -o ${OBJECTDIR}/_ext/1270477542/jidctint.o ../common/jidctint.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/uart2.o: ../common/uart2.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/uart2.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/uart2.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/uart2.o.d" -o ${OBJECTDIR}/_ext/1270477542/uart2.o ../common/uart2.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/usb_config.o: ../common/usb_config.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/usb_config.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/usb_config.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/usb_config.o.d" -o ${OBJECTDIR}/_ext/1270477542/usb_config.o ../common/usb_config.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/usb_host.o: ../common/usb_host.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/usb_host.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/usb_host.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/usb_host.o.d" -o ${OBJECTDIR}/_ext/1270477542/usb_host.o ../common/usb_host.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/usb_host_generic.o: ../common/usb_host_generic.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/usb_host_generic.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/usb_host_generic.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/usb_host_generic.o.d" -o ${OBJECTDIR}/_ext/1270477542/usb_host_generic.o ../common/usb_host_generic.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/usb_host_hid.o: ../common/usb_host_hid.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/usb_host_hid.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/usb_host_hid.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/usb_host_hid.o.d" -o ${OBJECTDIR}/_ext/1270477542/usb_host_hid.o ../common/usb_host_hid.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/usb_host_hid_parser.o: ../common/usb_host_hid_parser.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/usb_host_hid_parser.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/usb_host_hid_parser.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/usb_host_hid_parser.o.d" -o ${OBJECTDIR}/_ext/1270477542/usb_host_hid_parser.o ../common/usb_host_hid_parser.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/usb_host_msd.o: ../common/usb_host_msd.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/usb_host_msd.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/usb_host_msd.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/usb_host_msd.o.d" -o ${OBJECTDIR}/_ext/1270477542/usb_host_msd.o ../common/usb_host_msd.c   -fshort-enums
	
${OBJECTDIR}/_ext/1270477542/usb_host_msd_scsi.o: ../common/usb_host_msd_scsi.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/1270477542 
	@${RM} ${OBJECTDIR}/_ext/1270477542/usb_host_msd_scsi.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/1270477542/usb_host_msd_scsi.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/1270477542/usb_host_msd_scsi.o.d" -o ${OBJECTDIR}/_ext/1270477542/usb_host_msd_scsi.o ../common/usb_host_msd_scsi.c   -fshort-enums
	
${OBJECTDIR}/_ext/812168374/29307.o: ../source/29307.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/812168374 
	@${RM} ${OBJECTDIR}/_ext/812168374/29307.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/812168374/29307.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/812168374/29307.o.d" -o ${OBJECTDIR}/_ext/812168374/29307.o ../source/29307.c   -fshort-enums
	
${OBJECTDIR}/_ext/812168374/RunningAverage.o: ../source/RunningAverage.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/812168374 
	@${RM} ${OBJECTDIR}/_ext/812168374/RunningAverage.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/812168374/RunningAverage.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/812168374/RunningAverage.o.d" -o ${OBJECTDIR}/_ext/812168374/RunningAverage.o ../source/RunningAverage.c   -fshort-enums
	
${OBJECTDIR}/_ext/812168374/instrument_constants.o: ../source/instrument_constants.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/812168374 
	@${RM} ${OBJECTDIR}/_ext/812168374/instrument_constants.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/812168374/instrument_constants.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/812168374/instrument_constants.o.d" -o ${OBJECTDIR}/_ext/812168374/instrument_constants.o ../source/instrument_constants.c   -fshort-enums
	
${OBJECTDIR}/_ext/812168374/reading_calculation.o: ../source/reading_calculation.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/812168374 
	@${RM} ${OBJECTDIR}/_ext/812168374/reading_calculation.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/812168374/reading_calculation.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/812168374/reading_calculation.o.d" -o ${OBJECTDIR}/_ext/812168374/reading_calculation.o ../source/reading_calculation.c   -fshort-enums
	
${OBJECTDIR}/_ext/812168374/electrometer.o: ../source/electrometer.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/812168374 
	@${RM} ${OBJECTDIR}/_ext/812168374/electrometer.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/812168374/electrometer.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/812168374/electrometer.o.d" -o ${OBJECTDIR}/_ext/812168374/electrometer.o ../source/electrometer.c   -fshort-enums
	
${OBJECTDIR}/_ext/812168374/models.o: ../source/models.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/812168374 
	@${RM} ${OBJECTDIR}/_ext/812168374/models.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/812168374/models.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/812168374/models.o.d" -o ${OBJECTDIR}/_ext/812168374/models.o ../source/models.c   -fshort-enums
	
${OBJECTDIR}/_ext/812168374/interrupt_routines.o: ../source/interrupt_routines.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/812168374 
	@${RM} ${OBJECTDIR}/_ext/812168374/interrupt_routines.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/812168374/interrupt_routines.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/812168374/interrupt_routines.o.d" -o ${OBJECTDIR}/_ext/812168374/interrupt_routines.o ../source/interrupt_routines.c   -fshort-enums
	
${OBJECTDIR}/_ext/812168374/time_constants.o: ../source/time_constants.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/812168374 
	@${RM} ${OBJECTDIR}/_ext/812168374/time_constants.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/812168374/time_constants.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/812168374/time_constants.o.d" -o ${OBJECTDIR}/_ext/812168374/time_constants.o ../source/time_constants.c   -fshort-enums
	
${OBJECTDIR}/_ext/812168374/model_constants.o: ../source/model_constants.c  nbproject/Makefile-${CND_CONF}.mk
	@${MKDIR} ${OBJECTDIR}/_ext/812168374 
	@${RM} ${OBJECTDIR}/_ext/812168374/model_constants.o.d 
	@${FIXDEPS} "${OBJECTDIR}/_ext/812168374/model_constants.o.d" $(SILENT) -rsi ${MP_CC_DIR}../ -c ${MP_CC} $(MP_EXTRA_CC_PRE)  -g -x c -c -mprocessor=$(MP_PROCESSOR_OPTION) -DTHIS_IS_MODEL_9DP -I"../include" -I"../common" -Os -MMD -MF "${OBJECTDIR}/_ext/812168374/model_constants.o.d" -o ${OBJECTDIR}/_ext/812168374/model_constants.o ../source/model_constants.c   -fshort-enums
	
endif

# ------------------------------------------------------------------------------------
# Rules for buildStep: link
ifeq ($(TYPE_IMAGE), DEBUG_RUN)
dist/${CND_CONF}/${IMAGE_TYPE}/29307.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk    ../elf32pic32mx_MSD_Boot.ld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mdebugger -D__MPLAB_DEBUGGER_PK3=1 -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/29307.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}       -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=__ICD2RAM=1,--defsym=__MPLAB_DEBUG=1,--defsym=__DEBUG=1,--defsym=__MPLAB_DEBUGGER_PK3=1,--defsym=_min_heap_size=24576,--defsym=_min_stack_size=6144,-L"../../lib",-L"..",-Map="${DISTDIR}/29307.X.${IMAGE_TYPE}.map",-Os 
else
dist/${CND_CONF}/${IMAGE_TYPE}/29307.X.${IMAGE_TYPE}.${OUTPUT_SUFFIX}: ${OBJECTFILES}  nbproject/Makefile-${CND_CONF}.mk   ../elf32pic32mx_MSD_Boot.ld
	@${MKDIR} dist/${CND_CONF}/${IMAGE_TYPE} 
	${MP_CC} $(MP_EXTRA_LD_PRE)  -mprocessor=$(MP_PROCESSOR_OPTION)  -o dist/${CND_CONF}/${IMAGE_TYPE}/29307.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX} ${OBJECTFILES_QUOTED_IF_SPACED}       -Wl,--defsym=__MPLAB_BUILD=1$(MP_EXTRA_LD_POST)$(MP_LINKER_FILE_OPTION),--defsym=_min_heap_size=24576,--defsym=_min_stack_size=6144,-L"../../lib",-L"..",-Map="${DISTDIR}/29307.X.${IMAGE_TYPE}.map",-Os
	${MP_CC_DIR}\\pic32-bin2hex dist/${CND_CONF}/${IMAGE_TYPE}/29307.X.${IMAGE_TYPE}.${DEBUGGABLE_SUFFIX}  
endif


# Subprojects
.build-subprojects:


# Subprojects
.clean-subprojects:

# Clean Targets
.clean-conf: ${CLEAN_SUBPROJECTS}
	${RM} -r build/default
	${RM} -r dist/default

# Enable dependency checking
.dep.inc: .depcheck-impl

DEPFILES=$(shell mplabwildcard ${POSSIBLE_DEPFILES})
ifneq (${DEPFILES},)
include ${DEPFILES}
endif
