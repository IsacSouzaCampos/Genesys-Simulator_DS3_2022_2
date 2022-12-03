/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Smart_AutomaticStatisticsCollection.cpp
 * Author: rlcancian
 * 
 * Created on 3 de Setembro de 2019, 18:34
 */

#include "Smart_AutomaticStatisticsCollection.h"

// you have to included need libs

// GEnSyS Simulator
#include "../../../../kernel/simulator/Simulator.h"

// Model Components
#include "../../../../plugins/components/Create.h"
#include "../../../../plugins/components/Process.h"
#include "../../../../plugins/components/Dispose.h"

Smart_AutomaticStatisticsCollection::Smart_AutomaticStatisticsCollection() {
}

/**
 * This is the main function of the application. 
 * It instanciates the simulator, builds a simulation model and then simulate that model.
 */
int Smart_AutomaticStatisticsCollection::main(int argc, char** argv) {
	Simulator* genesys = new Simulator();
	this->setDefaultTraceHandlers(genesys->getTracer());
	this->insertFakePluginsByHand(genesys);
	
        // crete model
	Model* model = genesys->getModels()->newModel();
	PluginManager* plugins = genesys->getPlugins();
	
        Create* create = plugins->newInstance<Create>(model);
        create->setTimeBetweenCreationsExpression("norm(8,9)");
        create->setTimeUnit(Util::TimeUnit::minute);
        create->setEntitiesPerCreation(1);  // Entities per arrival do arena?
        
        Process* process = plugins->newInstance<Process>(model);
	process->getSeizeRequests()->insert(new SeizableItem(plugins->newInstance<Resource>(model)));
	process->setQueueableItem(new QueueableItem(plugins->newInstance<Queue>(model)));
        process->setDelayTimeUnit(Util::TimeUnit::hour);
	process->setDelayExpression("tria(1,2,3)");
        // como definir prioridade?
        // allocation (value added)
        
        
	Dispose* dispose = plugins->newInstance<Dispose>(model);
	
        // connect model components to create a "workflow"
	create->getConnections()->insert(process);
	process->getConnections()->insert(dispose);
	
        // set options, save and simulate
	model->getSimulation()->setReplicationLength(60, Util::TimeUnit::second);
	model->save("./models/Smart_AutomaticStatisticsCollection.gen");
	model->getSimulation()->start();
        
        for(int i = 0; i < 1e9; i++);
	delete genesys;
	return 0;
};
