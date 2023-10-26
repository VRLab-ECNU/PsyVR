#pragma once

UENUM(BlueprintType)
enum class EEncodingOpt : uint8
{
	AutoDetect,
	ForceAnsi,
	ForceUnicode,
	ForceUTF8,
	ForceUTF8WithoutBOM
};