/* genlib.h:
 *	Header file for functions in genlib.c (and some others).
 *
 *	General notice:
 *	This code is part of a boot-monitor package developed as a generic base
 *	platform for embedded system designs.  As such, it is likely to be
 *	distributed to various projects beyond the control of the original
 *	author.  Please notify the author of any enhancements made or bugs found
 *	so that all may benefit from the changes.  In addition, notification back
 *	to the author will allow the new user to pick up changes that may have
 *	been made by other users after this version of the code was distributed.
 *
 *	Note1: the majority of this code was edited with 4-space tabs.
 *	Note2: as more and more contributions are accepted, the term "author"
 *		   is becoming a mis-representation of credit.
 *
 *	Original author:	Ed Sutter
 *	Email:				esutter@lucent.com
 *	Phone:				908-582-2351
 */
#ifndef _GENLIB_H_
#define _GENLIB_H_

#include <stdarg.h>

/* Some compilers consider sizeof() to be unsigned... */
#define sizeof (int)sizeof

extern int optind;
extern char *optarg;

extern int abs(int);
extern int atoi(char *);
extern int memcmp(char *, char *, int);
extern int strcmp(char *, char *);
extern int strcasecmp(char *, char *);
extern int strncmp(char *, char *, int);
extern int strlen(char *);
extern int strspn(char *, char *);
extern int getopt(int,char **,char *);
extern int s_memcpy(char *, char *, int, int, int);
extern int s_memset(unsigned char *,unsigned char,int,int,int);
extern char *memccpy(char *, char *, int, int);
extern char *memchr(char *, char, int);
extern char *memcpy(char *, char *, int);
extern void bcopy(char *, char *, int);
extern char *memset(char *, char, int);
extern char *strcat(char *, char *);
extern char *strchr(char *, char);
extern char *strstr(char *, char *);
extern char *strcpy(char *, char *);
extern char *strncat(char *, char *, int);
extern char *strncpy(char *, char *, int);
extern char *strpbrk(char *, char *);
extern char *strrchr(char *, char);
extern char *strtok(char *, char *);
extern char *strtolower(char *string);
extern char *strtoupper(char *string);
extern long strtol(char *, char **, int);
extern unsigned short swap2(unsigned short);
extern unsigned long swap4(unsigned long);
extern unsigned long strtoul(char *, char **, int);
extern void getoptinit(void);
extern void prstring(char *);
extern void prlong(long);

/* Included here, but not in genlib.c: */
extern int target_gotachar(void), gotachar(void);
extern int target_getchar(void), getchar(void);
extern int target_putchar(char), putchar(char);
extern int target_console_empty(void);
extern int AddrToSym(int,unsigned long,char *,unsigned long *);
extern int printf(char *, ...);
extern int sprintf(char *, char *, ...);
extern int snprintf(char *, int, char *, ...);
extern int vsnprintf(char *, int, char *, va_list);
extern int cprintf(char *, ...);
extern int getbytes(char *,int,int);
extern int getbytes_t(char *,int,int);
extern int putbytes(char *,int);
extern int getUsrLvl(void);
extern int setenv(char *,char *);
extern int shell_sprintf(char *,char *fmt,...);
extern int getline(char *,int,int);
extern int getline_t(char *,int,int);
extern int getline_p(char *,int,int,char *);
extern int getfullline(char *buf,int max,int ledit,int timeout,\
	char *prefill, int echo);
extern int stkchk(char *);
extern int inRange(char *,int);
extern int More(void);
extern int validPassword(char *,int);
extern int newPasswordFile(void);
extern int extValidPassword(char *,int);
extern int setCmdUlvl(char *,int);
extern int askuser(char *);
extern int hitakey(void);
extern int getreg(char *,unsigned long *);
extern int putreg(char *,unsigned long);
extern int putargv(int,char *);
extern int addrtosector(unsigned char *,int *,int *,unsigned char **);
extern int AppFlashWrite(unsigned char *,unsigned char *,long);
extern int AppFlashErase(int);
extern int flushDcache(char *,int);
extern int invalidateIcache(char *,int);
extern int pollConsole(char *);
extern int sectortoaddr(int,int *,unsigned char **);
extern int sectorProtect(char *,int);
extern int FlashInit(void);
extern int cacheInit(void);
extern int pioget(char,int);
extern int extendHeap(char *,int);
extern int decompress(char *,int,char *);
extern int RedirectionCheck(char *);
extern int docommand(char *, int);
extern int SymFileFd(int);
extern int ShellVarInit(void);
extern int showDate(int);
extern int inUmonBssSpace(char *,char *);
extern int passwordFileExists(void);
extern int Mtrace(char *, ...);
extern int monWatchDog(void);
extern int ConsoleBaudSet(int);
extern int ChangeConsoleBaudrate(int);
extern int TextInRam(void);
extern int uMonInRam(void);
extern long portCmd(int, void *);
extern unsigned short xcrc16(unsigned char *buffer,unsigned long nbytes);
extern unsigned long crc32(unsigned char *,unsigned long);
extern unsigned long intsoff(void);
extern unsigned long getAppRamStart(void);
extern unsigned long assign_handler(long, unsigned long, unsigned long);
extern char *line_edit(char *);
#ifndef MALLOC_DEBUG
extern char *malloc(int);
extern char *realloc(char *,int);
#endif
extern char *getenvp(void);
extern char *getenv(char *);
extern char *getpass(char *,char *,int,int);
extern char *getsym(char *,char *,int);
extern char *monVersion(void);
extern char *monBuilt(void);
extern char *ExceptionType2String(int);
extern char *shellsym_chk(char,char *,int *,char *,int);
extern void stkusage(void);
extern void target_reset(void);
extern void flush_console_out(void);
extern void flush_console_in(void);
extern void initCPUio(void);
extern void historyinit(void);
extern void AppWarmStart(unsigned long mask);
extern void MtraceInit(char *,int);
extern void monrestart(int);
extern void historylog(char *);
extern void free(char *);
extern void puts(char *);
extern void putstr(char *);
extern void MonitorBuiltEnvSet(void);
extern void writeprompt(void);
extern void intsrestore(unsigned long);
extern void prascii(unsigned char *,int);
extern void cacheInitForTarget(void);
extern void exceptionAutoRestart(int);
extern void clrTmpMaxUsrLvl(int (*)(void));
extern void *setTmpMaxUsrLvl(void);
extern void rawon(void), rawoff(void);
extern void monHeader(int);
extern void mstatshowcom(void);
extern void CommandLoop(void);
extern void flushRegtbl(void);

extern void showregs(void), reginit(void);
extern void initUsrLvl(int);
extern void warmstart(int);
extern void coldstart(void);
extern void InitRemoteIO(void);
extern void appexit(int);
extern void pioset(char,int);
extern void pioclr(char,int);
extern void getargv(int *argc, char ***argv);
extern void init0(void), init1(void), init2(void), init3(void);
extern void EnableBreakInterrupt(void);
extern void DisableBreakInterrupt(void);
extern void	ctxMON(void), ctxAPP(void);
extern void printMem(unsigned char *,int,int);
extern void monDelay(int);
extern void ticktock(void);
extern void atinit(void);
extern void umonBssInit(void);
extern void ConsoleBaudEnvSet(void);
extern void LowerFlashProtectWindow(void);
#if INCLUDE_REDIRECT
extern void RedirectCharacter(char);
extern void RedirectionCmdDone(void);
#else
#define RedirectCharacter(c)
#define RedirectionCmdDone()
#endif

extern unsigned long MonStack[];
extern unsigned long crc32tab[];
extern unsigned short xcrc16tab[];
extern char *Mtracebuf;
extern char ApplicationInfo[];
extern unsigned long ExceptionAddr, LoopsPerMillisecond;
extern unsigned long APPLICATION_RAMSTART, BOOTROM_BASE;
extern int ConsoleDevice;
extern int ConsoleBaudRate;
extern int StateOfMonitor, AppExitStatus, ExceptionType;
extern int	bss_start, bss_end, boot_base;
extern int  (*remoterawon)(void), (*remoterawoff)(void);
extern int  (*remoteputchar)(int), (*remotegetchar)(void);
extern int	(*remotegotachar)(void);
extern int  (*dcacheFlush)(char *,int), (*icacheInvalidate)(char *,int);
extern int	(*extgetUsrLvl)(void);
extern int	(*moncomptr)(int,void *, void *, void *);

#define STACK_PREINIT_VAL	0x63636363

/* If the watchdog macro is defined, then we also define the
 * WATCHDOG_ENABLED macro so that code can use #ifdef WATCHDOG_ENABLED
 * or simply insert WATCHDOG_MACRO inline...
 * This eliminates the need for the ifdef wrapper if no other code is
 * included with the macro. 
 */
#ifdef WATCHDOG_MACRO
#define WATCHDOG_ENABLED
#else
#define WATCHDOG_MACRO
#endif

#endif
