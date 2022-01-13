/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   StatisticsCollector.cpp
 * Author: rafael.luiz.cancian
 * 
 * Created on 30 de Agosto de 2018, 17:24
 */

#include "StatisticsCollector.h"
#include "../TraitsKernel.h"

//using namespace GenesysKernel;


typedef TraitsKernel<ModelComponent>::StatisticsCollector_StatisticsImplementation StatisticsClass;

StatisticsCollector::StatisticsCollector(Model* model, std::string name, ModelElement* parent, bool insertIntoModel) : ModelElement(model, Util::TypeOf<StatisticsCollector>(), name, insertIntoModel) {
	_parent = parent;
	_initStaticsAndCollector();
	_addSimulationResponses();
}

void StatisticsCollector::_addSimulationResponses() {
	GetterMember getterMemberAverage = DefineGetterMember<StatisticsClass>(static_cast<StatisticsClass*> (this->_statistics), &StatisticsClass::average);
	SimulationResponse* resp = new SimulationResponse(Util::TypeOf<StatisticsClass>(), getName() + ".average", getterMemberAverage, getName());
	_parentModel->getResponses()->insert(resp);
	// add the halfwidth as response
	GetterMember getterMemberHalfWidth = DefineGetterMember<StatisticsClass>(static_cast<StatisticsClass*> (this->_statistics), &StatisticsClass::halfWidthConfidenceInterval);
	resp = new SimulationResponse(Util::TypeOf<StatisticsClass>(), /*parentName + ":" + */getName() + ".halfWidth", getterMemberHalfWidth, getName());
	_parentModel->getResponses()->insert(resp);
}

void StatisticsCollector::_initStaticsAndCollector() {
	Collector_if* collector = new TraitsKernel<ModelComponent>::StatisticsCollector_CollectorImplementation();
	_statistics = new StatisticsClass(collector); //TraitsKernel<ModelComponent>::StatisticsCollector_StatisticsImplementation(collector);
}

std::string StatisticsCollector::show() {
	std::string parentStr = "";
	if (_parent != nullptr) {
		try {
			parentStr = _parent->getName();
		} catch (...) { // if parent changed or deleted, can cause seg fault
			parentStr = "<<INCONSISTENT>>"; /* \todo: ++*/
		}
	}
	return ModelElement::show() +
			",parent=\"" + parentStr + "\"" +
			",numElements=" + std::to_string(_statistics->numElements());
}

ModelElement* StatisticsCollector::getParent() const {
	return _parent;
}

Statistics_if* StatisticsCollector::getStatistics() const {
	return _statistics;
}

PluginInformation* StatisticsCollector::GetPluginInformation() {
	PluginInformation* info = new PluginInformation(Util::TypeOf<StatisticsCollector>(), &StatisticsCollector::LoadInstance);
	info->setGenerateReport(true);
	return info;
}

ModelElement* StatisticsCollector::LoadInstance(Model* model, std::map<std::string, std::string>* fields) {
	StatisticsCollector* newElement = new StatisticsCollector(model);
	try {
		newElement->_loadInstance(fields);
	} catch (const std::exception& e) {

	}
	return newElement;
}

bool StatisticsCollector::_loadInstance(std::map<std::string, std::string>* fields) {
	bool res = ModelElement::_loadInstance(fields);
	if (res) {
	}
	return res;
}

std::map<std::string, std::string>* StatisticsCollector::_saveInstance(bool saveDefaultValues) {
	std::map<std::string, std::string>* fields = ModelElement::_saveInstance(saveDefaultValues); //Util::TypeOf<StatisticsCollector>());
	std::string parentId = "", parentTypename = "";
	if (this->_parent != nullptr) {
		parentId = std::to_string(_parent->getId());
		parentTypename = _parent->getClassname();
	}
	SaveField(fields, "parentTypename", parentTypename);
	SaveField(fields, "parentId", parentId);
	return fields;
}

bool StatisticsCollector::_check(std::string* errorMessage) {
	// \TODO: To implement!
	*errorMessage += "";
	return true;
}