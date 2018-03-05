##
## Auto Generated makefile by CodeLite IDE
## any manual changes will be erased      
##
## Debug
ProjectName            :=Droid_3D
ConfigurationName      :=Debug
WorkspacePath          :=/home/dberry/Documents/Devel/1_Current/GL3/Current
ProjectPath            :=/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D
IntermediateDirectory  :=./Debug
OutDir                 := $(IntermediateDirectory)
CurrentFileName        :=
CurrentFilePath        :=
CurrentFileFullPath    :=
User                   :=dberry
Date                   :=03/06/18
CodeLitePath           :=/home/dberry/.codelite
LinkerName             :=g++
SharedObjectLinkerName :=g++ -shared -fPIC
ObjectSuffix           :=.o
DependSuffix           :=.o.d
PreprocessSuffix       :=.o.i
DebugSwitch            :=-gstab
IncludeSwitch          :=-I
LibrarySwitch          :=-l
OutputSwitch           :=-o 
LibraryPathSwitch      :=-L
PreprocessorSwitch     :=-D
SourceSwitch           :=-c 
OutputFile             :=./$(ProjectName)
Preprocessors          :=
ObjectSwitch           :=-o 
ArchiveOutputSwitch    := 
PreprocessOnlySwitch   :=-E 
ObjectsFileList        :="Droid_3D.txt"
PCHCompileFlags        :=
MakeDirCommand         :=mkdir -p
LinkOptions            :=  
IncludePath            :=  $(IncludeSwitch). $(IncludeSwitch). $(IncludeSwitch)hdr/system $(IncludeSwitch)hdr/bsp $(IncludeSwitch)hdr/console $(IncludeSwitch)hdr/game $(IncludeSwitch)hdr/glload $(IncludeSwitch)hdr/glm $(IncludeSwitch)hdr/io $(IncludeSwitch)hdr/libGL $(IncludeSwitch)hdr/openGL $(IncludeSwitch)hdr/script $(IncludeSwitch)hdr/physfs $(IncludeSwitch)/usr/include/bullet $(IncludeSwitch)/usr/include/freetype2 
IncludePCH             := 
RcIncludePath          := 
Libs                   := $(LibrarySwitch)glfw $(LibrarySwitch)GL $(LibrarySwitch)GLU $(LibrarySwitch)glload $(LibrarySwitch)rt $(LibrarySwitch)pthread $(LibrarySwitch)freetype $(LibrarySwitch)AntTweakBar $(LibrarySwitch)X11 $(LibrarySwitch)GLEW $(LibrarySwitch)BulletCollision $(LibrarySwitch)BulletDynamics $(LibrarySwitch)LinearMath $(LibrarySwitch)assimp $(LibrarySwitch)freeimage 
ArLibs                 :=  "glfw" "GL" "GLU" "glload" "rt" "pthread" "freetype" "AntTweakBar" "X11" "GLEW" "BulletCollision" "BulletDynamics" "LinearMath" "assimp" "freeimage" 
LibPath                := $(LibraryPathSwitch). 

##
## Common variables
## AR, CXX, CC, AS, CXXFLAGS and CFLAGS can be overriden using an environment variables
##
AR       := ar rcus
CXX      := g++
CC       := gcc
CXXFLAGS :=  -g -O0 -Wall -fpermissive $(Preprocessors)
CFLAGS   :=  -g -O0 -Wall $(Preprocessors)
ASFLAGS  := 
AS       := as


##
## User defined environment variables
##
CodeLiteDir:=/usr/share/codelite
Objects0=$(IntermediateDirectory)/src_openGL_s_fontUtil.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_as_module.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_console_s_comHelp.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_console_s_console.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_libGL_s_window.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_physfs_physfs_archiver_iso9660.c$(ObjectSuffix) $(IntermediateDirectory)/src_physfs_physfs_archiver_zip.c$(ObjectSuffix) $(IntermediateDirectory)/src_script_as_restore.cpp$(ObjectSuffix) 

Objects1=$(IntermediateDirectory)/src_system_s_timing.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_physfs_physfs_archiver_slb.c$(ObjectSuffix) \
	$(IntermediateDirectory)/src_script_as_typeinfo.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_physfs_physfs.c$(ObjectSuffix) $(IntermediateDirectory)/src_console_s_conScript.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_openGL_s_shaders.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_console_s_conVariables.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_physfs_physfs_archiver_unpacked.c$(ObjectSuffix) $(IntermediateDirectory)/src_bsp_s_lightMaps.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_as_callfunc_x86.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_bsp_s_renderBSP.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_as_string.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_system_s_runFrame.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_as_globalproperty.cpp$(ObjectSuffix) 

Objects2=$(IntermediateDirectory)/src_script_as_compiler.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_physfs_physfs_unicode.c$(ObjectSuffix) $(IntermediateDirectory)/src_io_io_logfile.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_scriptstdstring_utils.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_openGL_s_openGLWrap.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_bsp_s_entitiesBSP.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_system_s_maths.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_as_callfunc_xenon.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_system_s_debug.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_physfs_physfs_platform_unix.c$(ObjectSuffix) $(IntermediateDirectory)/src_io_io_textures.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_physfs_physfs_archiver_hog.c$(ObjectSuffix) $(IntermediateDirectory)/src_console_s_comQuit.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_openGL_s_openGL.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_as_callfunc_x64_msvc.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_as_datatype.cpp$(ObjectSuffix) 

Objects3=$(IntermediateDirectory)/src_script_as_outputbuffer.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_openGL_s_ttfFont.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_system_s_shutdown.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_as_scriptarray.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_as_callfunc_ppc_64.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_game_s_movement.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_as_callfunc_sh4.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_as_scriptbuilder.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_physfs_physfs_platform_winrt.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_as_string_util.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_physfs_physfs_platform_windows.c$(ObjectSuffix) $(IntermediateDirectory)/src_physfs_physfs_platform_qnx.c$(ObjectSuffix) \
	$(IntermediateDirectory)/src_script_as_scriptobject.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_physfs_physfs_platform_haiku.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_physfs_physfs_byteorder.c$(ObjectSuffix) $(IntermediateDirectory)/src_physfs_physfs_archiver_wad.c$(ObjectSuffix) $(IntermediateDirectory)/src_physfs_physfs_archiver_vdf.c$(ObjectSuffix) $(IntermediateDirectory)/src_physfs_physfs_archiver_qpak.c$(ObjectSuffix) $(IntermediateDirectory)/src_physfs_physfs_archiver_mvl.c$(ObjectSuffix) $(IntermediateDirectory)/src_physfs_physfs_archiver_grp.c$(ObjectSuffix) $(IntermediateDirectory)/src_bsp_s_varsBSP.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_physfs_physfs_archiver_7z.c$(ObjectSuffix) \
	$(IntermediateDirectory)/src_bsp_s_loadBsp.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_as_generic.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_io_io_keyboard.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_bsp_s_doorsBSP.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_bsp_s_shaderLights.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_as_parser.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_game_s_bullet.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_console_s_comOpenGLInfo.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_openGL_s_defRender.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_as_callfunc_mips.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_openGL_s_lightPass.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_openGL_s_shadowMap.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_libGL_s_physicsDebug.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_as_callfunc_ppc.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_libGL_s_assimp.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_libGL_s_physics.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_libGL_s_antBar.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_libGL_s_vsProfileLib.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_io_io_fileSystem.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_as_scriptengine.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_physfs_physfs_platform_posix.c$(ObjectSuffix) $(IntermediateDirectory)/src_io_io_loadImage.cpp$(ObjectSuffix) 

Objects4=$(IntermediateDirectory)/src_script_as_objecttype.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_as_callfunc_arm.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_as_context.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_as_callfunc_x64_mingw.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_as_callfunc.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_as_atomic.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_scriptstdstring.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_as_configgroup.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_script_as_scriptcode.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_as_memory.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_as_tokenizer.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_as_scriptfunction.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_as_bytecode.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_as_variablescope.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_system_s_startup.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_as_builder.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_as_callfunc_x64_gcc.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_as_scriptnode.cpp$(ObjectSuffix) \
	$(IntermediateDirectory)/src_script_as_gc.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_system_s_main.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_system_s_camera.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_system_s_globals.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_system_s_render.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_physfs_physfs_archiver_dir.c$(ObjectSuffix) $(IntermediateDirectory)/src_physfs_physfs_platform_os2.c$(ObjectSuffix) $(IntermediateDirectory)/src_bsp_s_collideBSP.cpp$(ObjectSuffix) $(IntermediateDirectory)/src_script_as_thread.cpp$(ObjectSuffix) 



Objects=$(Objects0) $(Objects1) $(Objects2) $(Objects3) $(Objects4) 

##
## Main Build Targets 
##
.PHONY: all clean PreBuild PrePreBuild PostBuild MakeIntermediateDirs
all: $(OutputFile)

$(OutputFile): $(IntermediateDirectory)/.d $(Objects) 
	@$(MakeDirCommand) $(@D)
	@echo "" > $(IntermediateDirectory)/.d
	@echo $(Objects0)  > $(ObjectsFileList)
	@echo $(Objects1) >> $(ObjectsFileList)
	@echo $(Objects2) >> $(ObjectsFileList)
	@echo $(Objects3) >> $(ObjectsFileList)
	@echo $(Objects4) >> $(ObjectsFileList)
	$(LinkerName) $(OutputSwitch)$(OutputFile) @$(ObjectsFileList) $(LibPath) $(Libs) $(LinkOptions)

MakeIntermediateDirs:
	@test -d ./Debug || $(MakeDirCommand) ./Debug


$(IntermediateDirectory)/.d:
	@test -d ./Debug || $(MakeDirCommand) ./Debug

PreBuild:


##
## Objects
##
$(IntermediateDirectory)/src_openGL_s_fontUtil.cpp$(ObjectSuffix): src/openGL/s_fontUtil.cpp $(IntermediateDirectory)/src_openGL_s_fontUtil.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/openGL/s_fontUtil.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_openGL_s_fontUtil.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_openGL_s_fontUtil.cpp$(DependSuffix): src/openGL/s_fontUtil.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_openGL_s_fontUtil.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_openGL_s_fontUtil.cpp$(DependSuffix) -MM src/openGL/s_fontUtil.cpp

$(IntermediateDirectory)/src_openGL_s_fontUtil.cpp$(PreprocessSuffix): src/openGL/s_fontUtil.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_openGL_s_fontUtil.cpp$(PreprocessSuffix) src/openGL/s_fontUtil.cpp

$(IntermediateDirectory)/src_script_as_module.cpp$(ObjectSuffix): src/script/as_module.cpp $(IntermediateDirectory)/src_script_as_module.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_module.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_module.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_module.cpp$(DependSuffix): src/script/as_module.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_module.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_module.cpp$(DependSuffix) -MM src/script/as_module.cpp

$(IntermediateDirectory)/src_script_as_module.cpp$(PreprocessSuffix): src/script/as_module.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_module.cpp$(PreprocessSuffix) src/script/as_module.cpp

$(IntermediateDirectory)/src_console_s_comHelp.cpp$(ObjectSuffix): src/console/s_comHelp.cpp $(IntermediateDirectory)/src_console_s_comHelp.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/console/s_comHelp.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_console_s_comHelp.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_console_s_comHelp.cpp$(DependSuffix): src/console/s_comHelp.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_console_s_comHelp.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_console_s_comHelp.cpp$(DependSuffix) -MM src/console/s_comHelp.cpp

$(IntermediateDirectory)/src_console_s_comHelp.cpp$(PreprocessSuffix): src/console/s_comHelp.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_console_s_comHelp.cpp$(PreprocessSuffix) src/console/s_comHelp.cpp

$(IntermediateDirectory)/src_console_s_console.cpp$(ObjectSuffix): src/console/s_console.cpp $(IntermediateDirectory)/src_console_s_console.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/console/s_console.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_console_s_console.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_console_s_console.cpp$(DependSuffix): src/console/s_console.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_console_s_console.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_console_s_console.cpp$(DependSuffix) -MM src/console/s_console.cpp

$(IntermediateDirectory)/src_console_s_console.cpp$(PreprocessSuffix): src/console/s_console.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_console_s_console.cpp$(PreprocessSuffix) src/console/s_console.cpp

$(IntermediateDirectory)/src_libGL_s_window.cpp$(ObjectSuffix): src/libGL/s_window.cpp $(IntermediateDirectory)/src_libGL_s_window.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/libGL/s_window.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_libGL_s_window.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_libGL_s_window.cpp$(DependSuffix): src/libGL/s_window.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_libGL_s_window.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_libGL_s_window.cpp$(DependSuffix) -MM src/libGL/s_window.cpp

$(IntermediateDirectory)/src_libGL_s_window.cpp$(PreprocessSuffix): src/libGL/s_window.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_libGL_s_window.cpp$(PreprocessSuffix) src/libGL/s_window.cpp

$(IntermediateDirectory)/src_physfs_physfs_archiver_iso9660.c$(ObjectSuffix): src/physfs/physfs_archiver_iso9660.c $(IntermediateDirectory)/src_physfs_physfs_archiver_iso9660.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/physfs/physfs_archiver_iso9660.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_physfs_physfs_archiver_iso9660.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_physfs_physfs_archiver_iso9660.c$(DependSuffix): src/physfs/physfs_archiver_iso9660.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_physfs_physfs_archiver_iso9660.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_physfs_physfs_archiver_iso9660.c$(DependSuffix) -MM src/physfs/physfs_archiver_iso9660.c

$(IntermediateDirectory)/src_physfs_physfs_archiver_iso9660.c$(PreprocessSuffix): src/physfs/physfs_archiver_iso9660.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_physfs_physfs_archiver_iso9660.c$(PreprocessSuffix) src/physfs/physfs_archiver_iso9660.c

$(IntermediateDirectory)/src_physfs_physfs_archiver_zip.c$(ObjectSuffix): src/physfs/physfs_archiver_zip.c $(IntermediateDirectory)/src_physfs_physfs_archiver_zip.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/physfs/physfs_archiver_zip.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_physfs_physfs_archiver_zip.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_physfs_physfs_archiver_zip.c$(DependSuffix): src/physfs/physfs_archiver_zip.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_physfs_physfs_archiver_zip.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_physfs_physfs_archiver_zip.c$(DependSuffix) -MM src/physfs/physfs_archiver_zip.c

$(IntermediateDirectory)/src_physfs_physfs_archiver_zip.c$(PreprocessSuffix): src/physfs/physfs_archiver_zip.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_physfs_physfs_archiver_zip.c$(PreprocessSuffix) src/physfs/physfs_archiver_zip.c

$(IntermediateDirectory)/src_script_as_restore.cpp$(ObjectSuffix): src/script/as_restore.cpp $(IntermediateDirectory)/src_script_as_restore.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_restore.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_restore.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_restore.cpp$(DependSuffix): src/script/as_restore.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_restore.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_restore.cpp$(DependSuffix) -MM src/script/as_restore.cpp

$(IntermediateDirectory)/src_script_as_restore.cpp$(PreprocessSuffix): src/script/as_restore.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_restore.cpp$(PreprocessSuffix) src/script/as_restore.cpp

$(IntermediateDirectory)/src_system_s_timing.cpp$(ObjectSuffix): src/system/s_timing.cpp $(IntermediateDirectory)/src_system_s_timing.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/system/s_timing.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_system_s_timing.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_system_s_timing.cpp$(DependSuffix): src/system/s_timing.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_system_s_timing.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_system_s_timing.cpp$(DependSuffix) -MM src/system/s_timing.cpp

$(IntermediateDirectory)/src_system_s_timing.cpp$(PreprocessSuffix): src/system/s_timing.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_system_s_timing.cpp$(PreprocessSuffix) src/system/s_timing.cpp

$(IntermediateDirectory)/src_physfs_physfs_archiver_slb.c$(ObjectSuffix): src/physfs/physfs_archiver_slb.c $(IntermediateDirectory)/src_physfs_physfs_archiver_slb.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/physfs/physfs_archiver_slb.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_physfs_physfs_archiver_slb.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_physfs_physfs_archiver_slb.c$(DependSuffix): src/physfs/physfs_archiver_slb.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_physfs_physfs_archiver_slb.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_physfs_physfs_archiver_slb.c$(DependSuffix) -MM src/physfs/physfs_archiver_slb.c

$(IntermediateDirectory)/src_physfs_physfs_archiver_slb.c$(PreprocessSuffix): src/physfs/physfs_archiver_slb.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_physfs_physfs_archiver_slb.c$(PreprocessSuffix) src/physfs/physfs_archiver_slb.c

$(IntermediateDirectory)/src_script_as_typeinfo.cpp$(ObjectSuffix): src/script/as_typeinfo.cpp $(IntermediateDirectory)/src_script_as_typeinfo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_typeinfo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_typeinfo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_typeinfo.cpp$(DependSuffix): src/script/as_typeinfo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_typeinfo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_typeinfo.cpp$(DependSuffix) -MM src/script/as_typeinfo.cpp

$(IntermediateDirectory)/src_script_as_typeinfo.cpp$(PreprocessSuffix): src/script/as_typeinfo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_typeinfo.cpp$(PreprocessSuffix) src/script/as_typeinfo.cpp

$(IntermediateDirectory)/src_physfs_physfs.c$(ObjectSuffix): src/physfs/physfs.c $(IntermediateDirectory)/src_physfs_physfs.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/physfs/physfs.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_physfs_physfs.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_physfs_physfs.c$(DependSuffix): src/physfs/physfs.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_physfs_physfs.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_physfs_physfs.c$(DependSuffix) -MM src/physfs/physfs.c

$(IntermediateDirectory)/src_physfs_physfs.c$(PreprocessSuffix): src/physfs/physfs.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_physfs_physfs.c$(PreprocessSuffix) src/physfs/physfs.c

$(IntermediateDirectory)/src_console_s_conScript.cpp$(ObjectSuffix): src/console/s_conScript.cpp $(IntermediateDirectory)/src_console_s_conScript.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/console/s_conScript.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_console_s_conScript.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_console_s_conScript.cpp$(DependSuffix): src/console/s_conScript.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_console_s_conScript.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_console_s_conScript.cpp$(DependSuffix) -MM src/console/s_conScript.cpp

$(IntermediateDirectory)/src_console_s_conScript.cpp$(PreprocessSuffix): src/console/s_conScript.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_console_s_conScript.cpp$(PreprocessSuffix) src/console/s_conScript.cpp

$(IntermediateDirectory)/src_openGL_s_shaders.cpp$(ObjectSuffix): src/openGL/s_shaders.cpp $(IntermediateDirectory)/src_openGL_s_shaders.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/openGL/s_shaders.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_openGL_s_shaders.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_openGL_s_shaders.cpp$(DependSuffix): src/openGL/s_shaders.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_openGL_s_shaders.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_openGL_s_shaders.cpp$(DependSuffix) -MM src/openGL/s_shaders.cpp

$(IntermediateDirectory)/src_openGL_s_shaders.cpp$(PreprocessSuffix): src/openGL/s_shaders.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_openGL_s_shaders.cpp$(PreprocessSuffix) src/openGL/s_shaders.cpp

$(IntermediateDirectory)/src_console_s_conVariables.cpp$(ObjectSuffix): src/console/s_conVariables.cpp $(IntermediateDirectory)/src_console_s_conVariables.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/console/s_conVariables.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_console_s_conVariables.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_console_s_conVariables.cpp$(DependSuffix): src/console/s_conVariables.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_console_s_conVariables.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_console_s_conVariables.cpp$(DependSuffix) -MM src/console/s_conVariables.cpp

$(IntermediateDirectory)/src_console_s_conVariables.cpp$(PreprocessSuffix): src/console/s_conVariables.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_console_s_conVariables.cpp$(PreprocessSuffix) src/console/s_conVariables.cpp

$(IntermediateDirectory)/src_physfs_physfs_archiver_unpacked.c$(ObjectSuffix): src/physfs/physfs_archiver_unpacked.c $(IntermediateDirectory)/src_physfs_physfs_archiver_unpacked.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/physfs/physfs_archiver_unpacked.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_physfs_physfs_archiver_unpacked.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_physfs_physfs_archiver_unpacked.c$(DependSuffix): src/physfs/physfs_archiver_unpacked.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_physfs_physfs_archiver_unpacked.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_physfs_physfs_archiver_unpacked.c$(DependSuffix) -MM src/physfs/physfs_archiver_unpacked.c

$(IntermediateDirectory)/src_physfs_physfs_archiver_unpacked.c$(PreprocessSuffix): src/physfs/physfs_archiver_unpacked.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_physfs_physfs_archiver_unpacked.c$(PreprocessSuffix) src/physfs/physfs_archiver_unpacked.c

$(IntermediateDirectory)/src_bsp_s_lightMaps.cpp$(ObjectSuffix): src/bsp/s_lightMaps.cpp $(IntermediateDirectory)/src_bsp_s_lightMaps.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/bsp/s_lightMaps.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_bsp_s_lightMaps.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_bsp_s_lightMaps.cpp$(DependSuffix): src/bsp/s_lightMaps.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_bsp_s_lightMaps.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_bsp_s_lightMaps.cpp$(DependSuffix) -MM src/bsp/s_lightMaps.cpp

$(IntermediateDirectory)/src_bsp_s_lightMaps.cpp$(PreprocessSuffix): src/bsp/s_lightMaps.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_bsp_s_lightMaps.cpp$(PreprocessSuffix) src/bsp/s_lightMaps.cpp

$(IntermediateDirectory)/src_script_as_callfunc_x86.cpp$(ObjectSuffix): src/script/as_callfunc_x86.cpp $(IntermediateDirectory)/src_script_as_callfunc_x86.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_callfunc_x86.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_callfunc_x86.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_callfunc_x86.cpp$(DependSuffix): src/script/as_callfunc_x86.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_callfunc_x86.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_callfunc_x86.cpp$(DependSuffix) -MM src/script/as_callfunc_x86.cpp

$(IntermediateDirectory)/src_script_as_callfunc_x86.cpp$(PreprocessSuffix): src/script/as_callfunc_x86.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_callfunc_x86.cpp$(PreprocessSuffix) src/script/as_callfunc_x86.cpp

$(IntermediateDirectory)/src_bsp_s_renderBSP.cpp$(ObjectSuffix): src/bsp/s_renderBSP.cpp $(IntermediateDirectory)/src_bsp_s_renderBSP.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/bsp/s_renderBSP.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_bsp_s_renderBSP.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_bsp_s_renderBSP.cpp$(DependSuffix): src/bsp/s_renderBSP.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_bsp_s_renderBSP.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_bsp_s_renderBSP.cpp$(DependSuffix) -MM src/bsp/s_renderBSP.cpp

$(IntermediateDirectory)/src_bsp_s_renderBSP.cpp$(PreprocessSuffix): src/bsp/s_renderBSP.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_bsp_s_renderBSP.cpp$(PreprocessSuffix) src/bsp/s_renderBSP.cpp

$(IntermediateDirectory)/src_script_as_string.cpp$(ObjectSuffix): src/script/as_string.cpp $(IntermediateDirectory)/src_script_as_string.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_string.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_string.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_string.cpp$(DependSuffix): src/script/as_string.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_string.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_string.cpp$(DependSuffix) -MM src/script/as_string.cpp

$(IntermediateDirectory)/src_script_as_string.cpp$(PreprocessSuffix): src/script/as_string.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_string.cpp$(PreprocessSuffix) src/script/as_string.cpp

$(IntermediateDirectory)/src_system_s_runFrame.cpp$(ObjectSuffix): src/system/s_runFrame.cpp $(IntermediateDirectory)/src_system_s_runFrame.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/system/s_runFrame.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_system_s_runFrame.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_system_s_runFrame.cpp$(DependSuffix): src/system/s_runFrame.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_system_s_runFrame.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_system_s_runFrame.cpp$(DependSuffix) -MM src/system/s_runFrame.cpp

$(IntermediateDirectory)/src_system_s_runFrame.cpp$(PreprocessSuffix): src/system/s_runFrame.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_system_s_runFrame.cpp$(PreprocessSuffix) src/system/s_runFrame.cpp

$(IntermediateDirectory)/src_script_as_globalproperty.cpp$(ObjectSuffix): src/script/as_globalproperty.cpp $(IntermediateDirectory)/src_script_as_globalproperty.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_globalproperty.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_globalproperty.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_globalproperty.cpp$(DependSuffix): src/script/as_globalproperty.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_globalproperty.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_globalproperty.cpp$(DependSuffix) -MM src/script/as_globalproperty.cpp

$(IntermediateDirectory)/src_script_as_globalproperty.cpp$(PreprocessSuffix): src/script/as_globalproperty.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_globalproperty.cpp$(PreprocessSuffix) src/script/as_globalproperty.cpp

$(IntermediateDirectory)/src_script_as_compiler.cpp$(ObjectSuffix): src/script/as_compiler.cpp $(IntermediateDirectory)/src_script_as_compiler.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_compiler.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_compiler.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_compiler.cpp$(DependSuffix): src/script/as_compiler.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_compiler.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_compiler.cpp$(DependSuffix) -MM src/script/as_compiler.cpp

$(IntermediateDirectory)/src_script_as_compiler.cpp$(PreprocessSuffix): src/script/as_compiler.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_compiler.cpp$(PreprocessSuffix) src/script/as_compiler.cpp

$(IntermediateDirectory)/src_physfs_physfs_unicode.c$(ObjectSuffix): src/physfs/physfs_unicode.c $(IntermediateDirectory)/src_physfs_physfs_unicode.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/physfs/physfs_unicode.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_physfs_physfs_unicode.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_physfs_physfs_unicode.c$(DependSuffix): src/physfs/physfs_unicode.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_physfs_physfs_unicode.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_physfs_physfs_unicode.c$(DependSuffix) -MM src/physfs/physfs_unicode.c

$(IntermediateDirectory)/src_physfs_physfs_unicode.c$(PreprocessSuffix): src/physfs/physfs_unicode.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_physfs_physfs_unicode.c$(PreprocessSuffix) src/physfs/physfs_unicode.c

$(IntermediateDirectory)/src_io_io_logfile.cpp$(ObjectSuffix): src/io/io_logfile.cpp $(IntermediateDirectory)/src_io_io_logfile.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/io/io_logfile.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_io_io_logfile.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_io_io_logfile.cpp$(DependSuffix): src/io/io_logfile.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_io_io_logfile.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_io_io_logfile.cpp$(DependSuffix) -MM src/io/io_logfile.cpp

$(IntermediateDirectory)/src_io_io_logfile.cpp$(PreprocessSuffix): src/io/io_logfile.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_io_io_logfile.cpp$(PreprocessSuffix) src/io/io_logfile.cpp

$(IntermediateDirectory)/src_script_scriptstdstring_utils.cpp$(ObjectSuffix): src/script/scriptstdstring_utils.cpp $(IntermediateDirectory)/src_script_scriptstdstring_utils.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/scriptstdstring_utils.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_scriptstdstring_utils.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_scriptstdstring_utils.cpp$(DependSuffix): src/script/scriptstdstring_utils.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_scriptstdstring_utils.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_scriptstdstring_utils.cpp$(DependSuffix) -MM src/script/scriptstdstring_utils.cpp

$(IntermediateDirectory)/src_script_scriptstdstring_utils.cpp$(PreprocessSuffix): src/script/scriptstdstring_utils.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_scriptstdstring_utils.cpp$(PreprocessSuffix) src/script/scriptstdstring_utils.cpp

$(IntermediateDirectory)/src_openGL_s_openGLWrap.cpp$(ObjectSuffix): src/openGL/s_openGLWrap.cpp $(IntermediateDirectory)/src_openGL_s_openGLWrap.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/openGL/s_openGLWrap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_openGL_s_openGLWrap.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_openGL_s_openGLWrap.cpp$(DependSuffix): src/openGL/s_openGLWrap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_openGL_s_openGLWrap.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_openGL_s_openGLWrap.cpp$(DependSuffix) -MM src/openGL/s_openGLWrap.cpp

$(IntermediateDirectory)/src_openGL_s_openGLWrap.cpp$(PreprocessSuffix): src/openGL/s_openGLWrap.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_openGL_s_openGLWrap.cpp$(PreprocessSuffix) src/openGL/s_openGLWrap.cpp

$(IntermediateDirectory)/src_bsp_s_entitiesBSP.cpp$(ObjectSuffix): src/bsp/s_entitiesBSP.cpp $(IntermediateDirectory)/src_bsp_s_entitiesBSP.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/bsp/s_entitiesBSP.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_bsp_s_entitiesBSP.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_bsp_s_entitiesBSP.cpp$(DependSuffix): src/bsp/s_entitiesBSP.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_bsp_s_entitiesBSP.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_bsp_s_entitiesBSP.cpp$(DependSuffix) -MM src/bsp/s_entitiesBSP.cpp

$(IntermediateDirectory)/src_bsp_s_entitiesBSP.cpp$(PreprocessSuffix): src/bsp/s_entitiesBSP.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_bsp_s_entitiesBSP.cpp$(PreprocessSuffix) src/bsp/s_entitiesBSP.cpp

$(IntermediateDirectory)/src_system_s_maths.cpp$(ObjectSuffix): src/system/s_maths.cpp $(IntermediateDirectory)/src_system_s_maths.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/system/s_maths.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_system_s_maths.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_system_s_maths.cpp$(DependSuffix): src/system/s_maths.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_system_s_maths.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_system_s_maths.cpp$(DependSuffix) -MM src/system/s_maths.cpp

$(IntermediateDirectory)/src_system_s_maths.cpp$(PreprocessSuffix): src/system/s_maths.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_system_s_maths.cpp$(PreprocessSuffix) src/system/s_maths.cpp

$(IntermediateDirectory)/src_script_as_callfunc_xenon.cpp$(ObjectSuffix): src/script/as_callfunc_xenon.cpp $(IntermediateDirectory)/src_script_as_callfunc_xenon.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_callfunc_xenon.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_callfunc_xenon.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_callfunc_xenon.cpp$(DependSuffix): src/script/as_callfunc_xenon.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_callfunc_xenon.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_callfunc_xenon.cpp$(DependSuffix) -MM src/script/as_callfunc_xenon.cpp

$(IntermediateDirectory)/src_script_as_callfunc_xenon.cpp$(PreprocessSuffix): src/script/as_callfunc_xenon.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_callfunc_xenon.cpp$(PreprocessSuffix) src/script/as_callfunc_xenon.cpp

$(IntermediateDirectory)/src_system_s_debug.cpp$(ObjectSuffix): src/system/s_debug.cpp $(IntermediateDirectory)/src_system_s_debug.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/system/s_debug.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_system_s_debug.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_system_s_debug.cpp$(DependSuffix): src/system/s_debug.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_system_s_debug.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_system_s_debug.cpp$(DependSuffix) -MM src/system/s_debug.cpp

$(IntermediateDirectory)/src_system_s_debug.cpp$(PreprocessSuffix): src/system/s_debug.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_system_s_debug.cpp$(PreprocessSuffix) src/system/s_debug.cpp

$(IntermediateDirectory)/src_physfs_physfs_platform_unix.c$(ObjectSuffix): src/physfs/physfs_platform_unix.c $(IntermediateDirectory)/src_physfs_physfs_platform_unix.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/physfs/physfs_platform_unix.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_physfs_physfs_platform_unix.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_physfs_physfs_platform_unix.c$(DependSuffix): src/physfs/physfs_platform_unix.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_physfs_physfs_platform_unix.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_physfs_physfs_platform_unix.c$(DependSuffix) -MM src/physfs/physfs_platform_unix.c

$(IntermediateDirectory)/src_physfs_physfs_platform_unix.c$(PreprocessSuffix): src/physfs/physfs_platform_unix.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_physfs_physfs_platform_unix.c$(PreprocessSuffix) src/physfs/physfs_platform_unix.c

$(IntermediateDirectory)/src_io_io_textures.cpp$(ObjectSuffix): src/io/io_textures.cpp $(IntermediateDirectory)/src_io_io_textures.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/io/io_textures.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_io_io_textures.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_io_io_textures.cpp$(DependSuffix): src/io/io_textures.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_io_io_textures.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_io_io_textures.cpp$(DependSuffix) -MM src/io/io_textures.cpp

$(IntermediateDirectory)/src_io_io_textures.cpp$(PreprocessSuffix): src/io/io_textures.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_io_io_textures.cpp$(PreprocessSuffix) src/io/io_textures.cpp

$(IntermediateDirectory)/src_physfs_physfs_archiver_hog.c$(ObjectSuffix): src/physfs/physfs_archiver_hog.c $(IntermediateDirectory)/src_physfs_physfs_archiver_hog.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/physfs/physfs_archiver_hog.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_physfs_physfs_archiver_hog.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_physfs_physfs_archiver_hog.c$(DependSuffix): src/physfs/physfs_archiver_hog.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_physfs_physfs_archiver_hog.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_physfs_physfs_archiver_hog.c$(DependSuffix) -MM src/physfs/physfs_archiver_hog.c

$(IntermediateDirectory)/src_physfs_physfs_archiver_hog.c$(PreprocessSuffix): src/physfs/physfs_archiver_hog.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_physfs_physfs_archiver_hog.c$(PreprocessSuffix) src/physfs/physfs_archiver_hog.c

$(IntermediateDirectory)/src_console_s_comQuit.cpp$(ObjectSuffix): src/console/s_comQuit.cpp $(IntermediateDirectory)/src_console_s_comQuit.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/console/s_comQuit.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_console_s_comQuit.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_console_s_comQuit.cpp$(DependSuffix): src/console/s_comQuit.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_console_s_comQuit.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_console_s_comQuit.cpp$(DependSuffix) -MM src/console/s_comQuit.cpp

$(IntermediateDirectory)/src_console_s_comQuit.cpp$(PreprocessSuffix): src/console/s_comQuit.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_console_s_comQuit.cpp$(PreprocessSuffix) src/console/s_comQuit.cpp

$(IntermediateDirectory)/src_openGL_s_openGL.cpp$(ObjectSuffix): src/openGL/s_openGL.cpp $(IntermediateDirectory)/src_openGL_s_openGL.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/openGL/s_openGL.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_openGL_s_openGL.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_openGL_s_openGL.cpp$(DependSuffix): src/openGL/s_openGL.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_openGL_s_openGL.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_openGL_s_openGL.cpp$(DependSuffix) -MM src/openGL/s_openGL.cpp

$(IntermediateDirectory)/src_openGL_s_openGL.cpp$(PreprocessSuffix): src/openGL/s_openGL.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_openGL_s_openGL.cpp$(PreprocessSuffix) src/openGL/s_openGL.cpp

$(IntermediateDirectory)/src_script_as_callfunc_x64_msvc.cpp$(ObjectSuffix): src/script/as_callfunc_x64_msvc.cpp $(IntermediateDirectory)/src_script_as_callfunc_x64_msvc.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_callfunc_x64_msvc.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_callfunc_x64_msvc.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_callfunc_x64_msvc.cpp$(DependSuffix): src/script/as_callfunc_x64_msvc.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_callfunc_x64_msvc.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_callfunc_x64_msvc.cpp$(DependSuffix) -MM src/script/as_callfunc_x64_msvc.cpp

$(IntermediateDirectory)/src_script_as_callfunc_x64_msvc.cpp$(PreprocessSuffix): src/script/as_callfunc_x64_msvc.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_callfunc_x64_msvc.cpp$(PreprocessSuffix) src/script/as_callfunc_x64_msvc.cpp

$(IntermediateDirectory)/src_script_as_datatype.cpp$(ObjectSuffix): src/script/as_datatype.cpp $(IntermediateDirectory)/src_script_as_datatype.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_datatype.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_datatype.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_datatype.cpp$(DependSuffix): src/script/as_datatype.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_datatype.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_datatype.cpp$(DependSuffix) -MM src/script/as_datatype.cpp

$(IntermediateDirectory)/src_script_as_datatype.cpp$(PreprocessSuffix): src/script/as_datatype.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_datatype.cpp$(PreprocessSuffix) src/script/as_datatype.cpp

$(IntermediateDirectory)/src_script_as_outputbuffer.cpp$(ObjectSuffix): src/script/as_outputbuffer.cpp $(IntermediateDirectory)/src_script_as_outputbuffer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_outputbuffer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_outputbuffer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_outputbuffer.cpp$(DependSuffix): src/script/as_outputbuffer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_outputbuffer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_outputbuffer.cpp$(DependSuffix) -MM src/script/as_outputbuffer.cpp

$(IntermediateDirectory)/src_script_as_outputbuffer.cpp$(PreprocessSuffix): src/script/as_outputbuffer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_outputbuffer.cpp$(PreprocessSuffix) src/script/as_outputbuffer.cpp

$(IntermediateDirectory)/src_openGL_s_ttfFont.cpp$(ObjectSuffix): src/openGL/s_ttfFont.cpp $(IntermediateDirectory)/src_openGL_s_ttfFont.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/openGL/s_ttfFont.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_openGL_s_ttfFont.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_openGL_s_ttfFont.cpp$(DependSuffix): src/openGL/s_ttfFont.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_openGL_s_ttfFont.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_openGL_s_ttfFont.cpp$(DependSuffix) -MM src/openGL/s_ttfFont.cpp

$(IntermediateDirectory)/src_openGL_s_ttfFont.cpp$(PreprocessSuffix): src/openGL/s_ttfFont.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_openGL_s_ttfFont.cpp$(PreprocessSuffix) src/openGL/s_ttfFont.cpp

$(IntermediateDirectory)/src_system_s_shutdown.cpp$(ObjectSuffix): src/system/s_shutdown.cpp $(IntermediateDirectory)/src_system_s_shutdown.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/system/s_shutdown.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_system_s_shutdown.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_system_s_shutdown.cpp$(DependSuffix): src/system/s_shutdown.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_system_s_shutdown.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_system_s_shutdown.cpp$(DependSuffix) -MM src/system/s_shutdown.cpp

$(IntermediateDirectory)/src_system_s_shutdown.cpp$(PreprocessSuffix): src/system/s_shutdown.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_system_s_shutdown.cpp$(PreprocessSuffix) src/system/s_shutdown.cpp

$(IntermediateDirectory)/src_script_as_scriptarray.cpp$(ObjectSuffix): src/script/as_scriptarray.cpp $(IntermediateDirectory)/src_script_as_scriptarray.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_scriptarray.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_scriptarray.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_scriptarray.cpp$(DependSuffix): src/script/as_scriptarray.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_scriptarray.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_scriptarray.cpp$(DependSuffix) -MM src/script/as_scriptarray.cpp

$(IntermediateDirectory)/src_script_as_scriptarray.cpp$(PreprocessSuffix): src/script/as_scriptarray.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_scriptarray.cpp$(PreprocessSuffix) src/script/as_scriptarray.cpp

$(IntermediateDirectory)/src_script_as_callfunc_ppc_64.cpp$(ObjectSuffix): src/script/as_callfunc_ppc_64.cpp $(IntermediateDirectory)/src_script_as_callfunc_ppc_64.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_callfunc_ppc_64.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_callfunc_ppc_64.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_callfunc_ppc_64.cpp$(DependSuffix): src/script/as_callfunc_ppc_64.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_callfunc_ppc_64.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_callfunc_ppc_64.cpp$(DependSuffix) -MM src/script/as_callfunc_ppc_64.cpp

$(IntermediateDirectory)/src_script_as_callfunc_ppc_64.cpp$(PreprocessSuffix): src/script/as_callfunc_ppc_64.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_callfunc_ppc_64.cpp$(PreprocessSuffix) src/script/as_callfunc_ppc_64.cpp

$(IntermediateDirectory)/src_game_s_movement.cpp$(ObjectSuffix): src/game/s_movement.cpp $(IntermediateDirectory)/src_game_s_movement.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/game/s_movement.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_game_s_movement.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_game_s_movement.cpp$(DependSuffix): src/game/s_movement.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_game_s_movement.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_game_s_movement.cpp$(DependSuffix) -MM src/game/s_movement.cpp

$(IntermediateDirectory)/src_game_s_movement.cpp$(PreprocessSuffix): src/game/s_movement.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_game_s_movement.cpp$(PreprocessSuffix) src/game/s_movement.cpp

$(IntermediateDirectory)/src_script_as_callfunc_sh4.cpp$(ObjectSuffix): src/script/as_callfunc_sh4.cpp $(IntermediateDirectory)/src_script_as_callfunc_sh4.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_callfunc_sh4.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_callfunc_sh4.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_callfunc_sh4.cpp$(DependSuffix): src/script/as_callfunc_sh4.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_callfunc_sh4.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_callfunc_sh4.cpp$(DependSuffix) -MM src/script/as_callfunc_sh4.cpp

$(IntermediateDirectory)/src_script_as_callfunc_sh4.cpp$(PreprocessSuffix): src/script/as_callfunc_sh4.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_callfunc_sh4.cpp$(PreprocessSuffix) src/script/as_callfunc_sh4.cpp

$(IntermediateDirectory)/src_script_as_scriptbuilder.cpp$(ObjectSuffix): src/script/as_scriptbuilder.cpp $(IntermediateDirectory)/src_script_as_scriptbuilder.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_scriptbuilder.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_scriptbuilder.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_scriptbuilder.cpp$(DependSuffix): src/script/as_scriptbuilder.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_scriptbuilder.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_scriptbuilder.cpp$(DependSuffix) -MM src/script/as_scriptbuilder.cpp

$(IntermediateDirectory)/src_script_as_scriptbuilder.cpp$(PreprocessSuffix): src/script/as_scriptbuilder.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_scriptbuilder.cpp$(PreprocessSuffix) src/script/as_scriptbuilder.cpp

$(IntermediateDirectory)/src_physfs_physfs_platform_winrt.cpp$(ObjectSuffix): src/physfs/physfs_platform_winrt.cpp $(IntermediateDirectory)/src_physfs_physfs_platform_winrt.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/physfs/physfs_platform_winrt.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_physfs_physfs_platform_winrt.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_physfs_physfs_platform_winrt.cpp$(DependSuffix): src/physfs/physfs_platform_winrt.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_physfs_physfs_platform_winrt.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_physfs_physfs_platform_winrt.cpp$(DependSuffix) -MM src/physfs/physfs_platform_winrt.cpp

$(IntermediateDirectory)/src_physfs_physfs_platform_winrt.cpp$(PreprocessSuffix): src/physfs/physfs_platform_winrt.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_physfs_physfs_platform_winrt.cpp$(PreprocessSuffix) src/physfs/physfs_platform_winrt.cpp

$(IntermediateDirectory)/src_script_as_string_util.cpp$(ObjectSuffix): src/script/as_string_util.cpp $(IntermediateDirectory)/src_script_as_string_util.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_string_util.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_string_util.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_string_util.cpp$(DependSuffix): src/script/as_string_util.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_string_util.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_string_util.cpp$(DependSuffix) -MM src/script/as_string_util.cpp

$(IntermediateDirectory)/src_script_as_string_util.cpp$(PreprocessSuffix): src/script/as_string_util.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_string_util.cpp$(PreprocessSuffix) src/script/as_string_util.cpp

$(IntermediateDirectory)/src_physfs_physfs_platform_windows.c$(ObjectSuffix): src/physfs/physfs_platform_windows.c $(IntermediateDirectory)/src_physfs_physfs_platform_windows.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/physfs/physfs_platform_windows.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_physfs_physfs_platform_windows.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_physfs_physfs_platform_windows.c$(DependSuffix): src/physfs/physfs_platform_windows.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_physfs_physfs_platform_windows.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_physfs_physfs_platform_windows.c$(DependSuffix) -MM src/physfs/physfs_platform_windows.c

$(IntermediateDirectory)/src_physfs_physfs_platform_windows.c$(PreprocessSuffix): src/physfs/physfs_platform_windows.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_physfs_physfs_platform_windows.c$(PreprocessSuffix) src/physfs/physfs_platform_windows.c

$(IntermediateDirectory)/src_physfs_physfs_platform_qnx.c$(ObjectSuffix): src/physfs/physfs_platform_qnx.c $(IntermediateDirectory)/src_physfs_physfs_platform_qnx.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/physfs/physfs_platform_qnx.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_physfs_physfs_platform_qnx.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_physfs_physfs_platform_qnx.c$(DependSuffix): src/physfs/physfs_platform_qnx.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_physfs_physfs_platform_qnx.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_physfs_physfs_platform_qnx.c$(DependSuffix) -MM src/physfs/physfs_platform_qnx.c

$(IntermediateDirectory)/src_physfs_physfs_platform_qnx.c$(PreprocessSuffix): src/physfs/physfs_platform_qnx.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_physfs_physfs_platform_qnx.c$(PreprocessSuffix) src/physfs/physfs_platform_qnx.c

$(IntermediateDirectory)/src_script_as_scriptobject.cpp$(ObjectSuffix): src/script/as_scriptobject.cpp $(IntermediateDirectory)/src_script_as_scriptobject.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_scriptobject.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_scriptobject.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_scriptobject.cpp$(DependSuffix): src/script/as_scriptobject.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_scriptobject.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_scriptobject.cpp$(DependSuffix) -MM src/script/as_scriptobject.cpp

$(IntermediateDirectory)/src_script_as_scriptobject.cpp$(PreprocessSuffix): src/script/as_scriptobject.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_scriptobject.cpp$(PreprocessSuffix) src/script/as_scriptobject.cpp

$(IntermediateDirectory)/src_physfs_physfs_platform_haiku.cpp$(ObjectSuffix): src/physfs/physfs_platform_haiku.cpp $(IntermediateDirectory)/src_physfs_physfs_platform_haiku.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/physfs/physfs_platform_haiku.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_physfs_physfs_platform_haiku.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_physfs_physfs_platform_haiku.cpp$(DependSuffix): src/physfs/physfs_platform_haiku.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_physfs_physfs_platform_haiku.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_physfs_physfs_platform_haiku.cpp$(DependSuffix) -MM src/physfs/physfs_platform_haiku.cpp

$(IntermediateDirectory)/src_physfs_physfs_platform_haiku.cpp$(PreprocessSuffix): src/physfs/physfs_platform_haiku.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_physfs_physfs_platform_haiku.cpp$(PreprocessSuffix) src/physfs/physfs_platform_haiku.cpp

$(IntermediateDirectory)/src_physfs_physfs_byteorder.c$(ObjectSuffix): src/physfs/physfs_byteorder.c $(IntermediateDirectory)/src_physfs_physfs_byteorder.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/physfs/physfs_byteorder.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_physfs_physfs_byteorder.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_physfs_physfs_byteorder.c$(DependSuffix): src/physfs/physfs_byteorder.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_physfs_physfs_byteorder.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_physfs_physfs_byteorder.c$(DependSuffix) -MM src/physfs/physfs_byteorder.c

$(IntermediateDirectory)/src_physfs_physfs_byteorder.c$(PreprocessSuffix): src/physfs/physfs_byteorder.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_physfs_physfs_byteorder.c$(PreprocessSuffix) src/physfs/physfs_byteorder.c

$(IntermediateDirectory)/src_physfs_physfs_archiver_wad.c$(ObjectSuffix): src/physfs/physfs_archiver_wad.c $(IntermediateDirectory)/src_physfs_physfs_archiver_wad.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/physfs/physfs_archiver_wad.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_physfs_physfs_archiver_wad.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_physfs_physfs_archiver_wad.c$(DependSuffix): src/physfs/physfs_archiver_wad.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_physfs_physfs_archiver_wad.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_physfs_physfs_archiver_wad.c$(DependSuffix) -MM src/physfs/physfs_archiver_wad.c

$(IntermediateDirectory)/src_physfs_physfs_archiver_wad.c$(PreprocessSuffix): src/physfs/physfs_archiver_wad.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_physfs_physfs_archiver_wad.c$(PreprocessSuffix) src/physfs/physfs_archiver_wad.c

$(IntermediateDirectory)/src_physfs_physfs_archiver_vdf.c$(ObjectSuffix): src/physfs/physfs_archiver_vdf.c $(IntermediateDirectory)/src_physfs_physfs_archiver_vdf.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/physfs/physfs_archiver_vdf.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_physfs_physfs_archiver_vdf.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_physfs_physfs_archiver_vdf.c$(DependSuffix): src/physfs/physfs_archiver_vdf.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_physfs_physfs_archiver_vdf.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_physfs_physfs_archiver_vdf.c$(DependSuffix) -MM src/physfs/physfs_archiver_vdf.c

$(IntermediateDirectory)/src_physfs_physfs_archiver_vdf.c$(PreprocessSuffix): src/physfs/physfs_archiver_vdf.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_physfs_physfs_archiver_vdf.c$(PreprocessSuffix) src/physfs/physfs_archiver_vdf.c

$(IntermediateDirectory)/src_physfs_physfs_archiver_qpak.c$(ObjectSuffix): src/physfs/physfs_archiver_qpak.c $(IntermediateDirectory)/src_physfs_physfs_archiver_qpak.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/physfs/physfs_archiver_qpak.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_physfs_physfs_archiver_qpak.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_physfs_physfs_archiver_qpak.c$(DependSuffix): src/physfs/physfs_archiver_qpak.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_physfs_physfs_archiver_qpak.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_physfs_physfs_archiver_qpak.c$(DependSuffix) -MM src/physfs/physfs_archiver_qpak.c

$(IntermediateDirectory)/src_physfs_physfs_archiver_qpak.c$(PreprocessSuffix): src/physfs/physfs_archiver_qpak.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_physfs_physfs_archiver_qpak.c$(PreprocessSuffix) src/physfs/physfs_archiver_qpak.c

$(IntermediateDirectory)/src_physfs_physfs_archiver_mvl.c$(ObjectSuffix): src/physfs/physfs_archiver_mvl.c $(IntermediateDirectory)/src_physfs_physfs_archiver_mvl.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/physfs/physfs_archiver_mvl.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_physfs_physfs_archiver_mvl.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_physfs_physfs_archiver_mvl.c$(DependSuffix): src/physfs/physfs_archiver_mvl.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_physfs_physfs_archiver_mvl.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_physfs_physfs_archiver_mvl.c$(DependSuffix) -MM src/physfs/physfs_archiver_mvl.c

$(IntermediateDirectory)/src_physfs_physfs_archiver_mvl.c$(PreprocessSuffix): src/physfs/physfs_archiver_mvl.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_physfs_physfs_archiver_mvl.c$(PreprocessSuffix) src/physfs/physfs_archiver_mvl.c

$(IntermediateDirectory)/src_physfs_physfs_archiver_grp.c$(ObjectSuffix): src/physfs/physfs_archiver_grp.c $(IntermediateDirectory)/src_physfs_physfs_archiver_grp.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/physfs/physfs_archiver_grp.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_physfs_physfs_archiver_grp.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_physfs_physfs_archiver_grp.c$(DependSuffix): src/physfs/physfs_archiver_grp.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_physfs_physfs_archiver_grp.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_physfs_physfs_archiver_grp.c$(DependSuffix) -MM src/physfs/physfs_archiver_grp.c

$(IntermediateDirectory)/src_physfs_physfs_archiver_grp.c$(PreprocessSuffix): src/physfs/physfs_archiver_grp.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_physfs_physfs_archiver_grp.c$(PreprocessSuffix) src/physfs/physfs_archiver_grp.c

$(IntermediateDirectory)/src_bsp_s_varsBSP.cpp$(ObjectSuffix): src/bsp/s_varsBSP.cpp $(IntermediateDirectory)/src_bsp_s_varsBSP.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/bsp/s_varsBSP.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_bsp_s_varsBSP.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_bsp_s_varsBSP.cpp$(DependSuffix): src/bsp/s_varsBSP.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_bsp_s_varsBSP.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_bsp_s_varsBSP.cpp$(DependSuffix) -MM src/bsp/s_varsBSP.cpp

$(IntermediateDirectory)/src_bsp_s_varsBSP.cpp$(PreprocessSuffix): src/bsp/s_varsBSP.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_bsp_s_varsBSP.cpp$(PreprocessSuffix) src/bsp/s_varsBSP.cpp

$(IntermediateDirectory)/src_physfs_physfs_archiver_7z.c$(ObjectSuffix): src/physfs/physfs_archiver_7z.c $(IntermediateDirectory)/src_physfs_physfs_archiver_7z.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/physfs/physfs_archiver_7z.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_physfs_physfs_archiver_7z.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_physfs_physfs_archiver_7z.c$(DependSuffix): src/physfs/physfs_archiver_7z.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_physfs_physfs_archiver_7z.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_physfs_physfs_archiver_7z.c$(DependSuffix) -MM src/physfs/physfs_archiver_7z.c

$(IntermediateDirectory)/src_physfs_physfs_archiver_7z.c$(PreprocessSuffix): src/physfs/physfs_archiver_7z.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_physfs_physfs_archiver_7z.c$(PreprocessSuffix) src/physfs/physfs_archiver_7z.c

$(IntermediateDirectory)/src_bsp_s_loadBsp.cpp$(ObjectSuffix): src/bsp/s_loadBsp.cpp $(IntermediateDirectory)/src_bsp_s_loadBsp.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/bsp/s_loadBsp.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_bsp_s_loadBsp.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_bsp_s_loadBsp.cpp$(DependSuffix): src/bsp/s_loadBsp.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_bsp_s_loadBsp.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_bsp_s_loadBsp.cpp$(DependSuffix) -MM src/bsp/s_loadBsp.cpp

$(IntermediateDirectory)/src_bsp_s_loadBsp.cpp$(PreprocessSuffix): src/bsp/s_loadBsp.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_bsp_s_loadBsp.cpp$(PreprocessSuffix) src/bsp/s_loadBsp.cpp

$(IntermediateDirectory)/src_script_as_generic.cpp$(ObjectSuffix): src/script/as_generic.cpp $(IntermediateDirectory)/src_script_as_generic.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_generic.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_generic.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_generic.cpp$(DependSuffix): src/script/as_generic.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_generic.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_generic.cpp$(DependSuffix) -MM src/script/as_generic.cpp

$(IntermediateDirectory)/src_script_as_generic.cpp$(PreprocessSuffix): src/script/as_generic.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_generic.cpp$(PreprocessSuffix) src/script/as_generic.cpp

$(IntermediateDirectory)/src_io_io_keyboard.cpp$(ObjectSuffix): src/io/io_keyboard.cpp $(IntermediateDirectory)/src_io_io_keyboard.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/io/io_keyboard.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_io_io_keyboard.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_io_io_keyboard.cpp$(DependSuffix): src/io/io_keyboard.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_io_io_keyboard.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_io_io_keyboard.cpp$(DependSuffix) -MM src/io/io_keyboard.cpp

$(IntermediateDirectory)/src_io_io_keyboard.cpp$(PreprocessSuffix): src/io/io_keyboard.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_io_io_keyboard.cpp$(PreprocessSuffix) src/io/io_keyboard.cpp

$(IntermediateDirectory)/src_bsp_s_doorsBSP.cpp$(ObjectSuffix): src/bsp/s_doorsBSP.cpp $(IntermediateDirectory)/src_bsp_s_doorsBSP.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/bsp/s_doorsBSP.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_bsp_s_doorsBSP.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_bsp_s_doorsBSP.cpp$(DependSuffix): src/bsp/s_doorsBSP.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_bsp_s_doorsBSP.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_bsp_s_doorsBSP.cpp$(DependSuffix) -MM src/bsp/s_doorsBSP.cpp

$(IntermediateDirectory)/src_bsp_s_doorsBSP.cpp$(PreprocessSuffix): src/bsp/s_doorsBSP.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_bsp_s_doorsBSP.cpp$(PreprocessSuffix) src/bsp/s_doorsBSP.cpp

$(IntermediateDirectory)/src_bsp_s_shaderLights.cpp$(ObjectSuffix): src/bsp/s_shaderLights.cpp $(IntermediateDirectory)/src_bsp_s_shaderLights.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/bsp/s_shaderLights.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_bsp_s_shaderLights.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_bsp_s_shaderLights.cpp$(DependSuffix): src/bsp/s_shaderLights.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_bsp_s_shaderLights.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_bsp_s_shaderLights.cpp$(DependSuffix) -MM src/bsp/s_shaderLights.cpp

$(IntermediateDirectory)/src_bsp_s_shaderLights.cpp$(PreprocessSuffix): src/bsp/s_shaderLights.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_bsp_s_shaderLights.cpp$(PreprocessSuffix) src/bsp/s_shaderLights.cpp

$(IntermediateDirectory)/src_script_as_parser.cpp$(ObjectSuffix): src/script/as_parser.cpp $(IntermediateDirectory)/src_script_as_parser.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_parser.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_parser.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_parser.cpp$(DependSuffix): src/script/as_parser.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_parser.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_parser.cpp$(DependSuffix) -MM src/script/as_parser.cpp

$(IntermediateDirectory)/src_script_as_parser.cpp$(PreprocessSuffix): src/script/as_parser.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_parser.cpp$(PreprocessSuffix) src/script/as_parser.cpp

$(IntermediateDirectory)/src_game_s_bullet.cpp$(ObjectSuffix): src/game/s_bullet.cpp $(IntermediateDirectory)/src_game_s_bullet.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/game/s_bullet.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_game_s_bullet.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_game_s_bullet.cpp$(DependSuffix): src/game/s_bullet.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_game_s_bullet.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_game_s_bullet.cpp$(DependSuffix) -MM src/game/s_bullet.cpp

$(IntermediateDirectory)/src_game_s_bullet.cpp$(PreprocessSuffix): src/game/s_bullet.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_game_s_bullet.cpp$(PreprocessSuffix) src/game/s_bullet.cpp

$(IntermediateDirectory)/src_console_s_comOpenGLInfo.cpp$(ObjectSuffix): src/console/s_comOpenGLInfo.cpp $(IntermediateDirectory)/src_console_s_comOpenGLInfo.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/console/s_comOpenGLInfo.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_console_s_comOpenGLInfo.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_console_s_comOpenGLInfo.cpp$(DependSuffix): src/console/s_comOpenGLInfo.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_console_s_comOpenGLInfo.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_console_s_comOpenGLInfo.cpp$(DependSuffix) -MM src/console/s_comOpenGLInfo.cpp

$(IntermediateDirectory)/src_console_s_comOpenGLInfo.cpp$(PreprocessSuffix): src/console/s_comOpenGLInfo.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_console_s_comOpenGLInfo.cpp$(PreprocessSuffix) src/console/s_comOpenGLInfo.cpp

$(IntermediateDirectory)/src_openGL_s_defRender.cpp$(ObjectSuffix): src/openGL/s_defRender.cpp $(IntermediateDirectory)/src_openGL_s_defRender.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/openGL/s_defRender.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_openGL_s_defRender.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_openGL_s_defRender.cpp$(DependSuffix): src/openGL/s_defRender.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_openGL_s_defRender.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_openGL_s_defRender.cpp$(DependSuffix) -MM src/openGL/s_defRender.cpp

$(IntermediateDirectory)/src_openGL_s_defRender.cpp$(PreprocessSuffix): src/openGL/s_defRender.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_openGL_s_defRender.cpp$(PreprocessSuffix) src/openGL/s_defRender.cpp

$(IntermediateDirectory)/src_script_as_callfunc_mips.cpp$(ObjectSuffix): src/script/as_callfunc_mips.cpp $(IntermediateDirectory)/src_script_as_callfunc_mips.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_callfunc_mips.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_callfunc_mips.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_callfunc_mips.cpp$(DependSuffix): src/script/as_callfunc_mips.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_callfunc_mips.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_callfunc_mips.cpp$(DependSuffix) -MM src/script/as_callfunc_mips.cpp

$(IntermediateDirectory)/src_script_as_callfunc_mips.cpp$(PreprocessSuffix): src/script/as_callfunc_mips.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_callfunc_mips.cpp$(PreprocessSuffix) src/script/as_callfunc_mips.cpp

$(IntermediateDirectory)/src_openGL_s_lightPass.cpp$(ObjectSuffix): src/openGL/s_lightPass.cpp $(IntermediateDirectory)/src_openGL_s_lightPass.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/openGL/s_lightPass.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_openGL_s_lightPass.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_openGL_s_lightPass.cpp$(DependSuffix): src/openGL/s_lightPass.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_openGL_s_lightPass.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_openGL_s_lightPass.cpp$(DependSuffix) -MM src/openGL/s_lightPass.cpp

$(IntermediateDirectory)/src_openGL_s_lightPass.cpp$(PreprocessSuffix): src/openGL/s_lightPass.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_openGL_s_lightPass.cpp$(PreprocessSuffix) src/openGL/s_lightPass.cpp

$(IntermediateDirectory)/src_openGL_s_shadowMap.cpp$(ObjectSuffix): src/openGL/s_shadowMap.cpp $(IntermediateDirectory)/src_openGL_s_shadowMap.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/openGL/s_shadowMap.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_openGL_s_shadowMap.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_openGL_s_shadowMap.cpp$(DependSuffix): src/openGL/s_shadowMap.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_openGL_s_shadowMap.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_openGL_s_shadowMap.cpp$(DependSuffix) -MM src/openGL/s_shadowMap.cpp

$(IntermediateDirectory)/src_openGL_s_shadowMap.cpp$(PreprocessSuffix): src/openGL/s_shadowMap.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_openGL_s_shadowMap.cpp$(PreprocessSuffix) src/openGL/s_shadowMap.cpp

$(IntermediateDirectory)/src_libGL_s_physicsDebug.cpp$(ObjectSuffix): src/libGL/s_physicsDebug.cpp $(IntermediateDirectory)/src_libGL_s_physicsDebug.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/libGL/s_physicsDebug.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_libGL_s_physicsDebug.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_libGL_s_physicsDebug.cpp$(DependSuffix): src/libGL/s_physicsDebug.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_libGL_s_physicsDebug.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_libGL_s_physicsDebug.cpp$(DependSuffix) -MM src/libGL/s_physicsDebug.cpp

$(IntermediateDirectory)/src_libGL_s_physicsDebug.cpp$(PreprocessSuffix): src/libGL/s_physicsDebug.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_libGL_s_physicsDebug.cpp$(PreprocessSuffix) src/libGL/s_physicsDebug.cpp

$(IntermediateDirectory)/src_script_as_callfunc_ppc.cpp$(ObjectSuffix): src/script/as_callfunc_ppc.cpp $(IntermediateDirectory)/src_script_as_callfunc_ppc.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_callfunc_ppc.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_callfunc_ppc.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_callfunc_ppc.cpp$(DependSuffix): src/script/as_callfunc_ppc.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_callfunc_ppc.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_callfunc_ppc.cpp$(DependSuffix) -MM src/script/as_callfunc_ppc.cpp

$(IntermediateDirectory)/src_script_as_callfunc_ppc.cpp$(PreprocessSuffix): src/script/as_callfunc_ppc.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_callfunc_ppc.cpp$(PreprocessSuffix) src/script/as_callfunc_ppc.cpp

$(IntermediateDirectory)/src_libGL_s_assimp.cpp$(ObjectSuffix): src/libGL/s_assimp.cpp $(IntermediateDirectory)/src_libGL_s_assimp.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/libGL/s_assimp.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_libGL_s_assimp.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_libGL_s_assimp.cpp$(DependSuffix): src/libGL/s_assimp.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_libGL_s_assimp.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_libGL_s_assimp.cpp$(DependSuffix) -MM src/libGL/s_assimp.cpp

$(IntermediateDirectory)/src_libGL_s_assimp.cpp$(PreprocessSuffix): src/libGL/s_assimp.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_libGL_s_assimp.cpp$(PreprocessSuffix) src/libGL/s_assimp.cpp

$(IntermediateDirectory)/src_libGL_s_physics.cpp$(ObjectSuffix): src/libGL/s_physics.cpp $(IntermediateDirectory)/src_libGL_s_physics.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/libGL/s_physics.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_libGL_s_physics.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_libGL_s_physics.cpp$(DependSuffix): src/libGL/s_physics.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_libGL_s_physics.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_libGL_s_physics.cpp$(DependSuffix) -MM src/libGL/s_physics.cpp

$(IntermediateDirectory)/src_libGL_s_physics.cpp$(PreprocessSuffix): src/libGL/s_physics.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_libGL_s_physics.cpp$(PreprocessSuffix) src/libGL/s_physics.cpp

$(IntermediateDirectory)/src_libGL_s_antBar.cpp$(ObjectSuffix): src/libGL/s_antBar.cpp $(IntermediateDirectory)/src_libGL_s_antBar.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/libGL/s_antBar.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_libGL_s_antBar.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_libGL_s_antBar.cpp$(DependSuffix): src/libGL/s_antBar.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_libGL_s_antBar.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_libGL_s_antBar.cpp$(DependSuffix) -MM src/libGL/s_antBar.cpp

$(IntermediateDirectory)/src_libGL_s_antBar.cpp$(PreprocessSuffix): src/libGL/s_antBar.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_libGL_s_antBar.cpp$(PreprocessSuffix) src/libGL/s_antBar.cpp

$(IntermediateDirectory)/src_libGL_s_vsProfileLib.cpp$(ObjectSuffix): src/libGL/s_vsProfileLib.cpp $(IntermediateDirectory)/src_libGL_s_vsProfileLib.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/libGL/s_vsProfileLib.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_libGL_s_vsProfileLib.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_libGL_s_vsProfileLib.cpp$(DependSuffix): src/libGL/s_vsProfileLib.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_libGL_s_vsProfileLib.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_libGL_s_vsProfileLib.cpp$(DependSuffix) -MM src/libGL/s_vsProfileLib.cpp

$(IntermediateDirectory)/src_libGL_s_vsProfileLib.cpp$(PreprocessSuffix): src/libGL/s_vsProfileLib.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_libGL_s_vsProfileLib.cpp$(PreprocessSuffix) src/libGL/s_vsProfileLib.cpp

$(IntermediateDirectory)/src_io_io_fileSystem.cpp$(ObjectSuffix): src/io/io_fileSystem.cpp $(IntermediateDirectory)/src_io_io_fileSystem.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/io/io_fileSystem.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_io_io_fileSystem.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_io_io_fileSystem.cpp$(DependSuffix): src/io/io_fileSystem.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_io_io_fileSystem.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_io_io_fileSystem.cpp$(DependSuffix) -MM src/io/io_fileSystem.cpp

$(IntermediateDirectory)/src_io_io_fileSystem.cpp$(PreprocessSuffix): src/io/io_fileSystem.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_io_io_fileSystem.cpp$(PreprocessSuffix) src/io/io_fileSystem.cpp

$(IntermediateDirectory)/src_script_as_scriptengine.cpp$(ObjectSuffix): src/script/as_scriptengine.cpp $(IntermediateDirectory)/src_script_as_scriptengine.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_scriptengine.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_scriptengine.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_scriptengine.cpp$(DependSuffix): src/script/as_scriptengine.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_scriptengine.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_scriptengine.cpp$(DependSuffix) -MM src/script/as_scriptengine.cpp

$(IntermediateDirectory)/src_script_as_scriptengine.cpp$(PreprocessSuffix): src/script/as_scriptengine.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_scriptengine.cpp$(PreprocessSuffix) src/script/as_scriptengine.cpp

$(IntermediateDirectory)/src_physfs_physfs_platform_posix.c$(ObjectSuffix): src/physfs/physfs_platform_posix.c $(IntermediateDirectory)/src_physfs_physfs_platform_posix.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/physfs/physfs_platform_posix.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_physfs_physfs_platform_posix.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_physfs_physfs_platform_posix.c$(DependSuffix): src/physfs/physfs_platform_posix.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_physfs_physfs_platform_posix.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_physfs_physfs_platform_posix.c$(DependSuffix) -MM src/physfs/physfs_platform_posix.c

$(IntermediateDirectory)/src_physfs_physfs_platform_posix.c$(PreprocessSuffix): src/physfs/physfs_platform_posix.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_physfs_physfs_platform_posix.c$(PreprocessSuffix) src/physfs/physfs_platform_posix.c

$(IntermediateDirectory)/src_io_io_loadImage.cpp$(ObjectSuffix): src/io/io_loadImage.cpp $(IntermediateDirectory)/src_io_io_loadImage.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/io/io_loadImage.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_io_io_loadImage.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_io_io_loadImage.cpp$(DependSuffix): src/io/io_loadImage.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_io_io_loadImage.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_io_io_loadImage.cpp$(DependSuffix) -MM src/io/io_loadImage.cpp

$(IntermediateDirectory)/src_io_io_loadImage.cpp$(PreprocessSuffix): src/io/io_loadImage.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_io_io_loadImage.cpp$(PreprocessSuffix) src/io/io_loadImage.cpp

$(IntermediateDirectory)/src_script_as_objecttype.cpp$(ObjectSuffix): src/script/as_objecttype.cpp $(IntermediateDirectory)/src_script_as_objecttype.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_objecttype.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_objecttype.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_objecttype.cpp$(DependSuffix): src/script/as_objecttype.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_objecttype.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_objecttype.cpp$(DependSuffix) -MM src/script/as_objecttype.cpp

$(IntermediateDirectory)/src_script_as_objecttype.cpp$(PreprocessSuffix): src/script/as_objecttype.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_objecttype.cpp$(PreprocessSuffix) src/script/as_objecttype.cpp

$(IntermediateDirectory)/src_script_as_callfunc_arm.cpp$(ObjectSuffix): src/script/as_callfunc_arm.cpp $(IntermediateDirectory)/src_script_as_callfunc_arm.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_callfunc_arm.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_callfunc_arm.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_callfunc_arm.cpp$(DependSuffix): src/script/as_callfunc_arm.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_callfunc_arm.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_callfunc_arm.cpp$(DependSuffix) -MM src/script/as_callfunc_arm.cpp

$(IntermediateDirectory)/src_script_as_callfunc_arm.cpp$(PreprocessSuffix): src/script/as_callfunc_arm.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_callfunc_arm.cpp$(PreprocessSuffix) src/script/as_callfunc_arm.cpp

$(IntermediateDirectory)/src_script_as_context.cpp$(ObjectSuffix): src/script/as_context.cpp $(IntermediateDirectory)/src_script_as_context.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_context.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_context.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_context.cpp$(DependSuffix): src/script/as_context.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_context.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_context.cpp$(DependSuffix) -MM src/script/as_context.cpp

$(IntermediateDirectory)/src_script_as_context.cpp$(PreprocessSuffix): src/script/as_context.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_context.cpp$(PreprocessSuffix) src/script/as_context.cpp

$(IntermediateDirectory)/src_script_as_callfunc_x64_mingw.cpp$(ObjectSuffix): src/script/as_callfunc_x64_mingw.cpp $(IntermediateDirectory)/src_script_as_callfunc_x64_mingw.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_callfunc_x64_mingw.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_callfunc_x64_mingw.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_callfunc_x64_mingw.cpp$(DependSuffix): src/script/as_callfunc_x64_mingw.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_callfunc_x64_mingw.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_callfunc_x64_mingw.cpp$(DependSuffix) -MM src/script/as_callfunc_x64_mingw.cpp

$(IntermediateDirectory)/src_script_as_callfunc_x64_mingw.cpp$(PreprocessSuffix): src/script/as_callfunc_x64_mingw.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_callfunc_x64_mingw.cpp$(PreprocessSuffix) src/script/as_callfunc_x64_mingw.cpp

$(IntermediateDirectory)/src_script_as_callfunc.cpp$(ObjectSuffix): src/script/as_callfunc.cpp $(IntermediateDirectory)/src_script_as_callfunc.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_callfunc.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_callfunc.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_callfunc.cpp$(DependSuffix): src/script/as_callfunc.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_callfunc.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_callfunc.cpp$(DependSuffix) -MM src/script/as_callfunc.cpp

$(IntermediateDirectory)/src_script_as_callfunc.cpp$(PreprocessSuffix): src/script/as_callfunc.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_callfunc.cpp$(PreprocessSuffix) src/script/as_callfunc.cpp

$(IntermediateDirectory)/src_script_as_atomic.cpp$(ObjectSuffix): src/script/as_atomic.cpp $(IntermediateDirectory)/src_script_as_atomic.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_atomic.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_atomic.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_atomic.cpp$(DependSuffix): src/script/as_atomic.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_atomic.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_atomic.cpp$(DependSuffix) -MM src/script/as_atomic.cpp

$(IntermediateDirectory)/src_script_as_atomic.cpp$(PreprocessSuffix): src/script/as_atomic.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_atomic.cpp$(PreprocessSuffix) src/script/as_atomic.cpp

$(IntermediateDirectory)/src_script_scriptstdstring.cpp$(ObjectSuffix): src/script/scriptstdstring.cpp $(IntermediateDirectory)/src_script_scriptstdstring.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/scriptstdstring.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_scriptstdstring.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_scriptstdstring.cpp$(DependSuffix): src/script/scriptstdstring.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_scriptstdstring.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_scriptstdstring.cpp$(DependSuffix) -MM src/script/scriptstdstring.cpp

$(IntermediateDirectory)/src_script_scriptstdstring.cpp$(PreprocessSuffix): src/script/scriptstdstring.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_scriptstdstring.cpp$(PreprocessSuffix) src/script/scriptstdstring.cpp

$(IntermediateDirectory)/src_script_as_configgroup.cpp$(ObjectSuffix): src/script/as_configgroup.cpp $(IntermediateDirectory)/src_script_as_configgroup.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_configgroup.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_configgroup.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_configgroup.cpp$(DependSuffix): src/script/as_configgroup.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_configgroup.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_configgroup.cpp$(DependSuffix) -MM src/script/as_configgroup.cpp

$(IntermediateDirectory)/src_script_as_configgroup.cpp$(PreprocessSuffix): src/script/as_configgroup.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_configgroup.cpp$(PreprocessSuffix) src/script/as_configgroup.cpp

$(IntermediateDirectory)/src_script_as_scriptcode.cpp$(ObjectSuffix): src/script/as_scriptcode.cpp $(IntermediateDirectory)/src_script_as_scriptcode.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_scriptcode.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_scriptcode.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_scriptcode.cpp$(DependSuffix): src/script/as_scriptcode.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_scriptcode.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_scriptcode.cpp$(DependSuffix) -MM src/script/as_scriptcode.cpp

$(IntermediateDirectory)/src_script_as_scriptcode.cpp$(PreprocessSuffix): src/script/as_scriptcode.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_scriptcode.cpp$(PreprocessSuffix) src/script/as_scriptcode.cpp

$(IntermediateDirectory)/src_script_as_memory.cpp$(ObjectSuffix): src/script/as_memory.cpp $(IntermediateDirectory)/src_script_as_memory.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_memory.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_memory.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_memory.cpp$(DependSuffix): src/script/as_memory.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_memory.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_memory.cpp$(DependSuffix) -MM src/script/as_memory.cpp

$(IntermediateDirectory)/src_script_as_memory.cpp$(PreprocessSuffix): src/script/as_memory.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_memory.cpp$(PreprocessSuffix) src/script/as_memory.cpp

$(IntermediateDirectory)/src_script_as_tokenizer.cpp$(ObjectSuffix): src/script/as_tokenizer.cpp $(IntermediateDirectory)/src_script_as_tokenizer.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_tokenizer.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_tokenizer.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_tokenizer.cpp$(DependSuffix): src/script/as_tokenizer.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_tokenizer.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_tokenizer.cpp$(DependSuffix) -MM src/script/as_tokenizer.cpp

$(IntermediateDirectory)/src_script_as_tokenizer.cpp$(PreprocessSuffix): src/script/as_tokenizer.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_tokenizer.cpp$(PreprocessSuffix) src/script/as_tokenizer.cpp

$(IntermediateDirectory)/src_script_as_scriptfunction.cpp$(ObjectSuffix): src/script/as_scriptfunction.cpp $(IntermediateDirectory)/src_script_as_scriptfunction.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_scriptfunction.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_scriptfunction.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_scriptfunction.cpp$(DependSuffix): src/script/as_scriptfunction.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_scriptfunction.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_scriptfunction.cpp$(DependSuffix) -MM src/script/as_scriptfunction.cpp

$(IntermediateDirectory)/src_script_as_scriptfunction.cpp$(PreprocessSuffix): src/script/as_scriptfunction.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_scriptfunction.cpp$(PreprocessSuffix) src/script/as_scriptfunction.cpp

$(IntermediateDirectory)/src_script_as_bytecode.cpp$(ObjectSuffix): src/script/as_bytecode.cpp $(IntermediateDirectory)/src_script_as_bytecode.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_bytecode.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_bytecode.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_bytecode.cpp$(DependSuffix): src/script/as_bytecode.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_bytecode.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_bytecode.cpp$(DependSuffix) -MM src/script/as_bytecode.cpp

$(IntermediateDirectory)/src_script_as_bytecode.cpp$(PreprocessSuffix): src/script/as_bytecode.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_bytecode.cpp$(PreprocessSuffix) src/script/as_bytecode.cpp

$(IntermediateDirectory)/src_script_as_variablescope.cpp$(ObjectSuffix): src/script/as_variablescope.cpp $(IntermediateDirectory)/src_script_as_variablescope.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_variablescope.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_variablescope.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_variablescope.cpp$(DependSuffix): src/script/as_variablescope.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_variablescope.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_variablescope.cpp$(DependSuffix) -MM src/script/as_variablescope.cpp

$(IntermediateDirectory)/src_script_as_variablescope.cpp$(PreprocessSuffix): src/script/as_variablescope.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_variablescope.cpp$(PreprocessSuffix) src/script/as_variablescope.cpp

$(IntermediateDirectory)/src_system_s_startup.cpp$(ObjectSuffix): src/system/s_startup.cpp $(IntermediateDirectory)/src_system_s_startup.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/system/s_startup.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_system_s_startup.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_system_s_startup.cpp$(DependSuffix): src/system/s_startup.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_system_s_startup.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_system_s_startup.cpp$(DependSuffix) -MM src/system/s_startup.cpp

$(IntermediateDirectory)/src_system_s_startup.cpp$(PreprocessSuffix): src/system/s_startup.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_system_s_startup.cpp$(PreprocessSuffix) src/system/s_startup.cpp

$(IntermediateDirectory)/src_script_as_builder.cpp$(ObjectSuffix): src/script/as_builder.cpp $(IntermediateDirectory)/src_script_as_builder.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_builder.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_builder.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_builder.cpp$(DependSuffix): src/script/as_builder.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_builder.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_builder.cpp$(DependSuffix) -MM src/script/as_builder.cpp

$(IntermediateDirectory)/src_script_as_builder.cpp$(PreprocessSuffix): src/script/as_builder.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_builder.cpp$(PreprocessSuffix) src/script/as_builder.cpp

$(IntermediateDirectory)/src_script_as_callfunc_x64_gcc.cpp$(ObjectSuffix): src/script/as_callfunc_x64_gcc.cpp $(IntermediateDirectory)/src_script_as_callfunc_x64_gcc.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_callfunc_x64_gcc.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_callfunc_x64_gcc.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_callfunc_x64_gcc.cpp$(DependSuffix): src/script/as_callfunc_x64_gcc.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_callfunc_x64_gcc.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_callfunc_x64_gcc.cpp$(DependSuffix) -MM src/script/as_callfunc_x64_gcc.cpp

$(IntermediateDirectory)/src_script_as_callfunc_x64_gcc.cpp$(PreprocessSuffix): src/script/as_callfunc_x64_gcc.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_callfunc_x64_gcc.cpp$(PreprocessSuffix) src/script/as_callfunc_x64_gcc.cpp

$(IntermediateDirectory)/src_script_as_scriptnode.cpp$(ObjectSuffix): src/script/as_scriptnode.cpp $(IntermediateDirectory)/src_script_as_scriptnode.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_scriptnode.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_scriptnode.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_scriptnode.cpp$(DependSuffix): src/script/as_scriptnode.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_scriptnode.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_scriptnode.cpp$(DependSuffix) -MM src/script/as_scriptnode.cpp

$(IntermediateDirectory)/src_script_as_scriptnode.cpp$(PreprocessSuffix): src/script/as_scriptnode.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_scriptnode.cpp$(PreprocessSuffix) src/script/as_scriptnode.cpp

$(IntermediateDirectory)/src_script_as_gc.cpp$(ObjectSuffix): src/script/as_gc.cpp $(IntermediateDirectory)/src_script_as_gc.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_gc.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_gc.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_gc.cpp$(DependSuffix): src/script/as_gc.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_gc.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_gc.cpp$(DependSuffix) -MM src/script/as_gc.cpp

$(IntermediateDirectory)/src_script_as_gc.cpp$(PreprocessSuffix): src/script/as_gc.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_gc.cpp$(PreprocessSuffix) src/script/as_gc.cpp

$(IntermediateDirectory)/src_system_s_main.cpp$(ObjectSuffix): src/system/s_main.cpp $(IntermediateDirectory)/src_system_s_main.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/system/s_main.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_system_s_main.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_system_s_main.cpp$(DependSuffix): src/system/s_main.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_system_s_main.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_system_s_main.cpp$(DependSuffix) -MM src/system/s_main.cpp

$(IntermediateDirectory)/src_system_s_main.cpp$(PreprocessSuffix): src/system/s_main.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_system_s_main.cpp$(PreprocessSuffix) src/system/s_main.cpp

$(IntermediateDirectory)/src_system_s_camera.cpp$(ObjectSuffix): src/system/s_camera.cpp $(IntermediateDirectory)/src_system_s_camera.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/system/s_camera.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_system_s_camera.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_system_s_camera.cpp$(DependSuffix): src/system/s_camera.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_system_s_camera.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_system_s_camera.cpp$(DependSuffix) -MM src/system/s_camera.cpp

$(IntermediateDirectory)/src_system_s_camera.cpp$(PreprocessSuffix): src/system/s_camera.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_system_s_camera.cpp$(PreprocessSuffix) src/system/s_camera.cpp

$(IntermediateDirectory)/src_system_s_globals.cpp$(ObjectSuffix): src/system/s_globals.cpp $(IntermediateDirectory)/src_system_s_globals.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/system/s_globals.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_system_s_globals.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_system_s_globals.cpp$(DependSuffix): src/system/s_globals.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_system_s_globals.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_system_s_globals.cpp$(DependSuffix) -MM src/system/s_globals.cpp

$(IntermediateDirectory)/src_system_s_globals.cpp$(PreprocessSuffix): src/system/s_globals.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_system_s_globals.cpp$(PreprocessSuffix) src/system/s_globals.cpp

$(IntermediateDirectory)/src_system_s_render.cpp$(ObjectSuffix): src/system/s_render.cpp $(IntermediateDirectory)/src_system_s_render.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/system/s_render.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_system_s_render.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_system_s_render.cpp$(DependSuffix): src/system/s_render.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_system_s_render.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_system_s_render.cpp$(DependSuffix) -MM src/system/s_render.cpp

$(IntermediateDirectory)/src_system_s_render.cpp$(PreprocessSuffix): src/system/s_render.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_system_s_render.cpp$(PreprocessSuffix) src/system/s_render.cpp

$(IntermediateDirectory)/src_physfs_physfs_archiver_dir.c$(ObjectSuffix): src/physfs/physfs_archiver_dir.c $(IntermediateDirectory)/src_physfs_physfs_archiver_dir.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/physfs/physfs_archiver_dir.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_physfs_physfs_archiver_dir.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_physfs_physfs_archiver_dir.c$(DependSuffix): src/physfs/physfs_archiver_dir.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_physfs_physfs_archiver_dir.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_physfs_physfs_archiver_dir.c$(DependSuffix) -MM src/physfs/physfs_archiver_dir.c

$(IntermediateDirectory)/src_physfs_physfs_archiver_dir.c$(PreprocessSuffix): src/physfs/physfs_archiver_dir.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_physfs_physfs_archiver_dir.c$(PreprocessSuffix) src/physfs/physfs_archiver_dir.c

$(IntermediateDirectory)/src_physfs_physfs_platform_os2.c$(ObjectSuffix): src/physfs/physfs_platform_os2.c $(IntermediateDirectory)/src_physfs_physfs_platform_os2.c$(DependSuffix)
	$(CC) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/physfs/physfs_platform_os2.c" $(CFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_physfs_physfs_platform_os2.c$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_physfs_physfs_platform_os2.c$(DependSuffix): src/physfs/physfs_platform_os2.c
	@$(CC) $(CFLAGS) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_physfs_physfs_platform_os2.c$(ObjectSuffix) -MF$(IntermediateDirectory)/src_physfs_physfs_platform_os2.c$(DependSuffix) -MM src/physfs/physfs_platform_os2.c

$(IntermediateDirectory)/src_physfs_physfs_platform_os2.c$(PreprocessSuffix): src/physfs/physfs_platform_os2.c
	$(CC) $(CFLAGS) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_physfs_physfs_platform_os2.c$(PreprocessSuffix) src/physfs/physfs_platform_os2.c

$(IntermediateDirectory)/src_bsp_s_collideBSP.cpp$(ObjectSuffix): src/bsp/s_collideBSP.cpp $(IntermediateDirectory)/src_bsp_s_collideBSP.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/bsp/s_collideBSP.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_bsp_s_collideBSP.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_bsp_s_collideBSP.cpp$(DependSuffix): src/bsp/s_collideBSP.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_bsp_s_collideBSP.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_bsp_s_collideBSP.cpp$(DependSuffix) -MM src/bsp/s_collideBSP.cpp

$(IntermediateDirectory)/src_bsp_s_collideBSP.cpp$(PreprocessSuffix): src/bsp/s_collideBSP.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_bsp_s_collideBSP.cpp$(PreprocessSuffix) src/bsp/s_collideBSP.cpp

$(IntermediateDirectory)/src_script_as_thread.cpp$(ObjectSuffix): src/script/as_thread.cpp $(IntermediateDirectory)/src_script_as_thread.cpp$(DependSuffix)
	$(CXX) $(IncludePCH) $(SourceSwitch) "/home/dberry/Documents/Devel/1_Current/GL3/Current/Droid_3D/src/script/as_thread.cpp" $(CXXFLAGS) $(ObjectSwitch)$(IntermediateDirectory)/src_script_as_thread.cpp$(ObjectSuffix) $(IncludePath)
$(IntermediateDirectory)/src_script_as_thread.cpp$(DependSuffix): src/script/as_thread.cpp
	@$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) -MG -MP -MT$(IntermediateDirectory)/src_script_as_thread.cpp$(ObjectSuffix) -MF$(IntermediateDirectory)/src_script_as_thread.cpp$(DependSuffix) -MM src/script/as_thread.cpp

$(IntermediateDirectory)/src_script_as_thread.cpp$(PreprocessSuffix): src/script/as_thread.cpp
	$(CXX) $(CXXFLAGS) $(IncludePCH) $(IncludePath) $(PreprocessOnlySwitch) $(OutputSwitch) $(IntermediateDirectory)/src_script_as_thread.cpp$(PreprocessSuffix) src/script/as_thread.cpp


-include $(IntermediateDirectory)/*$(DependSuffix)
##
## Clean
##
clean:
	$(RM) -r ./Debug/


