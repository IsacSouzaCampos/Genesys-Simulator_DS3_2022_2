/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   GenesysShell.h
 * Author: rlcancian
 *
 * Created on 23 de Maio de 2019, 13:02
 */

#ifndef GENESYSSHELL_H
#define GENESYSSHELL_H

#include "GenesysApplication_if.h"
#include "Simulator.h"
#include "List.h"

class GenesysShell : public GenesysApplication_if {
private:
    typedef std::function<void() > ExecuterMember;

    template<typename Class>
    ExecuterMember DefineExecuterMember(Class * object, void (Class::*function)()) {
        return std::bind(function, object); //, std::placeholders::_1
    }

    class ShellCommand {
    public:

        ShellCommand(std::string shortname, std::string longname, std::string descrition, ExecuterMember executer) {
            this->descrition = descrition;
            this->longname = longname;
            this->shortname = shortname;
            this->executer = executer;
        }
        std::string shortname;
        std::string longname;
        std::string descrition;
        ExecuterMember executer;
    };
public:
    GenesysShell();
    GenesysShell(const GenesysShell& orig);
    virtual ~GenesysShell();
public:
    virtual int main(int argc, char** argv);
public: // commands
    void cmdHelp();
    void cmdQuit();
    void cmdModelLoad();
private:
    void run(List<std::string>* arguments);
    void Trace(std::string message);
private:
    Simulator* _simulator = new Simulator();
    std::vector<std::string>* _parameters;
    List<ShellCommand*>* _commands = new List<ShellCommand*>();
    std::string _prompt = "$ReGenesys> ";
};

#endif /* GENESYSSHELL_H */

