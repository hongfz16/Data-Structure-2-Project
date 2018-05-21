#include "global.h"

void getLabel(const std::string& imagename, char* label)
{
	int i = 0;
	while (imagename[i] != '_')
	{
		label[i] = imagename[i];
		i++;
	}
	label[i] = '\0';
}