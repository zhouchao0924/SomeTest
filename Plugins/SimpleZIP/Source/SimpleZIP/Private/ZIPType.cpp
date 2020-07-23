#include "ZIPType.h"

void SimpleZIP::CollectErrorInformation(unsigned long Code)
{
	switch (Code)
	{
	case SIMPLE_ZIP_OK:
		UE_LOG(LogTemp, Log, TEXT("the pseudo-code zr-recent is never returned."));
		break;
	case	SIMPLE_ZIP_RECENT:
		UE_LOG(LogTemp, Log, TEXT("but can be passed to FormatZipMessage."));
		break;
	case	SIMPLE_ZIP_GENMASK:
		UE_LOG(LogTemp, Log, TEXT("SIMPLE_ZIP_GENMASK"));
		break;
	case	SIMPLE_ZIP_NODUPH:
		UE_LOG(LogTemp, Log, TEXT("couldn't duplicate the handle."));
		break;
	case	SIMPLE_ZIP_NOFILE:
		UE_LOG(LogTemp, Error, TEXT("couldn't create/open the file."));
		break;
	case	SIMPLE_ZIP_NOALLOC:
		UE_LOG(LogTemp, Error, TEXT("failed to allocate some resource."));
		break;
	case	SIMPLE_ZIP_WRITE:
		UE_LOG(LogTemp, Error, TEXT("a general error writing to the file."));
		break;
	case	SIMPLE_ZIP_NOTFOUND:
		UE_LOG(LogTemp, Warning, TEXT("couldn't find that file in the zip."));
		break;
	case	SIMPLE_ZIP_MORE:
		UE_LOG(LogTemp, Warning, TEXT("there's still more data to be unzipped."));
		break;
	case	SIMPLE_ZIP_CORRUPT:
		UE_LOG(LogTemp, Warning, TEXT("the zipfile is corrupt or not a zipfile."));
		break;
	case	SIMPLE_ZIP_READ:
		UE_LOG(LogTemp, Error, TEXT("a general error reading the file."));
		break;
	case	SIMPLE_ZIP_CALLERMASK:
		UE_LOG(LogTemp, Log, TEXT("SIMPLE_ZIP_CALLERMASK"));
		break;
	case	SIMPLE_ZIP_ARGS:
		UE_LOG(LogTemp, Error, TEXT("general mistake with the arguments."));
		break;
	case	SIMPLE_ZIP_NOTMMAP:
		UE_LOG(LogTemp, Log, TEXT("tried to ZipGetMemory, but that only works on mmap zipfiles, which yours wasn't."));
		break;
	case	SIMPLE_ZIP_MEMSIZE:
		UE_LOG(LogTemp, Error, TEXT("the memory size is too small."));
		break;
	case	SIMPLE_ZIP_FAILED:
		UE_LOG(LogTemp, Error, TEXT("the thing was already failed when you called this function."));
		break;
	case	SIMPLE_ZIP_ENDED:
		UE_LOG(LogTemp, Warning, TEXT("the zip creation has already been closed."));
		break;
	case	SIMPLE_ZIP_MISSIZE:
		UE_LOG(LogTemp, Warning, TEXT("the indicated input file size turned out mistaken."));
		break;
	case	SIMPLE_ZIP_PARTIALUNZ:
		UE_LOG(LogTemp, Warning, TEXT("the file had already been partially unzipped."));
		break;
	case	SIMPLE_ZIP_ZMODE:
		UE_LOG(LogTemp, Error, TEXT("tried to mix creating/opening a zip."));
		break;
	case	SIMPLE_ZIP_BUGMASK:
		UE_LOG(LogTemp, Log, TEXT("SIMPLE_ZIP_BUGMASK."));
		break;
	case	SIMPLE_ZIP_NOTINITED:
		UE_LOG(LogTemp, Error, TEXT("initialisation didn't work."));
		break;
	case	SIMPLE_ZIP_SEEK:
		UE_LOG(LogTemp, Warning, TEXT("trying to seek in an unseekable file."));
		break;
	case	SIMPLE_ZIP_NOCHANGE:
		UE_LOG(LogTemp, Error, TEXT("changed its mind on storage, but not allowed."));
		break;
	case	SIMPLE_ZIP_FLATE:
		UE_LOG(LogTemp, Error, TEXT("an internal error in the de / inflation code."));
		break;
	}
}

