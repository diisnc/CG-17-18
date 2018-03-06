//
//  main.c
//  Projecto
//
//  Created by Tiago Conceição on 10/03/14.
//  Copyright (c) 2014 Tiago Conceição. All rights reserved.
//

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <math.h>

void drawPlano(float length, float width, char* filename)
{
    FILE* f;
    f = fopen(filename,"w");
    
    if(f!=NULL)
    {
        fprintf(f,"%f %d %f\n",-length/2, 0, -width/2);
        fprintf(f,"%f %d %f\n",-length/2, 0, width/2);
        fprintf(f,"%f %d %f\n",length/2, 0, width/2);
        
        fprintf(f,"%f %d %f\n",length/2, 0, width/2);
        fprintf(f,"%f %d %f\n",length/2, 0, -width/2);
        fprintf(f,"%f %d %f\n",-length/2, 0, -width/2);
    }
    
    fclose(f);
}


void drawParalelipipedo(float length, float height, float width, char* filename)
{
    FILE* f;
    f = fopen(filename,"w");
    
    if(f!=NULL)
    {
        fprintf(f,"%f %f %f\n",length/2, -height/2, -width/2);
        fprintf(f,"%f %f %f\n",-length/2, -height/2, width/2);
        fprintf(f,"%f %f %f\n",-length/2, -height/2, -width/2);
        
        fprintf(f,"%f %f %f\n",-length/2, -height/2, width/2);
        fprintf(f,"%f %f %f\n",length/2, -height/2, -width/2);
        fprintf(f,"%f %f %f\n",length/2, -height/2, width/2);
        
        fprintf(f,"%f %f %f\n",length/2, height/2, -width/2);
        fprintf(f,"%f %f %f\n",-length/2, height/2, -width/2);
        fprintf(f,"%f %f %f\n",-length/2, height/2, width/2);
        
        fprintf(f,"%f %f %f\n",-length/2, height/2, width/2);
        fprintf(f,"%f %f %f\n",length/2, height/2, width/2);
        fprintf(f,"%f %f %f\n",length/2, height/2, -width/2);
        
        fprintf(f,"%f %f %f\n",-length/2, -height/2, width/2);
        fprintf(f,"%f %f %f\n",length/2, -height/2, width/2);
        fprintf(f,"%f %f %f\n",-length/2, height/2, width/2);
        
        fprintf(f,"%f %f %f\n",length/2, -height/2, width/2);
        fprintf(f,"%f %f %f\n",length/2, height/2, width/2);
        fprintf(f,"%f %f %f\n",-length/2, height/2, width/2);
        
        fprintf(f,"%f %f %f\n",length/2, -height/2, width/2);
        fprintf(f,"%f %f %f\n",length/2, -height/2, -width/2);
        fprintf(f,"%f %f %f\n",length/2, height/2, width/2);
        
        fprintf(f,"%f %f %f\n",length/2, -height/2, -width/2);
        fprintf(f,"%f %f %f\n",length/2, height/2, -width/2);
        fprintf(f,"%f %f %f\n",length/2, height/2, width/2);
        
        fprintf(f,"%f %f %f\n",length/2, -height/2, -width/2);
        fprintf(f,"%f %f %f\n",-length/2, -height/2, -width/2);
        fprintf(f,"%f %f %f\n",length/2, height/2, -width/2);
        
        fprintf(f,"%f %f %f\n",-length/2, -height/2, -width/2);
        fprintf(f,"%f %f %f\n",-length/2, height/2, -width/2);
        fprintf(f,"%f %f %f\n",length/2, height/2, -width/2);
        
        fprintf(f,"%f %f %f\n",-length/2, -height/2, -width/2);
        fprintf(f,"%f %f %f\n",-length/2, -height/2, width/2);
        fprintf(f,"%f %f %f\n",-length/2, height/2, -width/2);
        
        fprintf(f,"%f %f %f\n",-length/2, -height/2, width/2);
        fprintf(f,"%f %f %f\n",-length/2, height/2, width/2);
        fprintf(f,"%f %f %f\n",-length/2, height/2, -width/2);
    }
    
    fclose(f);
}

void drawEsfera(float radius, int slices, int stacks, char* filename)
{
    float slice = (360/slices)*(M_PI/180);
    float stack = (180/stacks)*(M_PI/180);
    float piMeios = M_PI/2;
    float doisPi = 2*M_PI;
    
    FILE* f;
    f = fopen(filename,"w");
    
    if(f!=NULL)
    {
        for(float j = -piMeios; j < piMeios; j+=stack)
        {
            for(float i = 0; i <= doisPi; i+=slice)
            {
                fprintf(f,"%f %f %f\n",cos(j+stack)*sin(i)*radius, sin(j+stack)*radius, cos(j+stack)*cos(i)*radius);
                fprintf(f,"%f %f %f\n",cos(j)*sin(i)*radius, sin(j)*radius, cos(j)*cos(i)*radius);
                fprintf(f,"%f %f %f\n",cos(j)*sin(i+slice)*radius, sin(j)*radius, cos(j)*cos(i+slice)*radius);
        
                fprintf(f,"%f %f %f\n",cos(j+stack)*sin(i)*radius, sin(j+stack)*radius, cos(j+stack)*cos(i)*radius);
                fprintf(f,"%f %f %f\n",cos(j)*sin(i+slice)*radius, sin(j)*radius, cos(j)*cos(i+slice)*radius);
                fprintf(f,"%f %f %f\n",cos(j+stack)*sin(i+slice)*radius, sin(j+stack)*radius, cos(j+stack)*cos(i+slice)*radius);
            }
        }
    }
    
    fclose(f);
}

void drawCone(float radius, float height, int slices, int stacks, char* filename)
{
    float slice = (360/slices)*(M_PI/180);
    float stack = height/stacks;
    float raio = radius/stacks;
    float doisPi = 2*M_PI;
    
    FILE* f;
    f = fopen(filename,"w");
    
    if(f!=NULL)
    {
        for (float i = 0; i < doisPi; i += slice)
        {
            fprintf(f,"%d %f %d\n",0, -height/3, 0);
            fprintf(f,"%f %f %f\n",sin(i+slice)*radius, -height/3, cos(i+slice)*radius);
            fprintf(f,"%f %f %f\n",sin(i)*radius, -height/3, cos(i)*radius);
        }
        
        float r = 0;
        for (float j = -height/3; j < (height-height/3); j+=stack)
        {
            for (float i = 0; i < doisPi; i += slice)
            {
                fprintf(f,"%f %f %f\n",sin(i)*(radius-(r+raio)), j+stack, cos(i)*(radius-(r+raio)));
                fprintf(f,"%f %f %f\n",sin(i)*(radius-r), j, cos(i)*(radius-r));
                fprintf(f,"%f %f %f\n",sin(i+slice)*(radius-r), j, cos(i+slice)*(radius-r));
            }
            r += raio;
        }
        
        r = 0;
        for (float j = -height/3; j < (height-height/3)-stack; j+=stack)
        {
            for (float i = 0; i < doisPi; i += slice)
            {
                fprintf(f,"%f %f %f\n",sin(i)*(radius-(r+raio)), j+stack, cos(i)*(radius-(r+raio)));
                fprintf(f,"%f %f %f\n",sin(i+slice)*(radius-r), j, cos(i+slice)*(radius-r));
                fprintf(f,"%f %f %f\n",sin(i+slice)*(radius-(r+raio)), j+stack, cos(i+slice)*(radius-(r+raio)));
            }
            r += raio;
        }
    }
    
    fclose(f);
}

void drawCilindro(float radius, float height, int slices, char* filename)
{
    float slice = (360/slices)*(M_PI/180);
    float doisPi = 2*M_PI;
    
    FILE* f;
    f = fopen(filename,"w");
    
    if(f!=NULL)
    {
        for (float i = 0; i < doisPi; i+=slice)
        {
            fprintf(f,"%d %f %d\n",0, -height/2, 0);
            fprintf(f,"%f %f %f\n",sin(i+slice)*radius, -height/2, cos(i+slice)*radius);
            fprintf(f,"%f %f %f\n",sin(i)*radius, -height/2, cos(i)*radius);
            
            fprintf(f,"%d %f %d\n",0, height/2, 0);
            fprintf(f,"%f %f %f\n",sin(i)*radius, height/2, cos(i)*radius);
            fprintf(f,"%f %f %f\n",sin(i+slice)*radius, height/2, cos(i+slice)*radius);
            
            fprintf(f,"%f %f %f\n",sin(i)*radius, height/2, cos(i)*radius);
            fprintf(f,"%f %f %f\n",sin(i)*radius, -height/2, cos(i)*radius);
            fprintf(f,"%f %f %f\n",sin(i+slice)*radius, -height/2, cos(i+slice)*radius);
            
            fprintf(f,"%f %f %f\n",sin(i+slice)*radius, height/2, cos(i+slice)*radius);
            fprintf(f,"%f %f %f\n",sin(i)*radius, height/2, cos(i)*radius);
            fprintf(f,"%f %f %f\n",sin(i+slice)*radius, -height/2, cos(i+slice)*radius);
        }
    }
    
    fclose(f);
}

int main(int argc, char* argv[])
{
    if(argc <= 1) { printf("Parâmetro inexistente! Opções: plano, paralelipipedo, esfera, cone, piramide, cilindro.\n"); }
    
    else if(!strcmp(argv[1],"plano"))
    {
        if(argc!=5) { printf("Parâmetros errados! Formato: 'plano' <comprimento> <largura> <nome_ficheiro>\n"); }
        else
        {
            float length = atof(argv[2]);
            float width = atof(argv[3]);
            drawPlano(length, width, argv[4]);
        }
    }
    
    else if(!strcmp(argv[1],"paralelipipedo"))
    {
        if(argc!=6) { printf("Parâmetros errados! Formato: 'paralelipipedo' <comprimento> <altura> <largura> <nome_ficheiro>\n"); exit(0); }
        else
        {
            float length = atof(argv[2]);
            float height = atof(argv[3]);
            float width = atof(argv[4]);
            drawParalelipipedo(length, height, width, argv[5]);
        }
    }
    
    else if(!strcmp(argv[1],"esfera"))
    {
        if(argc!=6) { printf("Parâmetros errados! Formato: 'esfera' <raio> <fatias> <camadas> <nome_ficheiro>\n"); exit(0); }
        else
        {
            float radius = atof(argv[2]);
            int slices = atoi(argv[3]);
            int stacks = atoi(argv[4]);
            drawEsfera(radius, slices, stacks, argv[5]);
        }
    }
    
    else if(!strcmp(argv[1],"cone"))
    {
        if(argc!=7) { printf("Parâmetros errados! Formato: 'cone' <raio> <altura> <fatias> <camadas> <nome_ficheiro>\n"); exit(0); }
        else
        {
            float radius = atof(argv[2]);
            float height = atof(argv[3]);
            int slices = atoi(argv[4]);
            int stacks = atoi(argv[5]);
            drawCone(radius, height, slices, stacks, argv[6]);
        }
    }
    
    else if(!strcmp(argv[1],"piramide"))
    {
        if(argc!=6) { printf("Parâmetros errados! Formato: 'piramide' <raio> <altura> <lados> <nome_ficheiro>\n"); exit(0); }
        else
        {
            float radius = atof(argv[2]);
            float height = atof(argv[3]);
            int slices = atoi(argv[4]);
            drawCone(radius, height, slices, 1, argv[5]);
        }
    }
    
    else if(!strcmp(argv[1],"cilindro"))
    {
        if(argc!=6) { printf("Parâmetros errados! Formato: 'cilindro' <raio> <altura> <fatias> <nome_ficheiro>\n"); exit(0); }
        else
        {
            float radius = atof(argv[2]);
            float height = atof(argv[3]);
            int slices = atoi(argv[4]);
            drawCilindro(radius, height, slices, argv[5]);
        }
    }
    
    return 0;
}


