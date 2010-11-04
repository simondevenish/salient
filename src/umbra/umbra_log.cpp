/*
* Umbra
* Copyright (c) 2009 Mingos
* All rights reserved.
*
* Redistribution and use in source and binary forms, with or without
* modification, are permitted provided that the following conditions are met:
*     * Redistributions of source code must retain the above copyright
*       notice, this list of conditions and the following disclaimer.
*     * Redistributions in binary form must reproduce the above copyright
*       notice, this list of conditions and the following disclaimer in the
*       documentation and/or other materials provided with the distribution.
*     * The name of Mingos may not be used to endorse or promote products
*       derived from this software without specific prior written permission.
*
* THIS SOFTWARE IS PROVIDED BY MINGOS ``AS IS'' AND ANY
* EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
* WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
* DISCLAIMED. IN NO EVENT SHALL MINGOS BE LIABLE FOR ANY
* DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
* (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
* LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND
* ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
* (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
* SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#include "umbra.hpp"
#include <stdarg.h>
#include <stdio.h>

FILE * UmbraLog::out = NULL;
int UmbraLog::indent = 0;

const char * logTypeString[] = {
	"INF.",
	"NOT.",
	"WAR.",
	"ERR.",
	"FAT."
};

const char * logTypeStringLong[] = {
	"INFO",
	"NOTIFICATION",
	"WARNING",
	"ERROR",
	"FATAL ERROR"
};

const char * resultString[] = {
	"[END BLOCK: FAILURE]",
	"[END BLOCK: SUCCESS]",
	"[END BLOCK]"
};

void UmbraLog::initialise () {
	out = fopen("log.txt","w");
	fprintf(out,UMBRA_TITLE" ver. "UMBRA_VERSION" ("UMBRA_STATUS") Log file, Running time on creation: %dms.\n"
	            "---===---\n"
	            "INF. = INFORMATION. Informative message.\n"
	            "NOT. = NOTICE. Something unexpected that does not affect the program execution.\n"
	            "WAR. = WARNING. An error that may potentially provoke some misbehaviour.\n"
	            "ERR. = ERROR. An error that is guaranteed to provoke some misbehaviour.\n"
	            "FAT. = FATAL ERROR. An error that prevents the program from continuing.\n"
	            "---===---",
	            TCODSystem::getElapsedMilli());
	fflush(out);
}

void UmbraLog::save () {
	indent = 0;
	UmbraLog::info("Log file saved.");
	if (out != NULL) fclose(out);
}

void UmbraLog::output (UmbraLogType type, UmbraLogResult res, const char * str) {
	if (UmbraConfig::logLevel > (UmbraLogLevel)type) return;
	if (out == NULL) {
		initialise();
		if (res >= UMBRA_LOGRESULT_FAILURE && indent == 0) {
			error("UmbraLog::closeBlock | Tried to close a block, but it hasn't been opened in the first place.");
			return;
		}
	}
	UmbraLogMessage * msg = new UmbraLogMessage();
	//create the arrows marking the indent level
	std::string arrows;
	for (int i = 0; i < indent; i++)
		if (i < indent - 1) arrows += "|   ";
		else {
			if (res >= UMBRA_LOGRESULT_FAILURE) arrows += "\\---";
			else arrows += "|   ";
		}
	//ir result is a negative number, then it's not a block close
	if (res < UMBRA_LOGRESULT_FAILURE)
		fprintf(out,"\n%s %06d %s%s", logTypeString[type], TCODSystem::getElapsedMilli(), arrows.c_str(), str);
	//else we're closing a block (no message, just block close notification)
	else
		fprintf(out,"\n%s %06d %s%s", logTypeString[UMBRA_LOGTYPE_INFO], TCODSystem::getElapsedMilli(), arrows.c_str(), resultString[res]);
	fflush(out);
}

void UmbraLog::output (UmbraLogType type, UmbraLogResult res, std::string str) {
	output(type,res,str.c_str());
}

void UmbraLog::openBlock (const char* str, ...) {
	char s[2048];
	va_list ap;
	va_start(ap,str);
	vsprintf(s,str,ap);
	va_end(ap);
	output(UMBRA_LOGTYPE_INFO,(UmbraLogResult)(-1),s);
	++indent;
}

void UmbraLog::openBlock (std::string str) {
	output(UMBRA_LOGTYPE_INFO,(UmbraLogResult)(-1),str);
	++indent;
}

void UmbraLog::info (const char * str, ...) {
	char s[2048];
	va_list ap;
	va_start(ap,str);
	vsprintf(s,str,ap);
	va_end(ap);
	output(UMBRA_LOGTYPE_INFO,(UmbraLogResult)(-1),s);
}

void UmbraLog::info (std::string str) {
	output(UMBRA_LOGTYPE_INFO,(UmbraLogResult)(-1),str);
}

void UmbraLog::warning (const char * str, ...) {
	char s[2048];
	va_list ap;
	va_start(ap,str);
	vsprintf(s,str,ap);
	va_end(ap);
	output(UMBRA_LOGTYPE_WARNING,(UmbraLogResult)(-1),s);
}

void UmbraLog::warning (std::string str) {
	output(UMBRA_LOGTYPE_WARNING,(UmbraLogResult)(-1),str);
}

void UmbraLog::error (const char * str, ...) {
	char s[2048];
	va_list ap;
	va_start(ap,str);
	vsprintf(s,str,ap);
	va_end(ap);
	output(UMBRA_LOGTYPE_ERROR,(UmbraLogResult)(-1),s);
}

void UmbraLog::error (std::string str) {
	output(UMBRA_LOGTYPE_ERROR,(UmbraLogResult)(-1),str);
}

void UmbraLog::fatalError (const char * str, ...) {
	char s[2048];
	va_list ap;
	va_start(ap,str);
	vsprintf(s,str,ap);
	va_end(ap);
	output(UMBRA_LOGTYPE_FATAL,(UmbraLogResult)(-1),s);
}

void UmbraLog::fatalError (std::string str) {
	output(UMBRA_LOGTYPE_FATAL,(UmbraLogResult)(-1),str);
}

void UmbraLog::closeBlock (UmbraLogResult result) {
	output(UMBRA_LOGTYPE_INFO,result,"");
	if (indent > 0) --indent;
}