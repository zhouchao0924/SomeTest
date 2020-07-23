#include "Settings/SimpleVCSettings.h"

USimpleVCSettings::USimpleVCSettings()
	:bAuto(true)
	,DataServerType(EUploadServerType::OSSServer)
	,DLCVersionListName("Version/DLCList.con")
	,PatchVersionListName("Version/VersionList.con")
	,ServerVersionName("Version/ServerVersion.con")
	,DLCVersionPaths("Version/History/DLCVersion")
	,PatchVersionPaths("Version/History/PatchVersion")
{

}

