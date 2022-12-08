/*
 * Click nbfs://nbhost/SystemFileSystem/Templates/Licenses/license-default.txt to change this license
 * Click nbfs://nbhost/SystemFileSystem/Templates/cppFiles/class.cc to edit this template
 */

/* 
 * File:   DynamicLinkedCode.cpp
 * Author: rlcancian
 * 
 * Created on 11 de janeiro de 2022, 22:24
 */

#include "CppCompiler.h"

#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/param.h>
#include <grp.h>
//
#include <cstdlib>
//
#include <iostream>
#include <cstdio>
//
#include <bits/stdc++.h> 

#include "../../kernel/simulator/Model.h"


#ifdef PLUGINCONNECT_DYNAMIC

extern "C" StaticGetPluginInformation getPluginInformation() {
	return &CppCompiler::getPluginInformation;
}
#endif

ModelDataDefinition* CppCompiler::NewInstance(Model* model, std::string name) {
	return new CppCompiler(model, name);
}

CppCompiler::CppCompiler(Model* model, std::string name) : ModelDataDefinition(model, Util::TypeOf<CppCompiler>(), name) {
}

// static 

ModelDataDefinition* CppCompiler::LoadInstance(Model* model, PersistenceRecord *fields) {
	CppCompiler* newElement = new CppCompiler(model);
	try {
		newElement->_loadInstance(fields);
	} catch (const std::exception& e) {

	}
	return newElement;
}

PluginInformation* CppCompiler::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<CppCompiler>(), &CppCompiler::LoadInstance, &CppCompiler::NewInstance);
	info->setDescriptionHelp("//@TODO");
	//info->setObservation("");
	//info->setMinimumOutputs();
	//info->setDynamicLibFilenameDependencies();
	//info->setFields();
	// ...
	return info;
}

//

std::string CppCompiler::show() {
	return ModelDataDefinition::show();
}

void CppCompiler::setSourceFilename(std::string _code) {
	this->_sourceFilename = _code;
}

std::string CppCompiler::getSourceFilename() const {
	return _sourceFilename;
}
// must be overriden 

bool CppCompiler::_loadInstance(PersistenceRecord *fields) {
	bool res = ModelDataDefinition::_loadInstance(fields);
	if (res) {
		// @TODO: not implemented yet
	}
	return res;
}

void CppCompiler::_saveInstance(PersistenceRecord *fields, bool saveDefaultValues) {
	ModelDataDefinition::_saveInstance(fields, saveDefaultValues);
	// @TODO: not implemented yet
}

// could be overriden 

bool CppCompiler::_check(std::string* errorMessage) {
	//@ TODO check if compiler command exists
	return true;
}

void CppCompiler::_createInternalAndAttachedData() {
}

void CppCompiler::_initBetweenReplications() {

}

// -----------------------------------------

CppCompiler::CompilationResult CppCompiler::_invokeCompiler(std::string command) {
	CppCompiler::CompilationResult result;
	_parentModel->getTracer()->trace("Invoking compiler: " + command);
	bool secureInvoke = false;
	if (!secureInvoke) {
		try {
			//std::system(command.c_str());
			FILE *handle = popen(command.c_str(), "r");
			if (handle == NULL) {
				result.success = false;
				result.generalMessage = "Error invoking compiler";
			} else {
				char buf[64];
				size_t readn;
				std::string compilOutput = "";
				while ((readn = fread(buf, 1, sizeof (buf), handle)) > 0) {
					//compilOutput += buf.
					fwrite(buf, 1, readn, stdout);
				}
				pclose(handle);
				// compiler executed (compilation successfull or not)
			}
		} catch (const std::exception& e) {
			result.success = false;
			result.generalMessage = "Error invoking compiler";
		}
	} else {
		_SPC_PIPE *pipe = this->_spc_popen(command.c_str(), nullptr, nullptr);
		if (pipe != NULL) {
			std::array<char, 128> buffer;
			std::string compilOutput;
			while (fgets(buffer.data(), 128, pipe->read_fd) != NULL) {
				std::cout << "Reading..." << std::endl;
				compilOutput += buffer.data();
			}
			this->_spc_pclose(pipe);
			result.success = true;
			result.compilationOutput = compilOutput;
		} else {
			result.success = false;
			result.generalMessage = "Error invoking compiler";
		}
	}

}

CppCompiler::CompilationResult CppCompiler::compileToExecutable(std::string sourceFilename) {
	CppCompiler::CompilationResult result;
	Util::FileDelete(this->_outputFilename);
	std::string command(_compilerCommand + " " + _flagsGeneral + " " + _flagsExecutable + " " + _objectFiles + " " + _sourceFilename + " -o " + _outputFilename);
	result = _invokeCompiler(command);
	return result;
}

CppCompiler::CompilationResult CppCompiler::compileToDynamicLibrary(std::string sourceFilename) {
	CppCompiler::CompilationResult result;
	Util::FileDelete(this->_outputFilename);
	std::string command(_compilerCommand + " " + _flagsGeneral + " " + _flagsDynamicLibrary + " " + _objectFiles + " " + _sourceFilename + " -o " + _outputFilename);
	result = _invokeCompiler(command);
	return result;
}

CppCompiler::CompilationResult CppCompiler::compileToStaticLibrary(std::string sourceFilename) {
	CppCompiler::CompilationResult result;
	Util::FileDelete(this->_outputFilename);
	std::string command(_compilerCommand + " " + _flagsGeneral + " " + _flagsStaticLibrary + " " + _objectFiles + " " + _sourceFilename + " -o " + _outputFilename);
	result = _invokeCompiler(command);
	return result;
}

CppCompiler::CompilationResult CppCompiler::loadLibrary(std::string libraryFilename) {
	CppCompiler::CompilationResult result;
	return result;
}

CppCompiler::CompilationResult CppCompiler::unloadLibrary(std::string libraryFilename) {
	CppCompiler::CompilationResult result;
	return result;
}

void CppCompiler::setObjectFiles(std::string _objectFiles) {
	this->_objectFiles = _objectFiles;
}

std::string CppCompiler::getObjectFiles() const {
	return _objectFiles;
}

void CppCompiler::setFlagsExecutable(std::string _flagsExecutable) {
	this->_flagsExecutable = _flagsExecutable;
}

std::string CppCompiler::getFlagsExecutable() const {
	return _flagsExecutable;
}

void CppCompiler::setLibraryLoaded(bool libraryLoaded) {
	this->libraryLoaded = libraryLoaded;
}

void CppCompiler::setFlagsStaticLibrary(std::string _flagsStaticLibrary) {
	this->_flagsStaticLibrary = _flagsStaticLibrary;
}

std::string CppCompiler::getFlagsStaticLibrary() const {
	return _flagsStaticLibrary;
}

void CppCompiler::setFlagsDynamicLibrary(std::string _flagsDynamicLibrary) {
	this->_flagsDynamicLibrary = _flagsDynamicLibrary;
}

std::string CppCompiler::getFlagsDynamicLibrary() const {
	return _flagsDynamicLibrary;
}

void CppCompiler::setFlagsGeneral(std::string _flagsGeneral) {
	this->_flagsGeneral = _flagsGeneral;
}

std::string CppCompiler::getFlagsGeneral() const {
	return _flagsGeneral;
}

void CppCompiler::setCompilerCommand(std::string _compilerCommand) {
	this->_compilerCommand = _compilerCommand;
}

std::string CppCompiler::getCompilerCommand() const {
	return _compilerCommand;
}

void CppCompiler::setOutputDir(std::string _outputDir) {
	this->_outputDir = _outputDir;
}

std::string CppCompiler::getOutputDir() const {
	return _outputDir;
}

void CppCompiler::setTempDir(std::string _tempDir) {
	this->_tempDir = _tempDir;
}

std::string CppCompiler::getTempDir() const {
	return _tempDir;
}

void CppCompiler::setOutputFilename(std::string _outputFilename) {
	this->_outputFilename = _outputFilename;
}

std::string CppCompiler::getOutputFilename() const {
	return _outputFilename;
}

bool CppCompiler::IsLibraryLoaded() const {
	return libraryLoaded;
}

//--------------------------------------------------------------

/*static*/ int CppCompiler::_orig_ngroups = -1;
/*static*/ gid_t CppCompiler::_orig_gid = -1;
/*static*/ uid_t CppCompiler::_orig_uid = -1;
/*static*/ gid_t CppCompiler::_orig_groups[NGROUPS_MAX];

void CppCompiler::_spc_drop_privileges(int permanent) {
	gid_t newgid = getgid(), oldgid = getegid();
	uid_t newuid = getuid(), olduid = geteuid();

	if (!permanent) {
		/* Save information about the privileges that are being dropped so that they
		 * can be restored later.
		 */
		_orig_gid = oldgid;
		_orig_uid = olduid;
		_orig_ngroups = getgroups(NGROUPS_MAX, CppCompiler::_orig_groups);
	}

	/* If root privileges are to be dropped, be sure to pare down the ancillary
	 * groups for the process before doing anything else because the setgroups(  )
	 * system call requires root privileges.  Drop ancillary groups regardless of
	 * whether privileges are being dropped temporarily or permanently.
	 */
	if (!olduid) setgroups(1, &newgid);

	if (newgid != oldgid) {
#if !defined(linux)
		setegid(newgid);
		if (permanent && setgid(newgid) == -1) abort();
#else
		if (setregid((permanent ? newgid : -1), newgid) = = -1) abort();
#endif
	}

	if (newuid != olduid) {
#if !defined(linux)
		seteuid(newuid);
		if (permanent && setuid(newuid) == -1) abort();
#else
		if (setreuid((permanent ? newuid : -1), newuid) = = -1) abort();
#endif
	}

	/* verify that the changes were successful */
	if (permanent) {
		if (newgid != oldgid && (setegid(oldgid) != -1 || getegid() != newgid))
			abort();
		if (newuid != olduid && (seteuid(olduid) != -1 || geteuid() != newuid))
			abort();
	} else {
		if (newgid != oldgid && getegid() != newgid) abort();
		if (newuid != olduid && geteuid() != newuid) abort();
	}
}

void CppCompiler::_spc_restore_privileges(void) {
	if (geteuid() != _orig_uid)
		if (seteuid(_orig_uid) == -1 || geteuid() != _orig_uid) abort();
	if (getegid() != _orig_gid)
		if (setegid(_orig_gid) == -1 || getegid() != _orig_gid) abort();
	if (!_orig_uid)
		setgroups(_orig_ngroups, _orig_groups);
}

//--------

void CppCompiler::_spc_sanitize_files(void) {
	int fd, fds;
	struct stat st;

	/* Make sure all open descriptors other than the standard ones are closed */
	if ((fds = getdtablesize()) == -1) fds = OPEN_MAX;
	for (fd = 3; fd < fds; fd++) close(fd);

	/* Verify that the standard descriptors are open.  If they're not, attempt to
	 * open them using /dev/null.  If any are unsuccessful, abort.
	 */
	for (fd = 0; fd < 3; fd++)
		if (fstat(fd, &st) == -1 && (errno != EBADF || !_open_devnull(fd))) abort();
}

/*static*/ int CppCompiler::_open_devnull(int fd) {
	FILE *f = 0;
	if (!fd) f = freopen(_PATH_DEVNULL, "rb", stdin);
	else if (fd == 1) f = freopen(_PATH_DEVNULL, "wb", stdout);
	else if (fd == 2) f = freopen(_PATH_DEVNULL, "wb", stderr);
	return (f && fileno(f) == fd);
}

//--------

pid_t CppCompiler::_spc_fork(void) {
	pid_t childpid;

	if ((childpid = fork()) == -1) return -1;

	/* Reseed PRNGs in both the parent and the child */
	/* See Chapter 11 for examples */

	/* If this is the parent process, there's nothing more to do */
	if (childpid != 0) return childpid;

	/* This is the child process */
	_spc_sanitize_files(); /* Close all open files.  See Recipe 1.1 */
	_spc_drop_privileges(1); /* Permanently drop privileges.  See Recipe 1.3 */

	return 0;
}

CppCompiler::_SPC_PIPE * CppCompiler::_spc_popen(const char *path, char *const argv[], char *const envp[]) {
	int stdin_pipe[2], stdout_pipe[2];
	_SPC_PIPE *p;

	if (!(p = (_SPC_PIPE *) malloc(sizeof (_SPC_PIPE)))) return 0;
	p->read_fd = p->write_fd = 0;
	p->child_pid = -1;

	if (pipe(stdin_pipe) == -1) {
		free(p);
		return 0;
	}
	if (pipe(stdout_pipe) == -1) {
		close(stdin_pipe[1]);
		close(stdin_pipe[0]);
		free(p);
		return 0;
	}
	if (!(p->read_fd = fdopen(stdout_pipe[0], "r"))) {
		close(stdout_pipe[1]);
		close(stdout_pipe[0]);
		close(stdin_pipe[1]);
		close(stdin_pipe[0]);
		free(p);
		return 0;
	}
	if (!(p->write_fd = fdopen(stdin_pipe[1], "w"))) {
		fclose(p->read_fd);
		close(stdout_pipe[1]);
		close(stdin_pipe[1]);
		close(stdin_pipe[0]);
		free(p);
		return 0;
	}
	if ((p->child_pid = _spc_fork()) == -1) {
		fclose(p->write_fd);
		fclose(p->read_fd);
		close(stdout_pipe[1]);
		close(stdin_pipe[0]);
		free(p);
		return 0;
	}
	if (!p->child_pid) {
		/* this is the child process */
		close(stdout_pipe[0]);
		close(stdin_pipe[1]);
		if (stdin_pipe[0] != 0) {
			dup2(stdin_pipe[0], 0);
			close(stdin_pipe[0]);
		}
		if (stdout_pipe[1] != 1) {
			dup2(stdout_pipe[1], 1);
			close(stdout_pipe[1]);
		}
		execve(path, argv, envp);
		exit(127);
	}
	close(stdout_pipe[1]);
	close(stdin_pipe[0]);
	return p;
}

int CppCompiler::_spc_pclose(_SPC_PIPE * p) {
	int status;
	pid_t pid;

	if (p->child_pid != -1) {
		do {
			pid = waitpid(p->child_pid, &status, 0);
		} while (pid == -1 && errno == EINTR);
	}
	if (p->read_fd) fclose(p->read_fd);
	if (p->write_fd) fclose(p->write_fd);
	free(p);
	if (pid != -1 && WIFEXITED(status)) return WEXITSTATUS(status);
	else return (pid == -1 ? -1 : 0);
}
