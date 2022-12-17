/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   DefiningResourceCapacity.cpp
 * Author: rlcancian
 * 
 * Created on 3 de Setembro de 2019, 18:34
 */

#include "Smart_DefiningResourceCapacity.h"

// you have to included need libs

// GEnSyS Simulator
#include "../../../../kernel/simulator/Simulator.h"

// Model Components
#include "../../../../plugins/components/Create.h"
#include "../../../../plugins/components/Clone.h"
#include "../../../../plugins/components/Process.h"
#include "../../../../plugins/components/Dispose.h"

Smart_DefiningResourceCapacity::Smart_DefiningResourceCapacity() {
}

/**
 * This is the main function of the application. 
 * It instanciates the simulator, builds a simulation model and then simulate that model.
 */
int Smart_DefiningResourceCapacity::main(int argc, char** argv) {
	Simulator* genesys = new Simulator();
	this->setDefaultTraceHandlers(genesys->getTracer());
	this->insertFakePluginsByHand(genesys);
	
        
        // crete model
	Model* model = genesys->getModels()->newModel();
	PluginManager* plugins = genesys->getPlugins();
	
        Create* create = plugins->newInstance<Create>(model);
        create->setTimeBetweenCreationsExpression("exp(3)");
        create->setTimeUnit(Util::TimeUnit::minute);
        create->setEntitiesPerCreation(1);  // igual EntitiesPerArrival?
        create->setFirstCreation(0);
        //create->setMaxCreations(INFINITY);
        
        
        //Resources
        Resource* bookkeeper1 = plugins->newInstance<Resource>(model);
        bookkeeper1->setCapacity(1);
        //bookkeeper1->setCostBusyTimeUnit(0);
        //bookkeeper1->setCostIdleTimeUnit(0);
  
        Resource* bookkeeper2 = plugins->newInstance<Resource>(model);
        bookkeeper2->setCapacity(4);
        //bookkeeper2->setCostBusyTimeUnit(0);
        //bookkeeper2->setCostIdleTimeUnit(0);
        
        //Queues
        Queue* queue1 = plugins->newInstance<Queue>(model);
        //queue1->setOrderRule(1); // :ModelDataDefinition::OrderRule(1)
        queue1->setReportStatistics(true);
        
        Queue* queue2 = plugins->newInstance<Queue>(model);
        //queue2->setOrderRule(1);
        queue2->setReportStatistics(true);
        
        //Clone
        Clone* clone = plugins->newInstance<Clone>(model);
        clone->setReportStatistics(false);

        //Processes
	Process* process1 = plugins->newInstance<Process>(model);
        process1->getSeizeRequests()->insert(new SeizableItem(bookkeeper1));
	process1->setQueueableItem(new QueueableItem(queue1));
        process1->setPriority(2);
        process1->setDelayTimeUnit(Util::TimeUnit::minute);
        process1->setDelayExpression("tria(0.5,5,10)");
        process1->isReportStatistics();
        
        Process* process2 = plugins->newInstance<Process>(model);
        process2->getSeizeRequests()->insert(new SeizableItem(bookkeeper2));
        process2->setQueueableItem(new QueueableItem(queue2));        // ...
        process2->setPriority(2);
        process2->setDelayTimeUnit(Util::TimeUnit::minute);
        process2->setDelayExpression("tria(0.5,5,10)");
        process2->isReportStatistics();
        
        
        Dispose* dispose1 = plugins->newInstance<Dispose>(model);
        Dispose* dispose2 = plugins->newInstance<Dispose>(model);
	
        
        // connect model components to create a "workflow"
	create->getConnections()->insert(clone);
        
        clone->getConnections()->insert(process1);
        clone->getConnections()->insert(process2);
        
        process1->getConnections()->insert(dispose1);
        process2->getConnections()->insert(dispose2);
	
        
        // set options, save and simulate
        ModelSimulation* sim = model->getSimulation();
        
        //sim->setTerminatingCondition("count(Dispose_1.CountNumberIn)>1000");
        sim->setReplicationLength(480);//tem q variar
        sim->setReplicationLengthTimeUnit(Util::TimeUnit::minute);
        sim->setNumberOfReplications(300);
        sim->setWarmUpPeriod(sim->getReplicationLength()*0.05); //5% de 480 = 24
        sim->setWarmUpPeriodTimeUnit(Util::TimeUnit::minute);	
        model->save("./models/Smart_DefiningResourceCapacity.gen");
	sim->start();
        
        for(int i = 0; i < 1e9; i++);
	delete genesys;
	return 0;
};