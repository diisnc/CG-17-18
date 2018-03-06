//
//  main.h
//  Motor
//
//  Created by Tiago Conceição on 14/03/14.
//  Copyright (c) 2014 Tiago Conceição. All rights reserved.
//

#ifndef __Motor__main__
#define __Motor__main__

#include <stdio.h>
#include <string.h>
#include <iostream>
#include <fstream>
#include <string>
#include <list>
#include "./tinyxml/tinyxml.h"
#include "./tinyxml/tinystr.h"

using namespace std;

class Ponto
{
public:
    float x;
    float y;
    float z;
};

class Modelo
{
public:
    string ficheiro;
    list<Ponto> pontos;
    
    Modelo() { this->ficheiro = "untitled"; this->pontos = list<Ponto>(); }
    
    Modelo(const string& ficheiro, list<Ponto> pontos) { this->ficheiro = ficheiro; this->pontos = pontos; }
};

class Cena
{
public:
    list<Modelo> modelos;
    
    Cena() { this->modelos = list<Modelo>(); }
    
    Cena(list<Modelo> modelos) { this->modelos = modelos; }
    
    void load(const char* pFilename);
};

void Cena::load(const char* pFilename)
{
	TiXmlDocument doc(pFilename);
	if (!doc.LoadFile()) return;
    TiXmlElement* root = doc.RootElement( );
    
	
    for (TiXmlElement* pModeloNode = root->FirstChild("modelo")->ToElement() ; pModeloNode; pModeloNode=pModeloNode->NextSiblingElement())
    {
        Modelo m;
        const char *ficheiro = pModeloNode->Attribute("ficheiro");
        if(ficheiro) m.ficheiro = ficheiro;
            
        fstream f;
        
        f.open(m.ficheiro,ios::in);
        
        if(f.is_open())
        {
            string line;
            while (getline(f,line))
            {
                float x,y,z;
                sscanf(line.c_str(),"%f %f %f\n",&x,&y,&z);
                Ponto p;
                p.x = x; p.y = y; p.z =z;
                m.pontos.push_back(p);
            }
            
            f.close();
        }
            
        modelos.push_back(m);
    }
}

#endif /* defined(__Motor__main__) */
