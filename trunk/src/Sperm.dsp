# Microsoft Developer Studio Project File - Name="Sperm" - Package Owner=<4>
# Microsoft Developer Studio Generated Build File, Format Version 6.00
# ** DO NOT EDIT **

# TARGTYPE "Win32 (x86) Application" 0x0101

CFG=Sperm - Win32 Debug
!MESSAGE This is not a valid makefile. To build this project using NMAKE,
!MESSAGE use the Export Makefile command and run
!MESSAGE 
!MESSAGE NMAKE /f "Sperm.mak".
!MESSAGE 
!MESSAGE You can specify a configuration when running NMAKE
!MESSAGE by defining the macro CFG on the command line. For example:
!MESSAGE 
!MESSAGE NMAKE /f "Sperm.mak" CFG="Sperm - Win32 Debug"
!MESSAGE 
!MESSAGE Possible choices for configuration are:
!MESSAGE 
!MESSAGE "Sperm - Win32 Release" (based on "Win32 (x86) Application")
!MESSAGE "Sperm - Win32 Debug" (based on "Win32 (x86) Application")
!MESSAGE 

# Begin Project
# PROP AllowPerConfigDependencies 0
# PROP Scc_ProjName ""
# PROP Scc_LocalPath ""
CPP=cl.exe
MTL=midl.exe
RSC=rc.exe

!IF  "$(CFG)" == "Sperm - Win32 Release"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 0
# PROP BASE Output_Dir "Release"
# PROP BASE Intermediate_Dir "Release"
# PROP BASE Target_Dir ""
# PROP Use_MFC 5
# PROP Use_Debug_Libraries 0
# PROP Output_Dir "Release"
# PROP Intermediate_Dir "Release"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MD /W3 /GX /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /c
# ADD CPP /nologo /MT /W3 /GX /Zi /O2 /D "WIN32" /D "NDEBUG" /D "_WINDOWS" /D "_MBCS" /Yu"stdafx.h" /FD /OPT:REF /c
# ADD BASE MTL /nologo /D "NDEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "NDEBUG" /win32
# ADD BASE RSC /l 0x804 /d "NDEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "NDEBUG"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /machine:I386
# ADD LINK32 Winmm.lib strmbasd.lib cv.lib highgui.lib cxcore.lib /nologo /subsystem:windows /machine:I386 /nodefaultlib:"LIBCMTD.LIB" /OPT:REF
# SUBTRACT LINK32 /pdb:none /debug

!ELSEIF  "$(CFG)" == "Sperm - Win32 Debug"

# PROP BASE Use_MFC 6
# PROP BASE Use_Debug_Libraries 1
# PROP BASE Output_Dir "Debug"
# PROP BASE Intermediate_Dir "Debug"
# PROP BASE Target_Dir ""
# PROP Use_MFC 6
# PROP Use_Debug_Libraries 1
# PROP Output_Dir "Debug"
# PROP Intermediate_Dir "Debug"
# PROP Ignore_Export_Lib 0
# PROP Target_Dir ""
# ADD BASE CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /Yu"stdafx.h" /FD /GZ /c
# ADD CPP /nologo /MDd /W3 /Gm /GX /ZI /Od /D "WIN32" /D "_DEBUG" /D "_WINDOWS" /D "_AFXDLL" /D "_MBCS" /FR /Yu"stdafx.h" /FD /GZ /c
# ADD BASE MTL /nologo /D "_DEBUG" /mktyplib203 /win32
# ADD MTL /nologo /D "_DEBUG" /win32
# ADD BASE RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
# ADD RSC /l 0x804 /d "_DEBUG" /d "_AFXDLL"
BSC32=bscmake.exe
# ADD BASE BSC32 /nologo
# ADD BSC32 /nologo
LINK32=link.exe
# ADD BASE LINK32 /nologo /subsystem:windows /debug /machine:I386 /pdbtype:sept
# ADD LINK32 /nologo /subsystem:windows /debug /machine:I386 /nodefaultlib:"LIBCMTD.LIB" /pdbtype:sept
# SUBTRACT LINK32 /nodefaultlib

!ENDIF 

# Begin Target

# Name "Sperm - Win32 Release"
# Name "Sperm - Win32 Debug"
# Begin Group "Source Files"

# PROP Default_Filter "cpp;c;cxx;rc;def;r;odl;idl;hpj;bat"
# Begin Source File

SOURCE=.\ActiveListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\ActiveListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\ActiveResultPropPage.cpp
# End Source File
# Begin Source File

SOURCE=.\ActiveResultPropPage.h
# End Source File
# Begin Source File

SOURCE=.\AllDataStructure.cpp
# End Source File
# Begin Source File

SOURCE=.\AllDataStructure.h
# End Source File
# Begin Source File

SOURCE=.\AllFunction.cpp
# End Source File
# Begin Source File

SOURCE=.\AllFunction.h
# End Source File
# Begin Source File

SOURCE=.\BrowerDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\BrowerDlg.h
# End Source File
# Begin Source File

SOURCE=.\CaptureVideo.cpp
# End Source File
# Begin Source File

SOURCE=.\CaptureVideo.h
# End Source File
# Begin Source File

SOURCE=.\CDXGraph.cpp
# End Source File
# Begin Source File

SOURCE=.\CDXGraph.h
# End Source File
# Begin Source File

SOURCE=.\CMySplitter.cpp
# End Source File
# Begin Source File

SOURCE=.\CMySplitter.h
# End Source File
# Begin Source File

SOURCE=.\column.cpp
# End Source File
# Begin Source File

SOURCE=.\column.h
# End Source File
# Begin Source File

SOURCE=.\columns.cpp
# End Source File
# Begin Source File

SOURCE=.\columns.h
# End Source File
# Begin Source File

SOURCE=.\CombinationAnalyseDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\CombinationAnalyseDlg.h
# End Source File
# Begin Source File

SOURCE=.\craxdrt.tlh
# End Source File
# Begin Source File

SOURCE=.\craxdrt.tli
# End Source File
# Begin Source File

SOURCE=.\crystalreportviewer10.cpp
# End Source File
# Begin Source File

SOURCE=.\crystalreportviewer10.h
# End Source File
# Begin Source File

SOURCE=.\CSampleGrabberCB_np.cpp
# End Source File
# Begin Source File

SOURCE=.\CSampleGrabberCB_np.h
# End Source File
# Begin Source File

SOURCE=.\dataformatdisp.cpp
# End Source File
# Begin Source File

SOURCE=.\dataformatdisp.h
# End Source File
# Begin Source File

SOURCE=.\datagrid.cpp
# End Source File
# Begin Source File

SOURCE=.\datagrid.h
# End Source File
# Begin Source File

SOURCE=.\DataListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\DataListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\DIB.CPP
# End Source File
# Begin Source File

SOURCE=.\DIB.H
# End Source File
# Begin Source File

SOURCE=.\DISDLG.cpp
# End Source File
# Begin Source File

SOURCE=.\DISDLG.h
# End Source File
# Begin Source File

SOURCE=.\DrawStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\DrawStatic.h
# End Source File
# Begin Source File

SOURCE=.\DShowUtilities.cpp
# End Source File
# Begin Source File

SOURCE=.\DShowUtilities.h
# End Source File
# Begin Source File

SOURCE=.\FluoreListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\FluoreListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\FluoreResultPropPage.cpp
# End Source File
# Begin Source File

SOURCE=.\FluoreResultPropPage.h
# End Source File
# Begin Source File

SOURCE=.\font.cpp
# End Source File
# Begin Source File

SOURCE=.\font.h
# End Source File
# Begin Source File

SOURCE=.\GloablFun.cpp
# End Source File
# Begin Source File

SOURCE=.\GloablFun.h
# End Source File
# Begin Source File

SOURCE=.\IGvfSnake.cpp
# End Source File
# Begin Source File

SOURCE=.\IGvfSnake.h
# End Source File
# Begin Source File

SOURCE=.\ImageConvert.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageConvert.h
# End Source File
# Begin Source File

SOURCE=.\ImageProcess.cpp
# End Source File
# Begin Source File

SOURCE=.\ImageProcess.h
# End Source File
# Begin Source File

SOURCE=.\LiveDataRetriveDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LiveDataRetriveDlg.h
# End Source File
# Begin Source File

SOURCE=.\LiveDetectDLG.cpp
# End Source File
# Begin Source File

SOURCE=.\LiveDetectDLG.h
# End Source File
# Begin Source File

SOURCE=.\LiveDetectedPamaSettingDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LiveDetectedPamaSettingDlg.h
# End Source File
# Begin Source File

SOURCE=.\LiveImagePage.cpp
# End Source File
# Begin Source File

SOURCE=.\LiveImagePage.h
# End Source File
# Begin Source File

SOURCE=.\LivenessResultPage.cpp
# End Source File
# Begin Source File

SOURCE=.\LivenessResultPage.h
# End Source File
# Begin Source File

SOURCE=.\LivePropertySheet.cpp
# End Source File
# Begin Source File

SOURCE=.\LivePropertySheet.h
# End Source File
# Begin Source File

SOURCE=.\LogInDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\LogInDlg.h
# End Source File
# Begin Source File

SOURCE=.\LoginThread.cpp
# End Source File
# Begin Source File

SOURCE=.\LoginThread.h
# End Source File
# Begin Source File

SOURCE=.\MainFrm.cpp
# End Source File
# Begin Source File

SOURCE=.\MainFrm.h
# End Source File
# Begin Source File

SOURCE=.\MemDC.h
# End Source File
# Begin Source File

SOURCE=.\MophyListCtrl.cpp
# End Source File
# Begin Source File

SOURCE=.\MophyListCtrl.h
# End Source File
# Begin Source File

SOURCE=.\MophyResultPropPage.cpp
# End Source File
# Begin Source File

SOURCE=.\MophyResultPropPage.h
# End Source File
# Begin Source File

SOURCE=.\MorphaDataGather.cpp
# End Source File
# Begin Source File

SOURCE=.\MorphaDataGather.h
# End Source File
# Begin Source File

SOURCE=.\MorphaDataRetriveDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MorphaDataRetriveDlg.h
# End Source File
# Begin Source File

SOURCE=.\MorphaDetectDLG.cpp
# End Source File
# Begin Source File

SOURCE=.\MorphaDetectDLG.h
# End Source File
# Begin Source File

SOURCE=.\MorphaDrawStatic.cpp
# End Source File
# Begin Source File

SOURCE=.\MorphaDrawStatic.h
# End Source File
# Begin Source File

SOURCE=.\MorphaEdgeDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MorphaEdgeDlg.h
# End Source File
# Begin Source File

SOURCE=.\MorphaPadDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MorphaPadDlg.h
# End Source File
# Begin Source File

SOURCE=.\MorphaParameterDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MorphaParameterDlg.h
# End Source File
# Begin Source File

SOURCE=.\MorphaResDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\MorphaResDlg.h
# End Source File
# Begin Source File

SOURCE=.\OptionDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\OptionDlg.h
# End Source File
# Begin Source File

SOURCE=.\PageEdit.cpp
# End Source File
# Begin Source File

SOURCE=.\PageEdit.h
# End Source File
# Begin Source File

SOURCE=.\PatientInfo.cpp
# End Source File
# Begin Source File

SOURCE=.\PatientInfo.h
# End Source File
# Begin Source File

SOURCE=.\PatientInfoQueryDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PatientInfoQueryDlg.h
# End Source File
# Begin Source File

SOURCE=.\picture.cpp
# End Source File
# Begin Source File

SOURCE=.\picture.h
# End Source File
# Begin Source File

SOURCE=.\PrinteDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\PrinteDlg.h
# End Source File
# Begin Source File

SOURCE=.\ReadMe.txt
# End Source File
# Begin Source File

SOURCE="D:\My Documents\boost_1_34_1\boost_1_34_1\boost\regex\v4\regbase.hpp"
# End Source File
# Begin Source File

SOURCE=.\RegesterDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\RegesterDlg.h
# End Source File
# Begin Source File

SOURCE=.\RegisterThread.cpp
# End Source File
# Begin Source File

SOURCE=.\RegisterThread.h
# End Source File
# Begin Source File

SOURCE=.\Resource.h
# End Source File
# Begin Source File

SOURCE=.\SampleGrabber.h
# End Source File
# Begin Source File

SOURCE=.\selbookmarks.cpp
# End Source File
# Begin Source File

SOURCE=.\selbookmarks.h
# End Source File
# Begin Source File

SOURCE=.\SinSpmReco.cpp
# End Source File
# Begin Source File

SOURCE=.\SinSpmReco.h
# End Source File
# Begin Source File

SOURCE=.\SnakeType.cpp
# End Source File
# Begin Source File

SOURCE=.\SnakeType.h
# End Source File
# Begin Source File

SOURCE=.\Sperm.cpp
# End Source File
# Begin Source File

SOURCE=.\Sperm.h
# End Source File
# Begin Source File

SOURCE=.\Sperm.rc
# End Source File
# Begin Source File

SOURCE=.\SpermDoc.cpp
# End Source File
# Begin Source File

SOURCE=.\SpermDoc.h
# End Source File
# Begin Source File

SOURCE=.\SpermMorphaSet.cpp
# End Source File
# Begin Source File

SOURCE=.\SpermMorphaSet.h
# End Source File
# Begin Source File

SOURCE=.\SperMopyParameter.h
# End Source File
# Begin Source File

SOURCE=.\SpermParamCalc.cpp
# End Source File
# Begin Source File

SOURCE=.\SpermParamCalc.h
# End Source File
# Begin Source File

SOURCE=.\SpermView.cpp
# End Source File
# Begin Source File

SOURCE=.\SpermView.h
# End Source File
# Begin Source File

SOURCE=.\Splash.cpp
# End Source File
# Begin Source File

SOURCE=.\Splash.h
# End Source File
# Begin Source File

SOURCE=.\split.cpp
# End Source File
# Begin Source File

SOURCE=.\split.h
# End Source File
# Begin Source File

SOURCE=.\splits.cpp
# End Source File
# Begin Source File

SOURCE=.\splits.h
# End Source File
# Begin Source File

SOURCE=.\StdAfx.cpp
# ADD CPP /Yc"stdafx.h"
# End Source File
# Begin Source File

SOURCE=.\StdAfx.h
# End Source File
# Begin Source File

SOURCE=.\stddataformatsdisp.cpp
# End Source File
# Begin Source File

SOURCE=.\stddataformatsdisp.h
# End Source File
# Begin Source File

SOURCE=.\SysReg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserManageDlg.cpp
# End Source File
# Begin Source File

SOURCE=.\UserManageDlg.h
# End Source File
# Begin Source File

SOURCE=.\VideoPlay.cpp
# End Source File
# Begin Source File

SOURCE=.\VideoPlay.h
# End Source File
# Begin Source File

SOURCE=.\View2.cpp
# End Source File
# Begin Source File

SOURCE=.\View2.h
# End Source File
# Begin Source File

SOURCE=.\View3.cpp
# End Source File
# Begin Source File

SOURCE=.\View3.h
# End Source File
# Begin Source File

SOURCE=.\Wraper.cpp
# End Source File
# Begin Source File

SOURCE=.\Wraper.h
# End Source File
# End Group
# Begin Group "Resource Files"

# PROP Default_Filter "ico;cur;bmp;dlg;rc2;rct;bin;rgs;gif;jpg;jpeg;jpe"
# Begin Source File

SOURCE=.\res\ARW05LT.ICO
# End Source File
# Begin Source File

SOURCE=.\res\ARW05RT.ICO
# End Source File
# Begin Source File

SOURCE=.\res\cursor1.cur
# End Source File
# Begin Source File

SOURCE=.\res\icon2.ico
# End Source File
# Begin Source File

SOURCE=.\res\Pointix.ico
# End Source File
# Begin Source File

SOURCE=.\res\Sperm.ico
# End Source File
# Begin Source File

SOURCE=.\res\Sperm.rc2
# End Source File
# Begin Source File

SOURCE=.\Sperm.rgs
# End Source File
# Begin Source File

SOURCE=.\res\SpermDoc.ico
# End Source File
# Begin Source File

SOURCE=.\Splsh16.bmp
# End Source File
# Begin Source File

SOURCE=.\res\Toolbar.bmp
# End Source File
# Begin Source File

SOURCE=".\res\界面图.bmp"
# End Source File
# Begin Source File

SOURCE=".\res\界面图.jpg"
# End Source File
# End Group
# Begin Source File

SOURCE=.\SysReg.h
# End Source File
# End Target
# End Project
# Section Sperm : {BEF6E003-A874-101A-8BBA-00AA00300CAB}
# 	2:5:Class:COleFont
# 	2:10:HeaderFile:font.h
# 	2:8:ImplFile:font.cpp
# End Section
# Section Sperm : {72ADFD78-2C39-11D0-9903-00A0C91BC942}
# 	1:10:IDB_SPLASH:102
# 	2:21:SplashScreenInsertKey:4.0
# End Section
# Section Sperm : {CDE57A52-8B86-11D0-B3C6-00A0C90AEA82}
# 	2:5:Class:CSelBookmarks
# 	2:10:HeaderFile:selbookmarks.h
# 	2:8:ImplFile:selbookmarks.cpp
# End Section
# Section Sperm : {7BF80981-BF32-101A-8BBB-00AA00300CAB}
# 	2:5:Class:CPicture
# 	2:10:HeaderFile:picture.h
# 	2:8:ImplFile:picture.cpp
# End Section
# Section Sperm : {CDE57A41-8B86-11D0-B3C6-00A0C90AEA82}
# 	2:5:Class:CDataGrid
# 	2:10:HeaderFile:datagrid.h
# 	2:8:ImplFile:datagrid.cpp
# End Section
# Section Sperm : {00405634-0011-0000-0C00-0D00A4A90000}
# 	1:9:IDR_SPERM:103
# End Section
# Section Sperm : {CDE57A50-8B86-11D0-B3C6-00A0C90AEA82}
# 	2:5:Class:CColumns
# 	2:10:HeaderFile:columns.h
# 	2:8:ImplFile:columns.cpp
# End Section
# Section Sperm : {E675F3F0-91B5-11D0-9484-00A0C91110ED}
# 	2:5:Class:CDataFormatDisp
# 	2:10:HeaderFile:dataformatdisp.h
# 	2:8:ImplFile:dataformatdisp.cpp
# End Section
# Section Sperm : {72ADFD6C-2C39-11D0-9903-00A0C91BC942}
# 	1:23:CG_IDR_POPUP_SPERM_VIEW:104
# End Section
# Section Sperm : {CDE57A54-8B86-11D0-B3C6-00A0C90AEA82}
# 	2:5:Class:CSplit
# 	2:10:HeaderFile:split.h
# 	2:8:ImplFile:split.cpp
# End Section
# Section Sperm : {99FF4676-FFC3-11D0-BD02-00C04FC2FB86}
# 	2:5:Class:CStdDataFormatsDisp
# 	2:10:HeaderFile:stddataformatsdisp.h
# 	2:8:ImplFile:stddataformatsdisp.cpp
# End Section
# Section Sperm : {CDE57A4F-8B86-11D0-B3C6-00A0C90AEA82}
# 	2:5:Class:CColumn
# 	2:10:HeaderFile:column.h
# 	2:8:ImplFile:column.cpp
# End Section
# Section Sperm : {CDE57A43-8B86-11D0-B3C6-00A0C90AEA82}
# 	2:21:DefaultSinkHeaderFile:datagrid.h
# 	2:16:DefaultSinkClass:CDataGrid
# End Section
# Section Sperm : {CDE57A53-8B86-11D0-B3C6-00A0C90AEA82}
# 	2:5:Class:CSplits
# 	2:10:HeaderFile:splits.h
# 	2:8:ImplFile:splits.cpp
# End Section
