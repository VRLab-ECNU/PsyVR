#include "Utils/PsyBPFL.h"

FString UPsyBPFL::ConcatCsvRow(TArray<FString> Row, FString Delimiter, bool WithNewline)
{
	FString result = "";

	for(auto &r: Row)
	{
		// if not the last one, append delimiter
		if(&r - &Row[0] != Row.Num() - 1)
			result.Append(r).Append(Delimiter);
		else
			result.Append(r);
	}

	if (WithNewline)
		result.Append("\n");
	
	return result;
}
