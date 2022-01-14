/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PluginManager.h
 * Author: rafael.luiz.cancian
 *
 * Created on 30 de Maio de 2019, 17:49
 */

#ifndef PLUGINMANAGER_H
#define PLUGINMANAGER_H

#include "../util/List.h"
#include "Plugin.h"
#include "PluginConnector_if.h"

//namespace GenesysKernel {
class Simulator;

class PluginManager {
public:
	PluginManager(Simulator* simulator);
	virtual ~PluginManager() = default;
public:
	std::string show();
public:
	bool completePluginsFieldsAndTemplates();
public:
	bool check(const std::string dynamicLibraryFilename);
	Plugin* insert(const std::string dynamicLibraryFilename);
	bool remove(const std::string dynamicLibraryFilename);
	bool remove(Plugin* plugin);
	Plugin* find(std::string pluginTypeName);
public:
	Plugin* front();
	Plugin* next();
	Plugin* last();
	unsigned int size();
	Plugin* getAtRank(unsigned int rank);
private:
	bool _insert(Plugin* plugin);
	void _insertDefaultKernelElements();
private:
	List<Plugin*>* _plugins = new List<Plugin*>();
	Simulator* _simulator;
	PluginConnector_if* _pluginConnector;
};
//namespace\\}
#endif /* PLUGINMANAGER_H */

