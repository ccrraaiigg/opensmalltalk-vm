/* Automatically generated from Squeak on #(18 March 2005 7:42:48 pm) */

#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

/* Default EXPORT macro that does nothing (see comment in sq.h): */
#define EXPORT(returnType) returnType

/* Do not include the entire sq.h file but just those parts needed. */
/*  The virtual machine proxy definition */
#include "sqVirtualMachine.h"
/* Configuration options */
#include "sqConfig.h"
/* Platform specific definitions */
#include "sqPlatformSpecific.h"

#define true 1
#define false 0
#define null 0  /* using 'null' because nil is predefined in Think C */
#ifdef SQUEAK_BUILTIN_PLUGIN
#undef EXPORT
// was #undef EXPORT(returnType) but screws NorCroft cc
#define EXPORT(returnType) static returnType
#endif
#include "FilePlugin.h"
#include "SocketPlugin.h"
#include "aio.h"

/* memory access macros */
#define byteAt(i) (*((unsigned char *) (i)))
#define byteAtput(i, val) (*((unsigned char *) (i)) = val)
#define longAt(i) (*((int *) (i)))
#define longAtput(i, val) (*((int *) (i)) = val)


/*** Constants ***/

/*** Function Prototypes ***/
static void aioForwardwithDataandFlags(int fd, void * data, int flags);
static int fileDescriptorFrom(int aFileHandle);
static int fileRecordSize(void);
static SQFile * fileValueOf(int anSQFileRecord);
#pragma export on
EXPORT(const char*) getModuleName(void);
#pragma export off
static int halt(void);
#pragma export on
EXPORT(int) initialiseModule(void);
#pragma export off
static int isSQFileObject(int objectPointer);
static int isSQSocketObject(int objectPointer);
static int msg(char *s);
#pragma export on
EXPORT(int) primitiveAioDisable(void);
EXPORT(int) primitiveAioEnable(void);
EXPORT(int) primitiveAioHandle(void);
EXPORT(int) primitiveAioSuspend(void);
EXPORT(int) primitiveModuleName(void);
EXPORT(int) primitiveOSFileHandle(void);
EXPORT(int) primitiveOSSocketHandle(void);
EXPORT(int) primitiveVersionString(void);
EXPORT(int) setInterpreter(struct VirtualMachine* anInterpreter);
EXPORT(int) shutdownModule(void);
#pragma export off
static int socketDescriptorFrom(int sqSocketOop);
static int socketRecordSize(void);
static SQSocket * socketValueOf(int anSQSocketRecord);
static int stringFromCString(char *aCString);
static char * versionString(void);
/*** Variables ***/

#ifdef SQUEAK_BUILTIN_PLUGIN
extern
#endif
struct VirtualMachine* interpreterProxy;
static const char *moduleName =
#ifdef SQUEAK_BUILTIN_PLUGIN
	"AioPlugin 18 March 2005 (i)"
#else
	"AioPlugin 18 March 2005 (e)"
#endif
;



/*	This function is called to signal a Smalltalk Semaphore when an asynchronous event is
	detected. When translated to C, the name of this method is aioForwardwithDataandFlags.
	The event handler is set up by #primitiveAioHandle. */

static void aioForwardwithDataandFlags(int fd, void * data, int flags) {
    int *pfd;
    int semaIndex;

	pfd = data;
	semaIndex = *pfd;
	interpreterProxy->signalSemaphoreWithIndex(semaIndex);
}


/*	Answer the OS file descriptor, an integer value, from a SQFile data structure,
	or answer -1 if unable to obtain the file descriptor (probably due to receiving
	an incorrect type of object as aFileHandle). This method may be called from a
	primitive, and is not intended to be called from Smalltalk. */

static int fileDescriptorFrom(int aFileHandle) {
    FILE *osFileStream;
    SQFile * sqFile;

	/* begin fileValueOf: */
	if ((interpreterProxy->isBytes(aFileHandle)) && ((interpreterProxy->byteSizeOf(aFileHandle)) == (fileRecordSize()))) {
		sqFile = interpreterProxy->arrayValueOf(aFileHandle);
		goto l1;
	} else {
		interpreterProxy->primitiveFail();
		sqFile = null;
		goto l1;
	}
l1:	/* end fileValueOf: */;
	if (sqFile == 0) {
		return -1;
	} else {
		osFileStream = sqFile->file;
		if (osFileStream == 0) {
			return -1;
		}
		return fileno(osFileStream);
	}
}


/*	Answer the size of a SQFile data structure in bytes. */

static int fileRecordSize(void) {
	return sizeof(SQFile);
}


/*	Return a pointer to the first byte of of the SQFile data structure file record within
	anSQFileRecord, which is expected to be a ByteArray of size self>>fileRecordSize. */

static SQFile * fileValueOf(int anSQFileRecord) {
	if ((interpreterProxy->isBytes(anSQFileRecord)) && ((interpreterProxy->byteSizeOf(anSQFileRecord)) == (fileRecordSize()))) {
		return interpreterProxy->arrayValueOf(anSQFileRecord);
	} else {
		interpreterProxy->primitiveFail();
		return null;
	}
}


/*	Note: This is hardcoded so it can be run from Squeak.
	The module name is used for validating a module *after*
	it is loaded to check if it does really contain the module
	we're thinking it contains. This is important! */

EXPORT(const char*) getModuleName(void) {
	return moduleName;
}

static int halt(void) {
	;
}

EXPORT(int) initialiseModule(void) {
}


/*	Answer true if objectPointer appears to be a valid SQFile ByteArray. This check
	is appropriate if objectPointer has been passed as a parameter to a primitive, and
	is expected to represent a valid file reference. */

static int isSQFileObject(int objectPointer) {
    unsigned char *sqFileBytes;
    int idx;

	if (!((interpreterProxy->isBytes(objectPointer)) && ((interpreterProxy->byteSizeOf(objectPointer)) == (fileRecordSize())))) {
		return 0;
	}
	sqFileBytes = interpreterProxy->arrayValueOf(objectPointer);
	idx = 0;
	while (idx < (fileRecordSize())) {
		if ((sqFileBytes[idx]) != 0) {
			return 1;
		}
		idx += 1;
	}
	return 0;
}


/*	Answer true if objectPointer appears to be a valid SQSocket ByteArray. This check
	is appropriate if objectPointer has been passed as a parameter to a primitive, and
	is expected to represent a valid socket reference. */

static int isSQSocketObject(int objectPointer) {
    int idx;
    unsigned char *sqSocketBytes;

	if (!((interpreterProxy->isBytes(objectPointer)) && ((interpreterProxy->byteSizeOf(objectPointer)) == (socketRecordSize())))) {
		return 0;
	}
	sqSocketBytes = interpreterProxy->arrayValueOf(objectPointer);
	idx = 0;
	while (idx < (socketRecordSize())) {
		if ((sqSocketBytes[idx]) != 0) {
			return 1;
		}
		idx += 1;
	}
	return 0;
}

static int msg(char *s) {
	fprintf(stderr, "\n%s: %s", moduleName, s);
}


/*	Definitively disable asynchronous event notification for a descriptor. The
	parameter is an OS level integer file descriptor. */

EXPORT(int) primitiveAioDisable(void) {
    int fd;

	fd = interpreterProxy->stackIntegerValue(0);
	if (fd == (interpreterProxy->nilObject())) {
		return interpreterProxy->primitiveFail();
	}
	if (fd < 0) {
		return interpreterProxy->primitiveFail();
	}
	aioDisable(fd);
	interpreterProxy->pop(2);
	interpreterProxy->pushInteger(fd);
}


/*	Enable asynchronous notification for a descriptor. The first parameter is an OS
	level integer file descriptor. The second parameter is the index of a Semaphore to
	be notified, and the third parameter is a flag indicating that descriptor represents
	an external object and should not be closed on termination of aio handling. Answer
	the semaphore index. */

EXPORT(int) primitiveAioEnable(void) {
    static int eventSemaphoreIndices[FD_SETSIZE];
    int fd;
    int flags;
    int semaIndex;
    int externalObject;

	fd = interpreterProxy->stackIntegerValue(2);
	if (fd == (interpreterProxy->nilObject())) {
		return interpreterProxy->primitiveFail();
	}
	if (fd < 0) {
		return interpreterProxy->primitiveFail();
	}
	semaIndex = interpreterProxy->stackIntegerValue(1);
	eventSemaphoreIndices[semaIndex] = semaIndex;
	externalObject = interpreterProxy->stackObjectValue(0);
	if (externalObject == (interpreterProxy->trueObject())) {
		flags = AIO_EXT;
	} else {
		flags = 0;
	}
	aioEnable(fd, &(eventSemaphoreIndices[semaIndex]), flags);
	interpreterProxy->pop(4);
	interpreterProxy->pushInteger(semaIndex);
}


/*	Handle asynchronous event notification for a descriptor. The first parameter is
	an OS level integer file descriptor. The remaining three parameters are Boolean
	flags representing the types of events for which notification is being requested:
	handle exceptions, handle for read, and handle for write.
	Flags are defined in the aio.h source as:
		AIO_X	(1<<0)	handle for exceptions
		AIO_R	(1<<1)	handle for read
		AIO_W	(1<<2)	handle for write */

EXPORT(int) primitiveAioHandle(void) {
    int fd;
    int readWatch;
    int flags;
    int exceptionWatch;
    int writeWatch;

	fd = interpreterProxy->stackIntegerValue(3);
	if (fd == (interpreterProxy->nilObject())) {
		return interpreterProxy->primitiveFail();
	}
	if (fd < 0) {
		return interpreterProxy->primitiveFail();
	}
	exceptionWatch = interpreterProxy->stackObjectValue(2);
	readWatch = interpreterProxy->stackObjectValue(1);
	writeWatch = interpreterProxy->stackObjectValue(0);
	flags = 0;
	if (exceptionWatch == (interpreterProxy->trueObject())) {
		flags = flags | (AIO_X);
	}
	if (readWatch == (interpreterProxy->trueObject())) {
		flags = flags | (AIO_R);
	}
	if (writeWatch == (interpreterProxy->trueObject())) {
		flags = flags | (AIO_W);
	}
	aioHandle(fd, aioForwardwithDataandFlags, flags);
	interpreterProxy->pop(5);
	interpreterProxy->pushInteger(flags);
}


/*	Temporarily suspend asynchronous event notification for a descriptor. The first
	parameter is an OS level integer file descriptor. The remaining three parameters
	are Boolean flags representing the types of events for which notification is being
	requested: handle exceptions, handle for read, and handle for write.
	Flags are defined in the aio.h source as:
		AIO_X	(1<<0)	handle for exceptions
		AIO_R	(1<<1)	handle for read
		AIO_W	(1<<2)	handle for write */

EXPORT(int) primitiveAioSuspend(void) {
    int fd;
    int readWatch;
    int flags;
    int exceptionWatch;
    int writeWatch;

	fd = interpreterProxy->stackIntegerValue(3);
	if (fd == (interpreterProxy->nilObject())) {
		return interpreterProxy->primitiveFail();
	}
	if (fd < 0) {
		return interpreterProxy->primitiveFail();
	}
	exceptionWatch = interpreterProxy->stackObjectValue(2);
	readWatch = interpreterProxy->stackObjectValue(1);
	writeWatch = interpreterProxy->stackObjectValue(0);
	flags = 0;
	if (exceptionWatch == (interpreterProxy->trueObject())) {
		flags = flags | (AIO_X);
	}
	if (readWatch == (interpreterProxy->trueObject())) {
		flags = flags | (AIO_R);
	}
	if (writeWatch == (interpreterProxy->trueObject())) {
		flags = flags | (AIO_W);
	}
	aioSuspend(fd, flags);
	interpreterProxy->pop(5);
	interpreterProxy->pushInteger(flags);
}


/*	Answer a string containing the module name string for this plugin. */

EXPORT(int) primitiveModuleName(void) {
	interpreterProxy->popthenPush(1, stringFromCString(moduleName));
}


/*	Take a struct SQFile from the stack, and answer the value of its Unix file number. */

EXPORT(int) primitiveOSFileHandle(void) {
    int fileNo;
    int sqFileOop;

	sqFileOop = interpreterProxy->stackValue(0);
	if (!(isSQFileObject(sqFileOop))) {
		return interpreterProxy->primitiveFail();
	}
	fileNo = fileDescriptorFrom(sqFileOop);
	interpreterProxy->pop(2);
	interpreterProxy->pushInteger(fileNo);
}


/*	Take a struct SQSocket from the stack, and answer the value of its Unix file number. */

EXPORT(int) primitiveOSSocketHandle(void) {
    int fileNo;
    int sqSocketOop;

	sqSocketOop = interpreterProxy->stackValue(0);
	if (!(isSQSocketObject(sqSocketOop))) {
		return interpreterProxy->primitiveFail();
	}
	fileNo = socketDescriptorFrom(sqSocketOop);
	if (fileNo < 0) {
		return interpreterProxy->primitiveFail();
	}
	interpreterProxy->pop(2);
	interpreterProxy->pushInteger(fileNo);
}


/*	Answer a string containing the version string for this plugin. */

EXPORT(int) primitiveVersionString(void) {
	interpreterProxy->popthenPush(1, stringFromCString(versionString()));
}


/*	Note: This is coded so that is can be run from Squeak. */

EXPORT(int) setInterpreter(struct VirtualMachine* anInterpreter) {
    int ok;

	interpreterProxy = anInterpreter;
	ok = interpreterProxy->majorVersion() == VM_PROXY_MAJOR;
	if (ok == 0) {
		return 0;
	}
	ok = interpreterProxy->minorVersion() >= VM_PROXY_MINOR;
	return ok;
}

EXPORT(int) shutdownModule(void) {
}


/*	Answer the OS file descriptor, an integer value, from a SQSocket data structure,
	or answer -1 if unable to obtain the file descriptor (probably due to receiving
	an incorrect type of object as aFileHandle). */
/*	Gross hack warning: The first element of privateSocketStruct happens to be
	the Unix file number of the socket. See sqUnixSocket.c for the definition. This
	method is coded to take advantage of this factoid, and will break if anyone
	ever redefines the data structure. */

static int socketDescriptorFrom(int sqSocketOop) {
    int descriptor;
    SQSocket * sqSocket;
    void *privateSocketStruct;

	/* begin socketValueOf: */
	if ((interpreterProxy->isBytes(sqSocketOop)) && ((interpreterProxy->byteSizeOf(sqSocketOop)) == (socketRecordSize()))) {
		sqSocket = interpreterProxy->arrayValueOf(sqSocketOop);
		goto l1;
	} else {
		sqSocket = null;
		goto l1;
	}
l1:	/* end socketValueOf: */;
	if (sqSocket == 0) {
		return -1;
	} else {
		privateSocketStruct = sqSocket->privateSocketPtr;
		if (privateSocketStruct == 0) {
			return -1;
		}
		descriptor = * (int *) privateSocketStruct;
		return descriptor;
	}
}


/*	Answer the size of a SQSocket data structure in bytes. */

static int socketRecordSize(void) {
	return sizeof(SQSocket);
}


/*	Return a pointer to the first byte of of the SQsocket data structure socket record within
	anSQSocketRecord, which is expected to be a ByteArray of size self>>socketRecordSize. */

static SQSocket * socketValueOf(int anSQSocketRecord) {
	if ((interpreterProxy->isBytes(anSQSocketRecord)) && ((interpreterProxy->byteSizeOf(anSQSocketRecord)) == (socketRecordSize()))) {
		return interpreterProxy->arrayValueOf(anSQSocketRecord);
	} else {
		return null;
	}
}


/*	Answer a new String copied from a null-terminated C string.
	Caution: This may invoke the garbage collector. */

static int stringFromCString(char *aCString) {
    int len;
    char *stringPtr;
    int newString;

	len = strlen(aCString);
	newString = interpreterProxy->instantiateClassindexableSize(interpreterProxy->classString(), len);
	stringPtr = interpreterProxy->arrayValueOf(newString);
	(char *)strncpy(stringPtr, aCString, len);
	return newString;
}


/*	Answer a string containing the version string for this plugin. Handle MNU
	errors, which can occur if class InterpreterPlugin has been removed from
	the system.

	Important: When this method is changed, the class side method must also be
	changed to match. */

static char * versionString(void) {
    static char version[]= "1.1";

	return version;
}


#ifdef SQUEAK_BUILTIN_PLUGIN


void* AioPlugin_exports[][3] = {
	{"AioPlugin", "primitiveOSFileHandle", (void*)primitiveOSFileHandle},
	{"AioPlugin", "primitiveAioDisable", (void*)primitiveAioDisable},
	{"AioPlugin", "primitiveAioEnable", (void*)primitiveAioEnable},
	{"AioPlugin", "primitiveAioSuspend", (void*)primitiveAioSuspend},
	{"AioPlugin", "initialiseModule", (void*)initialiseModule},
	{"AioPlugin", "primitiveAioHandle", (void*)primitiveAioHandle},
	{"AioPlugin", "shutdownModule", (void*)shutdownModule},
	{"AioPlugin", "primitiveVersionString", (void*)primitiveVersionString},
	{"AioPlugin", "primitiveModuleName", (void*)primitiveModuleName},
	{"AioPlugin", "primitiveOSSocketHandle", (void*)primitiveOSSocketHandle},
	{"AioPlugin", "getModuleName", (void*)getModuleName},
	{"AioPlugin", "setInterpreter", (void*)setInterpreter},
	{NULL, NULL, NULL}
};


#endif /* ifdef SQ_BUILTIN_PLUGIN */
