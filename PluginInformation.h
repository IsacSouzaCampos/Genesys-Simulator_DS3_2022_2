/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   PluginInformation.h
 * Author: rlcancian
 *
 * Created on 9 de Setembro de 2019, 20:02
 */

#ifndef PLUGININFORMATION_H
#define PLUGININFORMATION_H

#include <map>
#include <list>

class ModelElement;
class ModelComponent;
class Model;
class ElementManager;

// TODO: the following 2 types were diffferent but now on they are the same and should be merged
typedef ModelComponent* (*StaticLoaderComponentInstance)(Model*, std::map<std::string, std::string>*);
typedef ModelElement* (*StaticLoaderElementInstance)(Model*, std::map<std::string, std::string>*);
class PluginInformation;
typedef PluginInformation* (*StaticGetPluginInformation)();


class PluginInformation {
public:
    PluginInformation(std::string pluginTypename, StaticLoaderComponentInstance componentloader);
    PluginInformation(std::string pluginTypename, StaticLoaderElementInstance elementloader);
public:
    // gets
    StaticLoaderElementInstance elementloader() const;
    StaticLoaderComponentInstance componentLoader() const;
    bool isGenerateReport() const;
    bool isComponent() const;
    bool isSendTransfer() const;
    bool isReceiveTransfer() const;
    bool isSink() const;
    bool isSource() const;
    std::string observation() const;
    std::string version() const;
    std::string date() const;
    std::string author() const;
    std::string pluginTypename() const;
    // sets
    void insertDynamicLibFileDependence(std::string filename);
    void setDynamicLibFilenameDependencies(std::list<std::string>* dynamicLibFilenameDependencies);
    std::list<std::string>* dynamicLibFilenameDependencies() const;
    void setGenerateReport(bool generateReport);
    void setSendTransfer(bool sendTransfer);
    void setReceiveTransfer(bool receiveTransfer);
    void setSink(bool Sink);
    void setSource(bool Source);
    void setObservation(std::string observation);
    void setVersion(std::string version);
    void setDate(std::string date);
    void setAuthor(std::string author);
public:
private:
    std::string _author = "unknown";
    std::string _date = "01/07/2018";
    std::string _version = "1.0.0";
    std::string _observation = "none";
    bool _isSource = false;
    bool _isSink = false;
    bool _receiveTransfer = false; /*!< If true, an entity can arrive to this component without a phisical connection. In terms of model connection check, it is just like a Source component, since it does not need to have a predecessor */
    bool _sendTransfer = false; /*!< If true, an entity can be sent from this component to another one without a phisical connection. In terms of model connection check, it is just like a Sink component, since it does not need to have a sucessor */
    bool _generateReport = false;
    std::list<std::string>* _dynamicLibFilenameDependencies = new std::list<std::string>();
    // set from constructor
    std::string _pluginTypename;
    bool _isComponent = false;
    StaticLoaderComponentInstance _componentloader;
    StaticLoaderElementInstance _elementloader;
};


#endif /* PLUGININFORMATION_H */

