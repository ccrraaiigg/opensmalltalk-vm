'From Squeak3.8 of ''5 May 2005'' [latest update: #6665] on 5 June 2005 at 2:45:12 pm'!
"Change Set:		Gnuifier
Date:			1 January 2002
Author:			acg
Some code to automate building a VM under GCC.  To run, fileIn and execute:
	(Gnuifier on: aFileDirectory) gnuify
For example, try something like the following: 
	(Gnuifier on: 
		((FileDirectory default 
			directoryNamed: 'src') 
				directoryNamed: 'vm') pathName) gnuify
"!

Object subclass: #Gnuifier
	instanceVariableNames: 'directory'
	classVariableNames: ''
	poolDictionaries: ''
	category: 'VMConstruction-Building'!

!Gnuifier commentStamp: '<historical>' prior: 0!
My instances automate the translation of a Squeak interpreter for use with GCC.  In the specified FileDirectory, I copy 'interp.c' to 'interp.c.old'; translate a gnuified interpreter back into 'interp.c'; and save a working copy of sqGnu.h.

To gnuify an interpreter, try something like one of the following:

	(Gnuifier on: 
		((FileDirectory default 
			directoryNamed: 'src') 
				directoryNamed: 'vm') pathName) gnuify

	(Gnuifier on: 
		'powercow X:Users:werdna:Desktop:squeak:Squeak3.2a-4599 Folder:src:vm') gnuify


Structure:
 directory		FileDirectory -- specifying where I should do my gnuification

I can attempt to undo my damage with #deGnuify.!


!Gnuifier methodsFor: 'as yet unclassified' stamp: 'acg 12/30/2001 14:16'!
copyFrom: inFileStream to: outFileStream

"convert interp.c to use GNU features"

|  inData |

	Cursor read showWhile:
		[inData := inFileStream upToEnd withSqueakLineEndings].
	Cursor write showWhile:
		[outFileStream nextPutAll: inData].
	outFileStream close! !

!Gnuifier methodsFor: 'as yet unclassified' stamp: 'acg 1/1/2002 10:10'!
deGnuify

	(directory fileExists: 'interp.c.old')
		ifFalse: [^Error signal: 'Cannot deGnuify.  The old "interp.c" was not found.'].

	(directory fileExists: 'interp.c')
		ifTrue: [directory deleteFileNamed: 'interp.c'].

	self	
		copyFrom: 	(directory oldFileNamed: 'interp.c.old')
		to:			(directory newFileNamed: 'interp.c').

	(directory fileExists: 'sqGnu.h')
		ifTrue: [directory deleteFileNamed: 'sqGnu.h'].
	
	directory deleteFileNamed: 'interp.c.old'! !

!Gnuifier methodsFor: 'as yet unclassified' stamp: 'JMM 8/6/2002 13:52'!
gnuify

	(directory fileExists: 'interp.c.old') ifTrue:
		[(PopUpMenu 
			confirm: 'Interpreter probably guified (interp.c.old exists).
Do you want to gnuify anyway?') ifFalse: [^nil].
		directory deleteFileNamed: 'interp.c.old'].
		
	self
		copyFrom: 	(directory oldFileNamed: 'interp.c')
		to: 			(directory newFileNamed: 'interp.c.old').

	directory deleteFileNamed: 'interp.c'.
	self
		gnuifyFrom:(directory oldFileNamed: 'interp.c.old')
		to: 			(directory newFileNamed: 'interp.c').
	
! !

!Gnuifier methodsFor: 'as yet unclassified' stamp: 'JMM 6/5/2005 14:42'!
gnuifyFrom: inFileStream to: outFileStream

"convert interp.c to use GNU features"

|  inData beforeInterpret inInterpret inInterpretVars beforePrimitiveResponse inPrimitiveResponse |

	Cursor read showWhile:
		[inData := inFileStream upToEnd withSqueakLineEndings.
		 inFileStream close].

	Cursor write showWhile:
		["print a header"
		outFileStream
			nextPutAll: '/* This file has been post-processed for GNU C */';
			cr; cr; cr.

		beforeInterpret := true.    "whether we are before the beginning of interpret()"
		inInterpret := false.     "whether we are in the middle of interpret"
		inInterpretVars := false.    "whether we are in the variables of interpret"
		beforePrimitiveResponse := true.  "whether we are before the beginning of primitiveResponse()"
		inPrimitiveResponse := false.   "whether we are inside of primitiveResponse"
		inData linesDo: [ :inLine |
			| outLine extraOutLine |
			outLine := inLine. 	"print out one line for each input line; by default, print out the line that was input, but some rules modify it"
			extraOutLine := nil.   "occasionally print a second output line..."
			beforeInterpret ifTrue: [
				(inLine = '#include "sq.h"') ifTrue: [
					outLine := '#include "sqGnu.h"'. ].
				(inLine = 'sqInt interpret(void) {') ifTrue: [
					"reached the beginning of interpret"
					beforeInterpret := false.
					inInterpret := true.
					inInterpretVars := true. ] ]
			ifFalse: [
			inInterpretVars ifTrue: [
				(inLine findString: 'register struct foo * foo = &fum;') > 0 ifTrue: [
				outLine := '#ifdef FOO_REG
    register struct foo * foo FOO_REG = &fum;
#endif' ].
				(inLine findString: ' localIP;') > 0 ifTrue: [
					outLine := '    register char* localIP IP_REG;' ].
				(inLine findString: ' localSP;') > 0 ifTrue: [
					outLine := '    register char* localSP SP_REG;'. ].
				(inLine findString: ' currentBytecode;') > 0 ifTrue: [
					outLine := '    register sqInt currentBytecode CB_REG;' ].
				inLine isEmpty ifTrue: [
					"reached end of variables"
					inInterpretVars := false.
					outLine := '    JUMP_TABLE;'. ] ]
			ifFalse: [
			inInterpret ifTrue: [
				"working inside interpret(); translate the switch statement"
				(inLine beginsWith: '		case ') ifTrue: [
					| caseLabel |
					caseLabel := (inLine findTokens: '	 :') second.
					outLine := '		CASE(', caseLabel, ')' ].
				inLine = '			break;' ifTrue: [
					outLine := '			BREAK;' ].
				inLine = '}' ifTrue: [
					"all finished with interpret()"
					inInterpret := false. ] ]
			ifFalse: [
			beforePrimitiveResponse ifTrue: [
				(inLine beginsWith: 'sqInt primitiveResponse(') ifTrue: [
					"into primitiveResponse we go"
					beforePrimitiveResponse := false.
					inPrimitiveResponse := true.
					extraOutLine := '    PRIM_TABLE;'.  ] ]
			ifFalse: [
			inPrimitiveResponse ifTrue: [
				(inLine = '	switch (primitiveIndex) {') ifTrue: [
					extraOutLine := outLine.
					outLine := '	PRIM_DISPATCH;' ].
				(inLine = '	switch (foo->primitiveIndex) {') ifTrue: [
					extraOutLine := outLine.
					outLine := '	PRIM_DISPATCH;' ].
				(inLine beginsWith: '	case ') ifTrue: [
					| caseLabel |
					caseLabel := (inLine findTokens: '	 :') second.
					outLine := '	CASE(', caseLabel, ')' ].
				inLine = '}' ifTrue: [
					inPrimitiveResponse := false ] ].
			] ] ] ].

			outFileStream nextPutAll: outLine; cr.
			extraOutLine ifNotNil: [
				outFileStream nextPutAll: extraOutLine; cr ]]].

		outFileStream close! !

!Gnuifier methodsFor: 'as yet unclassified' stamp: 'acg 12/30/2001 14:12'!
setDirectory: aFileDirectory

	directory _ aFileDirectory! !


!Gnuifier class methodsFor: 'as yet unclassified' stamp: 'acg 12/30/2001 14:13'!
on: aFilePathString

	^self new setDirectory: (FileDirectory on: aFilePathString)! !
