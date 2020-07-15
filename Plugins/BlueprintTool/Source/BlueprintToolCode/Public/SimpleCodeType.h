#pragma once

namespace UE4Code_public
{
	struct FCodeType
	{
		const char *NameUTF;
	};

	enum ECodeType
	{
		Resources = 0,
		Event,
		Describe,
	};
}

struct FBlueprintToolCode
{
	static const UE4Code_public::FCodeType Mate_Data[];
};

const UE4Code_public::FCodeType FBlueprintToolCode::Mate_Data[] = {
	{"Resources"},
	{"Event"},
	{"Describe"},
};

using namespace UE4Code_public;