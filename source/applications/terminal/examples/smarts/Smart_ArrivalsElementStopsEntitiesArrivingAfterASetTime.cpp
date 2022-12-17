/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   Smart_ArrivalsElementStopsEntitiesArrivingAfterASetTime.cpp
 * Author: rlcancian
 * 
 * Created on 3 de Setembro de 2019, 18:34
 */

#include "Smart_ArrivalsElementStopsEntitiesArrivingAfterASetTime.h"

// you have to included need libs

// GEnSyS Simulator
#include "../../../../kernel/simulator/Simulator.h"

// Model Components
#include "../../../../plugins/components/Create.h"
#include "../../../../plugins/components/Assign.h"
#include "../../../../plugins/components/Process.h"
#include "../../../../plugins/components/Decide.h"
#include "../../../../plugins/components/Dispose.h"

Smart_ArrivalsElementStopsEntitiesArrivingAfterASetTime::Smart_ArrivalsElementStopsEntitiesArrivingAfterASetTime() {
}

/**
 * This is the main function of the application. 
 * It instanciates the simulator, builds a simulation model and then simulate that model.
 */
int Smart_ArrivalsElementStopsEntitiesArrivingAfterASetTime::main(int argc, char** argv) {
	Simulator* genesys = new Simulator();
	this->setDefaultTraceHandlers(genesys->getTracer());
	this->insertFakePluginsByHand(genesys);
	
        // crete model
	Model* model = genesys->getModels()->newModel();
	PluginManager* plugins = genesys->getPlugins();
	
        // Modelo
        
        EntityType* entityType = plugins->newInstance<EntityType>(model);
        
        Create* create = plugins->newInstance<Create>(model);
        create->setEntityType(entityType);
        create->setTimeUnit(Util::TimeUnit::minute);
        create->setTimeBetweenCreationsExpression("EXPO(5)");
        
        Resource* resource1 = plugins->newInstance<Resource>(model, "Teller");
        
        Process* process1 = plugins->newInstance<Process>(model, "Work with Teller");
        process1->getSeizeRequests()->insert(new SeizableItem(resource1));
	process1->setQueueableItem(new QueueableItem(plugins->newInstance<Queue>(model)));
        process1->setDelayTimeUnit(Util::TimeUnit::minute);
        process1->setAllocationType(Util::AllocationType::ValueAdded);
	process1->setDelayExpression("tria(1,2,3)");
        process1->setPriority(2);
        
        Decide* decide = plugins->newInstance<Decide>(model);
        decide->getConditions()->insert("DISC(0.25, 1, 1, 0) > 0");
	
        Resource* resource2 = plugins->newInstance<Resource>(model, "Supervisor");
        
        Process* process2 = plugins->newInstance<Process>(model, "Work with Supervisor");
        process2->getSeizeRequests()->insert(new SeizableItem(resource2));
	process2->setQueueableItem(new QueueableItem(plugins->newInstance<Queue>(model)));
        process2->setDelayTimeUnit(Util::TimeUnit::minute);
        process2->setAllocationType(Util::AllocationType::ValueAdded);
	process2->setDelayExpression("tria(5,10,15)");
        process2->setPriority(2);
        
        Dispose* dispose = plugins->newInstance<Dispose>(model);
	
        // connect model components to create a "workflow"
	create->getConnections()->insert(process1);
        process1->getConnections()->insert(decide);
        // True
        decide->getConnections()->insert(process2);
        process2->getConnections()->insert(process1);
        // False
        decide->getConnections()->insert(dispose);
	
        // set options, save and simulate
        ModelSimulation* sim = model->getSimulation();
        
        //sim->setTerminatingCondition("count(Dispose_1.CountNumberIn)>1000");
        sim->setReplicationLength(480);//tem q variar
        sim->setReplicationLengthTimeUnit(Util::TimeUnit::minute);
        sim->setNumberOfReplications(300);
        sim->setWarmUpPeriod(sim->getReplicationLength()*0.05); //5% de 480 = 24
        sim->setWarmUpPeriodTimeUnit(Util::TimeUnit::minute);	
	model->save("./models/Smart_ArrivalsElementStopsEntitiesArrivingAfterASetTime.gen");
	sim->start();
        
        for(int i = 0; i < 1e9; i++);
	delete genesys;
	return 0;
};
