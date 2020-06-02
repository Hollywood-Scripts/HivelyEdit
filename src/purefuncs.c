#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>

#include <hollywood/plugin.h>

extern hwPluginAPI *hwcl;

int pure_printf(const char *format, ...)
{
	va_list args;
	int r;

	va_start(args, format);
	r = hwcl->CRTBase->vprintf(format, args);
	va_end(args);

	return r;
}

int hwos_Close(APTR fh)
{
	return hwcl->DOSBase->hw_FClose(fh);
}

ULONG hwos_Seek(APTR fh, ULONG pos, int mode)
{
	return hwcl->DOSBase->hw_FSeek(fh, pos, mode);
}

int hwos_FRead(APTR fh, APTR block, ULONG blocklen)
{
	return hwcl->DOSBase->hw_FRead(fh, block, blocklen);
}

int hwos_FGetC(APTR fh)
{
	return hwcl->DOSBase->hw_FGetC(fh);
}

#ifdef HW_AMIGA
void *pure_malloc(size_t size)
{
	return hwcl->CRTBase->malloc(size);
}

void *pure_calloc(size_t num, size_t size)
{
	return hwcl->CRTBase->calloc(num, size);
}

void pure_free(void *ptr)
{
	hwcl->CRTBase->free(ptr);
}
#endif

