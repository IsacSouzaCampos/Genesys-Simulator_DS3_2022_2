/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Dummy.cpp
 * Author: rafael.luiz.cancian
 * 
 * Created on 22 de Maio de 2019, 18:41
 */

#include "Dummy.h"
#include "../../kernel/simulator/Model.h"

Dummy::Dummy(Model* model, std::string name) : ModelComponent(model, Util::TypeOf<Dummy>(), name) {
}

std::string Dummy::show() {
	return ModelComponent::show() + "";
}

ModelComponent* Dummy::LoadInstance(Model* model, std::map<std::string, std::string>* fields) {
	Dummy* newComponent = new Dummy(model);
	try {
		newComponent->_loadInstance(fields);
	} catch (const std::exception& e) {

	}
	return newComponent;
}

void Dummy::_execute(Entity* entity) {
	_parentModel->getTracer()->trace("I'm just a dummy model and I'll just send the entity forward");
	this->_parentModel->sendEntityToComponent(entity, this->getNextComponents()->getFrontConnection());
}

bool Dummy::_loadInstance(std::map<std::string, std::string>* fields) {
	bool res = ModelComponent::_loadInstance(fields);
	if (res) {
		// \todo: not implemented yet
	}
	return res;
}

void Dummy::_initBetweenReplications() {
}

std::map<std::string, std::string>* Dummy::_saveInstance() {
	std::map<std::string, std::string>* fields = ModelComponent::_saveInstance();
	// \todo: not implemented yet
	return fields;
}

bool Dummy::_check(std::string* errorMessage) {
	bool resultAll = true;
	// \todo: not implemented yet
	*errorMessage += "";
	return resultAll;
}

PluginInformation* Dummy::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<Dummy>(), &Dummy::LoadInstance);
	// ...
	return info;
}
