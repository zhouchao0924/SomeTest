#include "FileTree/FileParsing.h"

SimpleUnrealPakView::EFileType SimpleUnrealPakView::FFileList::GetFileType() const
{
	return Filename.Contains(TEXT(".")) ? EFileType::FILE : EFileType::FOLDER;
}

void SimpleUnrealPakView::FilesParsing(const TArray<FString> &Filenames, FFileList &List)
{
	//Editor
	//Filename = G:/HotUpdate/Content/Geometry/Meshes/1M_Cube.uasset 
	//Filename = G:/HotUpdate/Content/Geometry/Meshes/1M_Cube_Chamfer.uasset
	//Runtime 
	//Filename = G:\HotUpdate\Saved\Cooked\WindowsNoEditor\HotUpdate\Content\Geometry\Meshes\1M_Cube_Chamfer.uasset
	//Filename = ../../../Projectname/Content/Geometry/Meshes/1M_Cube.uasset 
	FFileList *FileList = &List;
	FileList->Filename = FApp::GetProjectName();
	for (const auto &Filename : Filenames)
	{
		auto Point = Filename;
		auto Pos = Point.Find("/Content/");
		auto NewPoint = Point.RightChop(Pos);
		NewPoint.ReplaceInline(TEXT("/Content/"), TEXT("/Game/"));

		TArray<FString> FileLevel;
		NewPoint.ParseIntoArray(FileLevel, TEXT("/"));

		FileList = &List;
		for (const auto &TmpFile: FileLevel)
		{
			FFileList FileListElement;
			FileListElement.Filename = TmpFile;

			int32 i = FileList->Children.AddUnique(FileListElement);
			FileList = &(FileList->Children[i]);
		}

		FileList->Filename = NewPoint;
	}
}

