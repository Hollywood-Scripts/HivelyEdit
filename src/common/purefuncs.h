#include <stdio.h>
#include <stddef.h>
#include <stdarg.h>

#include <hollywood/types.h>

extern UBYTE *verasans;
extern UBYTE *veramono;
extern UBYTE *veraserif;
extern int sizeof_verasans;
extern int sizeof_veramono;
extern int sizeof_veraserif;

#ifdef HW_AMIGA

#ifdef __cplusplus
extern "C"
{
#endif

void *pure_malloc(size_t size);
void *pure_realloc(void *memblock, size_t size);
void *pure_calloc(size_t num, size_t size);
void pure_free(void *ptr);
char *pure_strdup(const char *str);
FILE *pure_fopen(char *filename, char *modes);
int pure_fclose(FILE *stream);
int pure_fseek(FILE *fp, long offset, int how);
long pure_ftell(FILE *fp);
int pure_ferror(FILE *fp);
int pure_feof(FILE *fp);
size_t pure_fread(void *buf, size_t objsize, size_t nobjs, FILE *fp);
size_t pure_fwrite(const void *buf, size_t objsize, size_t nobjs, FILE *fp);
int pure_snprintf(char *buffer, size_t count, const char *format, ...);
int pure_vsnprintf(char *buffer, size_t count, const char *format, va_list argptr);
int pure_printf(const char *format, ...);
void pure_qsort(void *array, size_t num, size_t size, int (*cmpfunc)(const void *arg1, const void *arg2));
int pure_sscanf(const char *str, const char *ctl, ...);
double pure_strtod(const char *s, char **tp);
int pure_gettimeofday(void *tv, void *restrict);
int pure_stricmp(const char *s1, const char *s2);
int pure_strnicmp(const char *s1, const char *s2, int n);
long pure_strtol(const char *str, char **tail, int base);
unsigned long pure_strtoul(const char *str, char **tail, int base);

#ifdef __cplusplus
}
#endif

#else

#define pure_malloc malloc
#define pure_free free
#define pure_realloc realloc
#define pure_calloc calloc
#define pure_fopen fopen
#define pure_fclose fclose
#define pure_fread fread
#define pure_feof feof
#define pure_ferror ferror
#define pure_fwrite fwrite
#define pure_stricmp stricmp
#define pure_strnicmp strnicmp
#define pure_strtod strtod
#define pure_strtol strtol

#ifdef __VBCC__
#define pure_snprintf snprintf
#define pure_vsnprintf vsnprintf
#elif defined(_MSC_VER)
#define pure_snprintf _snprintf
#define pure_vsnprintf _vsnprintf
#else
#define pure_snprintf snprintf
#define pure_vsnprintf vsnprintf
#endif

#define pure_printf printf

#ifdef __VBCC__
void pure_qsort(void *array, size_t num, size_t size, int (*cmpfunc)(const void *arg1, const void *arg2));
char *mystrdup(const char *s1);
#define pure_strdup(str) mystrdup(str);
#else
#define pure_qsort qsort
#define pure_strdup(str) strdup(str)
#endif

#endif

#ifndef HOLLYWOOD_PLATFORM_H
int hwos_Close(APTR fh);
ULONG hwos_Seek(APTR fh, ULONG pos, int mode);
int hwos_FRead(APTR fh, APTR block, ULONG blocklen);
APTR hwos_LoadImage(STRPTR filename, ULONG *taglist, int *retwidth, int *retheight, int *retalphachannel);
ULONG *hwos_GetImageData(APTR handle);
void hwos_FreeImage(APTR handle);
void hwos_TmpNam(STRPTR buf);
int hwos_DeleteFile(STRPTR file);
void hwos_GetCurrentDir(STRPTR buf, int len);
int hwos_AddPart(STRPTR dirname, STRPTR filename, int size);
STRPTR hwos_FindTTFFont(STRPTR name, int weight, int slant, int fileonly, int *retoffset, int *retlen, int *rettmp);
APTR hwos_Lock(STRPTR name, int mode);
void hwos_UnLock(APTR lock);
int hwos_NameFromLock(APTR lock, STRPTR str, int len);
STRPTR hwos_PathPart(STRPTR pathname);
int hwos_FGetC(APTR fh);
#endif

long int pure_lrint(double x);
long pure_lrintf(float x);
double pure_rint(double x);


