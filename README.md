# CG-17-18

4-phase solar system project, developed for the course of CG, using Glut and OpenGL (C++).

## Como iniciar

* Seguir as instruções do CMAKE fornecidas pelo professor.
* Colocar o ficheiro `glut32.dll` (da pasta `toolkits/glut/`) na pasta onde são colocados os ficheiros gerados pelo projeto (normalmente chamada `build`).
* Abrir o ficheiro `classTP.sln` que se encontra na mesma pasta que `glut32.dll`.
* Antes de correr, escolher o projeto `classTP` como Startup Project no Visual Studio e correr no modo Release (em vez de Debug).

## Pastas

* `project`: Pasta que contém os ficheiros necessários para qualquer trabalho de CG.
* `tp`: Pasta que contém os ficheiros relevantes a este trabalho prático.

## Módulos

### Generator

Gera ficheiros `.3d` com os vértices necessários à renderização de um sólido.

Comando: `generator [forma] [vertices] [output]`

Exemplo: `generator square (v1 v2 v3 v4 v5 v6 v7 v8) square.3d`

**Nota:** Os vértices são inseridos pela ordem que quisermos, logo vamos inseri-los pela ordem certa e segundo a regra da mão direita.

### Engine

1. Recebe ficheiro XML criado manualmente.
2. Renderiza o conjunto de ficheiros `.3d` presentes no XML.

## Bibliotecas

* [http://www.grinninglizard.com/tinyxml2/index.html](http://www.grinninglizard.com/tinyxml2/index.html)
