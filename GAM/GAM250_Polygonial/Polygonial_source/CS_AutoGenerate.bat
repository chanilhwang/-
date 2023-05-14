::FileName: 		CS_AutoGenerate.bat
::author:   	    junwoo.seo
::digipen-id:   	junwoo.seo
::e-mail:   		seojunwoo72@gmail.com
@echo off
setlocal
SET CS_Engine=CS_Engine
SET CS_Game=CS_Game
call:CS_AddSounds
call:CS_SoundTypes
call:CS_StageTypes
call:CS_AddStages
call:CS_ComponentTypes
call:CS_AddComponents
call:CS_ArcheTypes
call::CS_AddArcheTypes
call::CS_ResourceLists
goto:eof
:CS_AddSounds

SET SOURCE=%CS_Engine%\CS_AddSounds.cpp
SET TOFIND=%CS_Game%\Sounds\*.mp3
nul > %SOURCE%
echo #include "CS_AddSounds.hpp" >> %SOURCE%
echo #include "CS_SoundTypes.hpp" >> %SOURCE%
echo #include "CS_SoundManager.hpp" >> %SOURCE%
echo void CS_AddSounds(void) {  >> %SOURCE%
::Get all files with the name *Stage in it and output just the file name
for %%f in (%TOFIND%) do (
  echo CS_SoundManager::RegisterSound^(CS_SoundTypes::%%~nf, "Sounds//%%~nf.mp3"^); >> %SOURCE%
)
echo } >> %SOURCE%

goto:eof
:CS_SoundTypes

SET ENUMFILE=%CS_Engine%\CS_SoundTypes.hpp
SET TOFIND=%CS_Game%\Sounds\*.mp3
nul > %ENUMFILE%
echo #pragma once >> %ENUMFILE%
echo #include ^<string^> >> %ENUMFILE%
echo. >> %ENUMFILE%
echo. >> %ENUMFILE%
echo enum class CS_SoundTypes {  >> %ENUMFILE%
for %%f in (%TOFIND%) do  (
  echo %%~nf, >> %ENUMFILE%
)
echo INVALID_Sound >> %ENUMFILE%
echo }; >> %ENUMFILE%

echo. >> %ENUMFILE%
echo. >> %ENUMFILE%

echo inline CS_SoundTypes StringToSound^(const std::string^& string^) { >> %ENUMFILE%

for %%f in (%TOFIND%) do (
  echo if^(string ^=^= "%%~nf"^) return CS_SoundTypes::%%~nf; >> %ENUMFILE%
)
echo return CS_SoundTypes::INVALID_Sound; >> %ENUMFILE%
echo } >> %ENUMFILE%

echo inline std::string SoundToString^(const CS_SoundTypes type^) { >> %ENUMFILE%
for %%f in (%TOFIND%) do (
  echo if^(type ^=^= CS_SoundTypes::%%~nf^) return "%%~nf"; >> %ENUMFILE%
)

echo return "INVALID_Sound"; >> %ENUMFILE%
echo } >> %ENUMFILE%


goto:eof
:CS_ResourceLists
SET HEADER=%CS_Engine%\CS_ResourceLists.hpp
SET SOURCE=%CS_Engine%\CS_ResourceLists.cpp
SET TOFIND=%CS_Game%\ArcheTypes\*.json
SET COUNT=0
for %%f in (%TOFIND%) do (
  SET /a COUNT+=1
)
nul > %HEADER%
echo #pragma once >> %HEADER%
echo extern const char* CS_ArcheTypeList[]; >> %HEADER%
echo extern int CS_ArcheTypeListSize; >> %HEADER%

nul > %SOURCE%
echo #include "CS_ResourceLists.hpp" >> %SOURCE%
echo const char* CS_ArcheTypeList[]={ >>%SOURCE%
for %%f in (%TOFIND%) do (
  echo "%%~nf", >> %SOURCE%
)
echo };>>%SOURCE%
echo int CS_ArcheTypeListSize=sizeof(CS_ArcheTypeList)/sizeof(const char*); >> %SOURCE%


echo extern const char* CS_ComponentTypeList[]; >> %HEADER%
echo extern int CS_ComponentTypeListSize; >> %HEADER%


SET TOFIND=%CS_Engine%\*_Component.hpp
SET TOFIND2=%CS_Game%\*_Component.hpp
echo const char* CS_ComponentTypeList[]={ >>%SOURCE%
for %%f in (%TOFIND%) do  (
  echo "%%~nf", >> %SOURCE%
)
for %%f in (%TOFIND2%) do (
  echo "%%~nf", >> %SOURCE%
)
echo };>>%SOURCE%
echo int CS_ComponentTypeListSize=sizeof(CS_ComponentTypeList)/sizeof(const char*); >> %SOURCE%

echo extern const char* CS_TextureList[]; >> %HEADER%
echo extern int CS_TextureListSize; >> %HEADER%

SET TOFIND=%CS_Game%\Textures\*.png
echo const char* CS_TextureList[]={ >>%SOURCE%
for %%f in (%TOFIND%) do  (
  echo "%%~nf.png", >> %SOURCE%
)
echo };>>%SOURCE%
echo int CS_TextureListSize=sizeof(CS_TextureList)/sizeof(const char*); >> %SOURCE%

goto:eof


:CS_StageTypes
SET ENUMFILE=%CS_Engine%\CS_StageTypes.hpp
SET TOFIND=%CS_Game%\*_Stage.hpp
nul > %ENUMFILE%
echo #pragma once >> %ENUMFILE%
echo #include ^<string^> >> %ENUMFILE%
echo. >> %ENUMFILE%
echo. >> %ENUMFILE%
echo enum class CS_StageTypes {  >> %ENUMFILE%
for %%f in (%TOFIND%) do (
  echo %%~nf, >> %ENUMFILE%
)
echo INVALID_Stage >> %ENUMFILE%
echo }; >> %ENUMFILE%

echo. >> %ENUMFILE%
echo. >> %ENUMFILE%

echo inline CS_StageTypes StringToStage^(const std::string^& string^) { >> %ENUMFILE%

for %%f in (%TOFIND%) do (
  echo if^(string ^=^= "%%~nf"^) return CS_StageTypes::%%~nf; >> %ENUMFILE%
)
echo return CS_StageTypes::INVALID_Stage; >> %ENUMFILE%
echo } >> %ENUMFILE%


goto:eof

:CS_AddStages
SET SOURCE=%CS_Engine%\CS_AddStages.cpp
SET TOFIND=%CS_Game%\*_Stage.hpp
nul > %SOURCE%
echo #include "CS_AddStages.hpp" >> %SOURCE%
echo #include "CS_StageManager.hpp" >> %SOURCE%
echo #include "CS_StageTypes.hpp" >> %SOURCE%
echo #include "CS_StageBuilder.hpp" >> %SOURCE%
::Include all Stage header files
for %%f in (%TOFIND%) do (
  echo #include "%%~nf.hpp" >> %SOURCE%
)
echo. >> %SOURCE%
echo. >> %SOURCE%

echo void CS_AddStages(void) {  >> %SOURCE%
::Get all files with the name *Stage in it and output just the file name
for %%f in (%TOFIND%) do (
  echo CS_StageManager::AddStage^(CS_StageTypes::%%~nf, new CS_TemplatedStageBuilder^< %%~nf ^>^(^) ^); >> %SOURCE%
)
echo } >> %SOURCE%

goto:eof
:CS_ComponentTypes
SET ENUMFILE=%CS_Engine%\CS_ComponentTypes.hpp
SET SKIP=CS_Component.hpp
SET TOFIND=%CS_Engine%\*_Component.hpp
SET TOFIND2=%CS_Game%\*_Component.hpp
nul > %ENUMFILE%
echo #pragma once >> %ENUMFILE%
echo #include ^<string^> >> %ENUMFILE%
echo. >> %ENUMFILE%
echo. >> %ENUMFILE%
echo enum class CS_ComponentTypes {  >> %ENUMFILE%
for %%f in (%TOFIND%) do  (
  echo %%~nf, >> %ENUMFILE%
)
for %%f in (%TOFIND2%) do (
  echo %%~nf, >> %ENUMFILE%
)
echo INVALID_Component >> %ENUMFILE%
echo }; >> %ENUMFILE%

echo. >> %ENUMFILE%
echo. >> %ENUMFILE%

echo inline CS_ComponentTypes StringToComponent^(const std::string^& string^) { >> %ENUMFILE%

for %%f in (%TOFIND%) do (
  echo if^(string ^=^= "%%~nf"^) return CS_ComponentTypes::%%~nf; >> %ENUMFILE%
)
for %%f in (%TOFIND2%) do (
  echo if^(string ^=^= "%%~nf"^) return CS_ComponentTypes::%%~nf; >> %ENUMFILE%
)
echo return CS_ComponentTypes::INVALID_Component; >> %ENUMFILE%
echo } >> %ENUMFILE%

echo inline std::string ComponentToString^(const CS_ComponentTypes type^) { >> %ENUMFILE%
for %%f in (%TOFIND%) do (
  echo if^(type ^=^= CS_ComponentTypes::%%~nf^) return "%%~nf"; >> %ENUMFILE%
)
for %%f in (%TOFIND2%) do (
  echo if^(type ^=^= CS_ComponentTypes::%%~nf^) return "%%~nf"; >> %ENUMFILE%
)
echo return "INVALID_Component"; >> %ENUMFILE%
echo } >> %ENUMFILE%
goto:eof

:CS_AddComponents
SET SOURCE=%CS_Engine%\CS_AddComponents.cpp
SET TOFIND=%CS_Engine%\*_Component.hpp
SET TOFIND2=%CS_Game%\*_Component.hpp
nul > %SOURCE%
echo #include "CS_AddComponents.hpp" >> %SOURCE%
echo #include "CS_ObjectManager.hpp" >> %SOURCE%
echo #include "CS_ComponentTypes.hpp" >> %SOURCE%
echo #include "CS_ComponentBuilder.hpp" >> %SOURCE%
::Include all Stage header files
for %%f in (%TOFIND%) do (
  echo #include "%%~nf.hpp" >> %SOURCE%
)
for %%f in (%TOFIND2%) do (
  echo #include "%%~nf.hpp" >> %SOURCE%
)
echo. >> %SOURCE%
echo. >> %SOURCE%

echo void CS_AddComponents(void) {  >> %SOURCE%
::Get all files with the name *Stage in it and output just the file name
for %%f in (%TOFIND%) do (
  echo CS_ObjectManager::AddComponent^(CS_ComponentTypes::%%~nf, new CS_TemplatedComponentBuilder^< %%~nf ^>^(^) ^); >> %SOURCE%
)
for %%f in (%TOFIND2%) do (
  echo CS_ObjectManager::AddComponent^(CS_ComponentTypes::%%~nf, new CS_TemplatedComponentBuilder^< %%~nf ^>^(^) ^); >> %SOURCE%
)
echo } >> %SOURCE%
goto:eof

:CS_ArcheTypes
SET ENUMFILE=%CS_Engine%\CS_ArcheTypes.hpp
SET TOFIND=%CS_Game%\ArcheTypes\*.json
nul > %ENUMFILE%
echo #pragma once >> %ENUMFILE%
echo #include ^<string^> >> %ENUMFILE%
echo. >> %ENUMFILE%
echo. >> %ENUMFILE%
echo enum class CS_ArcheTypes {  >> %ENUMFILE%
for %%f in (%TOFIND%) do  (
  echo %%~nf, >> %ENUMFILE%
)
echo INVALID_ArcheType >> %ENUMFILE%
echo }; >> %ENUMFILE%

echo. >> %ENUMFILE%
echo. >> %ENUMFILE%

echo inline CS_ArcheTypes StringToArcheType^(const std::string^& string^) { >> %ENUMFILE%

for %%f in (%TOFIND%) do (
  echo if^(string ^=^= "%%~nf"^) return CS_ArcheTypes::%%~nf; >> %ENUMFILE%
)
echo return CS_ArcheTypes::INVALID_ArcheType; >> %ENUMFILE%
echo } >> %ENUMFILE%

echo inline std::string ArcheTypeToString^(const CS_ArcheTypes type^) { >> %ENUMFILE%
for %%f in (%TOFIND%) do (
  echo if^(type ^=^= CS_ArcheTypes::%%~nf^) return "%%~nf"; >> %ENUMFILE%
)
for %%f in (%TOFIND%) do (
  echo if^(type ^=^= CS_ArcheTypes::%%~nf^) return "%%~nf"; >> %ENUMFILE%
)
echo return "INVALID_ArcheType"; >> %ENUMFILE%
echo } >> %ENUMFILE%
goto:eof

:CS_AddArcheTypes
SET SOURCE=%CS_Engine%\CS_AddArcheTypes.cpp
SET TOFIND=%CS_Game%\ArcheTypes\*.json
nul > %SOURCE%
echo #include "CS_ObjectManager.hpp" >> %SOURCE%
echo #include "CS_AddArcheTypes.hpp" >> %SOURCE%
echo. >> %SOURCE%
echo. >> %SOURCE%
echo void CS_AddArcheTypes(void) {  >> %SOURCE%
::Get all files with the name *Stage in it and output just the file name
for %%f in (%TOFIND%) do (
  echo CS_ObjectManager::AddArcheType^(CS_ArcheTypes::%%~nf, "ArcheTypes\\%%~nf.json"^); >> %SOURCE%
)
echo } >> %SOURCE%
goto:eof