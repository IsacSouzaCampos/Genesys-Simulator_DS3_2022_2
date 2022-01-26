/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Smart_RouteStation.cpp
 * Author: rlcancian
 * 
 * Created on 3 de Setembro de 2019, 18:34
 */

#include "Smart_RouteStation.h"

// you have to included need libs

// GEnSyS Simulator
#include "../../kernel/simulator/Simulator.h"

// Model Components
#include "../../plugins/components/Create.h"
#include "../../plugins/components/Route.h"
#include "../../plugins/components/Enter.h"
#include "../../plugins/components/Delay.h"
#include "../../plugins/components/Dispose.h"

// Model data definitions
//#include "../../kernel/simulator/EntityType.h"

Smart_RouteStation::Smart_RouteStation() {
}

/**
 * This is the main function of the application. 
 * It instanciates the simulator, builds a simulation model and then simulate that model.
 */
int Smart_RouteStation::main(int argc, char** argv) {
	Simulator* genesys = new Simulator();
	this->insertFakePluginsByHand(genesys);
	this->setDefaultTraceHandlers(genesys->getTracer());
	genesys->getTracer()->setTraceLevel(Util::TraceLevel::L9_mostDetailed);
	Model* model = genesys->getModels()->newModel();
	PluginManager* plugins = genesys->getPlugins();
	Create* create1 = plugins->newInstance<Create>(model);
	create1->setEntityType(plugins->newInstance<EntityType>(model));
	create1->setTimeBetweenCreationsExpression("4.0");
	Station* station1 = plugins->newInstance<Station>(model);
	Route* route1 = plugins->newInstance<Route>(model);
	route1->setStation(station1);
	route1->setRouteTimeExpression("0.1");
	//
	Enter* enter1 = plugins->newInstance<Enter>(model);
	enter1->setStation(station1);
	Delay* delay1 = plugins->newInstance<Delay>(model);
	Station* station2 = plugins->newInstance<Station>(model);
	Route* route2 = plugins->newInstance<Route>(model);
	route2->setStation(station2);
	route2->setRouteTimeExpression("0.2");
	//
	Enter* enter2 = plugins->newInstance<Enter>(model);
	enter2->setStation(station2);
	Delay* delay2 = plugins->newInstance<Delay>(model);
	Station* station3 = plugins->newInstance<Station>(model);
	Route* route3 = plugins->newInstance<Route>(model);
	route3->setStation(station3);
	route3->setRouteTimeExpression("0.3");
	//
	Enter* enter3 = plugins->newInstance<Enter>(model);
	enter3->setStation(station3);
	Dispose* dispose1 = plugins->newInstance<Dispose>(model);
	//
	create1->getConnections()->insert(route1);
	enter1->getConnections()->insert(delay1);
	delay1->getConnections()->insert(route2);
	enter2->getConnections()->insert(delay2);
	delay2->getConnections()->insert(route3);
	enter3->getConnections()->insert(dispose1);
	//
	model->getSimulation()->setReplicationLength(20);
	model->save("./models/Smart_RouteStation.gen");
	/*
	do {
		model->getSimulation()->step();
		std::cin.ignore(std::numeric_limits <std::streamsize> ::max(), '\n');
	} while (model->getSimulation()->isPaused());
	 */
	model->getSimulation()->start();
	return 0;
};

