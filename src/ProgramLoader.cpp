#include "ProgramLoader.h"

byte* ProgramLoader::LoadProgram(const char* path, long* size)
{

	FILE* f;
	fopen_s(&f,path, "rb");
	if (f == 0)
	{
		printf("error loadint the %s file\n", path);
		return 0;
	}

	fseek(f, 0, SEEK_END);
	

	*size = ftell(f);

	//reading and null terminating the string
	rewind(f);
	byte* data = (byte*)malloc((*size + 2) * sizeof(byte));
	if(data!=NULL)
	{
		fread(data, sizeof(byte), *size, f);
		data[*size - 1] = '\0';
		data[*size - 2] = '\0';
		//last 2 bytes are 0
	}

	fclose(f);

	return data;
}
