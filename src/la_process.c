/*
 * =====================================================================================
 *
 *       Filename:  la_process.c
 *    Description:  handle operating system processes
 *         Author:  Stephan Laukien
 *          Email:  software@laukien.com
 *        License:  Simplified BSD License
 *      Copyright:  (c) 2011-2014
 *
 * =====================================================================================
 */

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#ifdef __WIN32
#include <windows.h>
#else
#include <unistd.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <fcntl.h>
#endif
#include "la_array.h"
#include "la_datetime.h"
#include "la_directory.h"
#include "la_file.h"
#include "la_message.h"
#include "la_string.h"
#include "la_process.h"

struct la_process {
	PROCESS_ID id;
	BOOL wait;
	char *name;
	char *dir_temp;
	char *dir_work;
	PROCESS_STATUS status;
	PROCESS_EXIT exit;
	char *file_input;
	char *file_output;
	char *file_error;
#ifdef SYSTEM_OS_TYPE_WINDOWS
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	HANDLE hi;
	HANDLE ho;
	HANDLE he;
#endif
};

char *string_getRandom(size_t size, BOOL upper, BOOL lower, BOOL number, BOOL special) {
	srand(time(NULL));

	int start;
	int stop;
	if (special) {
		start = 32;
		stop = 126;
	} else {
		start = 48;
		stop = 122;
	}

	char *res = (char *)malloc(size + 1);
	if (!res) {
		return NULL;
	}
	
	char c;
	size_t i;
	for (i = 0; i < size; ++i) {
		while (1) {
			c = (rand() % (stop - start)) + start;
			if (special) break;
			if (c >= '0' && c <= '9' && number) break;
			if (c >= 'A' && c <= 'Z' && upper) break;
			if (c >= 'a' && c <= 'z' && lower) break;
		}
		res[i] = c;
	}
	res[size] = '\0';

	return res;
}

char *process_createName() {
	char *ts = datetime_getTimestampAsString();
	char *rs = string_getRandom(10, TRUE, TRUE, TRUE, FALSE);

	char *rnd = (char *)malloc(strlen(ts) + 1 + strlen(rs) + 1);
	sprintf(rnd, "%s_%s", ts, rs);

	free(rs);
	free(ts);

	return rnd;
}

PROCESS *process_new() {
	PROCESS *self = (PROCESS*) malloc(sizeof(PROCESS));
	if (!self) {
		message_error("unable to get memory");
	}
	
	self->id = 0;
	self->wait = TRUE;
	self->dir_work = NULL;
	self->dir_temp = directory_temp();
	self->name = process_createName();
	self->status = PROCESS_STATUS_NONE;
	self->exit = -1;

	size_t dir_len = strlen(self->dir_temp);
	size_t name_len = strlen(self->name);

	self->file_input = (char *)malloc(dir_len + 1 + name_len + 1 + 2 + 1);
	sprintf(self->file_input, "%s%c%s.in", self->dir_temp, DIRECTORY_SEPARATOR_CHAR, self->name);
	unlink(self->file_input);

	self->file_output = (char *)malloc(dir_len + 1 + name_len + 1 + 3 + 1);
	sprintf(self->file_output, "%s%c%s.out", self->dir_temp, DIRECTORY_SEPARATOR_CHAR, self->name);
	unlink(self->file_output);

	self->file_error = (char *)malloc(dir_len + 1 + name_len + 1 + 3 + 1);
	sprintf(self->file_error, "%s%c%s.err", self->dir_temp, DIRECTORY_SEPARATOR_CHAR, self->name);
	unlink(self->file_error);

	return self;
}

void process_free(PROCESS *self) {
	assert(self);

	/* wait if the process is running and the process should wait*/
	if (self->wait && self->status == PROCESS_STATUS_RUNNING) {
#ifdef SYSTEM_OS_TYPE_WINDOWS
		WaitForSingleObject(self->pi.hProcess, INFINITE);
		CloseHandle(self->pi.hProcess);
		CloseHandle(self->pi.hThread);
		CloseHandle(self->he);
		CloseHandle(self->ho);
		CloseHandle(self->hi);
#else
		int status;
		waitpid(self->id, &status, 0);
#endif
	}

#ifdef SYSTEM_OS_TYPE_WINDOWS
		CloseHandle(self->pi.hProcess);
		CloseHandle(self->pi.hThread);
		CloseHandle(self->he);
		CloseHandle(self->ho);
		CloseHandle(self->hi);
#endif

	unlink(self->file_input);
	unlink(self->file_output);
	unlink(self->file_error);

	free(self->name);
	free(self->dir_temp);
	free(self->file_input);
	free(self->file_output);
	free(self->file_error);

	free(self);
	self = NULL;
}

void process_setWait(PROCESS *self, BOOL wait) {
	assert(self);

	self->wait = wait;
}

BOOL process_isWait(PROCESS *self) {
	assert(self);

	return self->wait;
}

void process_wait(PROCESS *self) {
	assert(self);

	if (self->status == PROCESS_STATUS_RUNNING) {
#ifdef SYSTEM_OS_TYPE_WINDOWS
		WaitForSingleObject(self->pi.hProcess, INFINITE);
#else
		int status;                             /* dummy */
		waitpid(self->id, &status, 0);
#endif
	}
}

void process_setWorkDirectory(PROCESS *self, const char *dir) {
	assert(self);
	assert(dir);

	if (self->dir_work) {
		free(self->dir_work);
	}
	self->dir_work = strdup(dir);
}

void process_setTempDirectory(PROCESS *self, const char *dir) {
	assert(self);
	assert(dir);

	if (self->dir_temp) {
		free(self->dir_temp);
	}
	self->dir_temp = strdup(dir);
}

void process_setInputFromString(PROCESS *self, const char *str) {
	assert(self);
	assert(str);

	string_saveToFile(self->file_input, str);
}

void process_setInputFromFile(PROCESS *self, const char *file) {
	assert(self);
	assert(file);

	if (!file_exists(file)) {
		message_error("file not found");
		return;
	}

	file_copy(file, self->file_input, NULL, NULL);
}

void process_execute(PROCESS *self, const char *command) {
	assert(self);

#ifdef SYSTEM_OS_TYPE_WINDOWS
	/* security settings */
	SECURITY_ATTRIBUTES sa;
	ZeroMemory( &sa, sizeof(sa) );
	sa.nLength = sizeof(sa);
	sa.lpSecurityDescriptor = NULL;
	sa.bInheritHandle = TRUE;

	/* stdin */
	self->hi = GetStdHandle(STD_INPUT_HANDLE);
	if (file_exists(self->file_input)) {
		self->hi = CreateFile(self->file_input,
			GENERIC_READ,
			FILE_SHARE_WRITE | FILE_SHARE_READ,
			&sa,
			OPEN_EXISTING,
			FILE_ATTRIBUTE_NORMAL,
			NULL);
		if (self->hi == INVALID_HANDLE_VALUE) {
			self->hi = GetStdHandle(STD_INPUT_HANDLE);
		}
	}

	/* stdout */
	self->ho = CreateFile(self->file_output,
		GENERIC_WRITE,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		&sa,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (self->ho == INVALID_HANDLE_VALUE) {
		self->ho = GetStdHandle(STD_OUTPUT_HANDLE);
	}

	/* stderr */
	self->he = CreateFile(self->file_error,
		GENERIC_WRITE,
		FILE_SHARE_WRITE | FILE_SHARE_READ,
		&sa,
		CREATE_ALWAYS,
		FILE_ATTRIBUTE_NORMAL,
		NULL);
	if (self->he == INVALID_HANDLE_VALUE) {
		self->he = GetStdHandle(STD_ERROR_HANDLE);
	}

	/* set process information*/
	DWORD flags = CREATE_NO_WINDOW | CREATE_BREAKAWAY_FROM_JOB;

	ZeroMemory(&self->pi, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&self->si, sizeof(STARTUPINFO));
	self->si.cb = sizeof(STARTUPINFO); 

	self->si.dwFlags |= STARTF_USESTDHANDLES;
	self->si.hStdInput = self->hi;
	self->si.hStdOutput = self->ho;
	self->si.hStdError = self->he;

	/* call process */
	BOOL ok;
	char *cmdstr = strdup(command);
	ok = CreateProcess(
		NULL,
		cmdstr,
		NULL,
		NULL,
		TRUE,
		flags,
		NULL,
		self->dir_work,
		&self->si,
		&self->pi
	);
	free(cmdstr);
	if (ok) {
		self->status = PROCESS_STATUS_RUNNING;
		self->id = self->pi.dwProcessId;
		if (self->wait) {
			WaitForSingleObject(self->pi.hProcess, INFINITE);
			GetExitCodeProcess(self->pi.hProcess, &self->exit);
			self->status = PROCESS_STATUS_FINISHED;
			CloseHandle(self->pi.hProcess);
			CloseHandle(self->pi.hThread);
			CloseHandle(self->he);
			CloseHandle(self->ho);
			CloseHandle(self->hi);
		}
	} else {
		self->exit = ok;
		self->status = PROCESS_STATUS_ERROR;
	}
#else
	int status;
	self->id = fork();

    if (self->id < 0) {                         /* error */
		message_error("unable to fork process");
		return;
	}

    if (self->id > 0) {                         /* parent */
		self->status = PROCESS_STATUS_RUNNING;
		if (self->wait) {
			waitpid(self->id, &status, 0);
			self->exit = status;
			self->status = PROCESS_STATUS_FINISHED;
		} else {                                /* wait for child */
			int dummy;
			waitpid(self->id, &dummy, WNOHANG);
		}
	} else {                                    /* child */
		if (file_exists(self->file_input)) {
			int fd_i = open(self->file_input, O_RDONLY);
			dup2(fd_i, 0);
			close(fd_i);
		}

		int fd_o = open(self->file_output, O_WRONLY | O_CREAT | S_IRUSR | S_IWUSR, 0666);
		dup2(fd_o, 1);
		close(fd_o);

		int fd_e = open(self->file_error, O_WRONLY | O_CREAT | S_IRUSR | S_IWUSR, 0666);
		dup2(fd_e, 2);
		close(fd_e);

		/* change working directory */
		if (self->dir_work) {
			chdir(self->dir_work);
		}

		/* format command */
//		execl("./test", "process","arg1", "arg2",  NULL);
		char *cmdarg;
		char *cmdstr;
		const char *sep = strchr(command, ' ');
		if (sep) {
			int seplen = (sep - command) + 1;
			cmdstr = (char *)malloc(seplen - 1 + 1);
			memcpy(cmdstr, command, seplen - 1);
			cmdstr[seplen - 1] = '\0';

			cmdarg = strdup(command);
		} else {
			cmdstr = strdup(command);

			cmdarg = strdup(command);
		}
	
		ARRAY *cmdary = array_new();
		array_loadFromString(cmdary, cmdarg, ' ');
		char **args = array_getArray(cmdary);
		array_free(cmdary);

//		char *args[3] = {"proc", "NULL", NULL};
		int rc = execv(cmdstr, args);

		/* in the case of an error */
		array_freeArray(args);
		free(cmdarg);
		free(cmdstr);

		close(0);
		close(1);
		close(2);

		exit(rc);
	}
#endif
}

PROCESS_ID process_getId(PROCESS *self) {
	assert(self);

	return self->id;
}

PROCESS_EXIT process_getExit(PROCESS *self) {
	assert(self);

	return self->exit;
}

PROCESS_STATUS process_getStatus(PROCESS *self) {
	assert(self);
	if (self->status == PROCESS_STATUS_RUNNING) {
#ifdef SYSTEM_OS_TYPE_WINDOWS
		DWORD rc = WaitForSingleObject(self->pi.hProcess, 1);
		if (rc == WAIT_OBJECT_0) {
			GetExitCodeProcess(self->pi.hProcess, &self->exit);
			self->status = PROCESS_STATUS_FINISHED;
		} else if (rc == WAIT_FAILED) {
			self->status = PROCESS_STATUS_ERROR;
		}
#else
		int stat;
		waitpid(self->id, &stat, WNOHANG);
		if (WIFEXITED(stat)) {
			if (WEXITSTATUS(stat) == 255) {
				self->status = PROCESS_STATUS_ERROR;
			} else {
				self->exit = WEXITSTATUS(stat);
				self->status = PROCESS_STATUS_FINISHED;
			}
		} else if (WIFSTOPPED(stat)) {
			self->status = PROCESS_STATUS_TERMINATED;
		}
#endif
		
	}
	return self->status;
}

char *process_getInputAsString(PROCESS *self) {
	assert(self);

	if (file_exists(self->file_input)) {
		return string_loadFromFile(self->file_input);
	} else {
		return NULL;
	}
}

char *process_getInputAsFilename(PROCESS *self) {
	assert(self);

	return strdup(self->file_input);
}

char *process_getOutputAsString(PROCESS *self) {
	assert(self);

	if (file_exists(self->file_output)) {
		return string_loadFromFile(self->file_output);
	} else {
		return NULL;
	}
}

char *process_getOutputAsFilename(PROCESS *self) {
	assert(self);

	return strdup(self->file_output);
}

char *process_getErrorAsString(PROCESS *self) {
	assert(self);

	if (file_exists(self->file_error)) {
		return string_loadFromFile(self->file_error);
	} else {
		return NULL;
	}
}

char *process_getErrorAsFilename(PROCESS *self) {
	assert(self);

	return strdup(self->file_error);
}

#ifdef __cplusplus
namespace la {
	namespace process {
		Process::Process() {
			this->obj = process_new();
		}

		Process::~Process() {
			process_free(this->obj);
		}

		void Process::setWait(bool wait) {
			process_setWait(this->obj, wait);
		}

		bool Process::isWait() {
			return process_isWait(this->obj);
		}

		void Process::wait() {
			return process_wait(this->obj);
		}

		void Process::setTempDirectory(const std::string &dir) {
			process_setTempDirectory(this->obj, dir.c_str());
		}

		void Process::setInputFromString(const std::string &str) {
			process_setInputFromString(this->obj, str.c_str());
		}

		void Process::setInputFromFile(const std::string &file) {
			process_setInputFromFile(this->obj, file.c_str());
		}

		void Process::execute(const std::string &command) {
			process_execute(this->obj, command.c_str());
		}

		PROCESS_ID Process::getId() {
			return process_getId(this->obj);
		}

		PROCESS_EXIT Process::getExit() {
			return process_getExit(this->obj);
		}

		PROCESS_STATUS Process::getStatus() {
			return process_getStatus(this->obj);
		}

		std::string Process::getInputAsString() {
			char *tmp = process_getInputAsString(this->obj);
			std::string res = tmp;
			free(tmp);

			return res;
		}

		std::string Process::getInputAsFilename() {
			char *tmp = process_getInputAsFilename(this->obj);
			std::string res = tmp;
			free(tmp);

			return res;
		}

		std::string Process::getOutputAsString() {
			char *tmp = process_getOutputAsString(this->obj);
			std::string res = tmp;
			free(tmp);

			return res;
		}

		std::string Process::getOutputAsFilename() {
			char *tmp = process_getOutputAsFilename(this->obj);
			std::string res = tmp;
			free(tmp);

			return res;
		}

		std::string Process::getErrorAsString() {
			char *tmp = process_getErrorAsString(this->obj);
			std::string res = tmp;
			free(tmp);

			return res;
		}

		std::string Process::getErrorAsFilename() {
			char *tmp = process_getErrorAsFilename(this->obj);
			std::string res = tmp;
			free(tmp);

			return res;
		}
	}
}
#endif
