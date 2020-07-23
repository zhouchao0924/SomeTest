#include "Version/VersionInfor.h"
#include "Json.h"

void SimpleVersionControl::Save(const FVersionInfor &Infor, FString &Json)
{
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&Json);

	//{
	JsonWriter->WriteObjectStart();
	JsonWriter->WriteValue(TEXT("Name"), Infor.Name);
	JsonWriter->WriteValue(TEXT("Crc"), Infor.Crc);
	JsonWriter->WriteArrayStart(TEXT("Content"));
	for (const auto &Tmp : Infor.Content)
	{
		JsonWriter->WriteObjectStart();
		JsonWriter->WriteValue(TEXT("Len"), Tmp.Len);
		JsonWriter->WriteValue(TEXT("Link"), Tmp.Link);
		JsonWriter->WriteValue(TEXT("Crc"), Tmp.Crc);
		JsonWriter->WriteValue(TEXT("bDiscard"), Tmp.bDiscard);
		JsonWriter->WriteValue(TEXT("PakVersionType"), FString::FromInt((int32)Tmp.PakVersionType));
		JsonWriter->WriteObjectEnd();
	}
	JsonWriter->WriteArrayEnd();
	//}
	JsonWriter->WriteObjectEnd();
	JsonWriter->Close();
}

bool SimpleVersionControl::Read(const FString &Json, FVersionList &Infor)
{
	TSharedPtr<FJsonValue> JsonParsed;

	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(Json);
	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed))
	{
		bool bReadSuccess = true;

		const TSharedPtr<FJsonObject> JsonObject = JsonParsed->AsObject();

		if (!JsonObject->TryGetStringField(TEXT("Name"), Infor.Name))
		{
			bReadSuccess = false;
		}

		if (JsonObject->TryGetStringField(TEXT("Crc"), Infor.Crc))
		{
			bReadSuccess = false;
		}

		const TArray<TSharedPtr<FJsonValue>>* JsonArrays;
		if (!JsonObject->TryGetArrayField(TEXT("Content"), JsonArrays))
		{
			bReadSuccess = false;
		}
		else
		{
			if (JsonArrays)
			{
				for (const auto &Tmp : *JsonArrays)
				{
					Infor.Content.Add(Tmp->AsString());
				}
			}
		}

		return true;
	}

	return false;
}

bool SimpleVersionControl::Read(const FString &Json, FDLCList &Infor)
{
	TSharedPtr<FJsonValue> JsonParsed;

	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(Json);
	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed))
	{
		bool bReadSuccess = true;

		const TSharedPtr<FJsonObject> JsonObject = JsonParsed->AsObject();
		const TArray<TSharedPtr<FJsonValue>>* JsonArrays;
		if (!JsonObject->TryGetArrayField(TEXT("DLCList"), JsonArrays))
		{
			if (JsonArrays)
			{
				for (const auto &Tmp : *JsonArrays)
				{
					Infor.Add(Tmp->AsString());
				}
			}
		}

		return true;
	}

	return false;
}

void SimpleVersionControl::Save(const FVersionList &Infor, FString &Json)
{
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&Json);

	//{
	JsonWriter->WriteObjectStart();
	JsonWriter->WriteValue(TEXT("Name"), Infor.Name);
	JsonWriter->WriteValue(TEXT("Crc"), Infor.Crc);
	JsonWriter->WriteArrayStart(TEXT("Content"));
	for (const auto &Tmp : Infor.Content)
	{
		JsonWriter->WriteValue(Tmp);
	}
	JsonWriter->WriteArrayEnd();
	//}
	JsonWriter->WriteObjectEnd();
	JsonWriter->Close();
}

bool SimpleVersionControl::Read(const FString &Json, FDLCInfor &Infor)
{
	TSharedPtr<FJsonValue> JsonParsed;

	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(Json);
	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed))
	{
		bool bReadSuccess = true;

		const TSharedPtr<FJsonObject> JsonObject = JsonParsed->AsObject();
		if (!JsonObject->TryGetStringField(TEXT("Name"), Infor.Name))
		{
			bReadSuccess = false;
		}

		if (!JsonObject->TryGetStringField(TEXT("Crc"), Infor.Crc))
		{
			bReadSuccess = false;
		}

		if (!JsonObject->TryGetStringField(TEXT("Link"), Infor.Link))
		{
			bReadSuccess = false;
		}

		return true;
	}

	return false;
}

void SimpleVersionControl::Save(const FDLCList &Infor, FString &Json)
{
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&Json);
	JsonWriter->WriteObjectStart();
	JsonWriter->WriteArrayStart(TEXT("DLCList"));
	for (const auto &Tmp : Infor)
	{
		JsonWriter->WriteValue(Tmp);
	}
	JsonWriter->WriteArrayEnd();
	JsonWriter->WriteObjectEnd();
	JsonWriter->Close();
}

bool SimpleVersionControl::Read(const FString &Json, FVersionInfor &Infor)
{
	TSharedPtr<FJsonValue> JsonParsed;

	TSharedRef<TJsonReader<TCHAR>> JsonReader = TJsonReaderFactory<TCHAR>::Create(Json);
	if (FJsonSerializer::Deserialize(JsonReader, JsonParsed))
	{
		bool bReadSuccess = true;

		const TSharedPtr<FJsonObject> JsonObject = JsonParsed->AsObject();

		if (!JsonObject->TryGetStringField(TEXT("Name"), Infor.Name))
		{
			bReadSuccess = false;
		}

		if (JsonObject->TryGetStringField(TEXT("Crc"), Infor.Crc))
		{
			bReadSuccess = false;
		}

		const TArray<TSharedPtr<FJsonValue>>* JsonArrays;
		if (!JsonObject->TryGetArrayField(TEXT("Content"), JsonArrays))
		{
			bReadSuccess = false;
		}
		else
		{
			if (JsonArrays)
			{
				for (const auto &Tmp : *JsonArrays)
				{
					const TSharedPtr<FJsonObject> TmpJsonObject = Tmp->AsObject();

					FRemoteDataDescribe RemoteDataDescribe;
					if (!TmpJsonObject->TryGetStringField(TEXT("Link"), RemoteDataDescribe.Link))
					{
						bReadSuccess = false;
					}

					if (!TmpJsonObject->TryGetNumberField(TEXT("Len"), RemoteDataDescribe.Len))
					{
						bReadSuccess = false;
					}

					if (!TmpJsonObject->TryGetStringField(TEXT("Crc"), RemoteDataDescribe.Crc))
					{
						bReadSuccess = false;
					}

					{
						uint32 Type = 0;
						if (!TmpJsonObject->TryGetNumberField(TEXT("PakVersionType"), Type))
						{
							bReadSuccess = false;
						}
						else
						{
							RemoteDataDescribe.PakVersionType = (EPakVersionType)Type;
						}
					}

					{
						uint32 Type = 0;
						if (!TmpJsonObject->TryGetNumberField(TEXT("bDiscard"), Type))
						{
							bReadSuccess = false;
						}
						else
						{
							RemoteDataDescribe.bDiscard = (bool)Type;
						}
					}

					Infor.Content.Add(RemoteDataDescribe);
				}
			}
		}

		return true;
	}

	return false;
}

void SimpleVersionControl::Save(const FDLCInfor &Infor, FString &Json)
{
	TSharedRef<TJsonWriter<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>> JsonWriter = TJsonWriterFactory<TCHAR, TCondensedJsonPrintPolicy<TCHAR>>::Create(&Json);

	//{
	JsonWriter->WriteObjectStart();
	JsonWriter->WriteValue(TEXT("Name"), Infor.Name);
	JsonWriter->WriteValue(TEXT("Crc"), Infor.Crc);
	JsonWriter->WriteValue(TEXT("Link"), Infor.Link);
	//}
	JsonWriter->WriteObjectEnd();
	JsonWriter->Close();
}

void FVersionInfor::Empty()
{
	Name = "";
	Crc = "";
	Content.Empty();
}

void FVersionList::Empty()
{
	Name = "";
	Crc = "";
	Content.Empty();
}
