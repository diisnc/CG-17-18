# CG-17-18

4-phase solar system project, developed for the course of CG, using Glut and OpenGL (C++).

## TODO

### 1: Gerador de ficheiros

* Deve reter a informação necessária para desenhar um objeto
* Pode ser feito com a estrutura que quisermos

`generator [forma] [vertices] [output]`

Exemplo: `generator square (v1 v2 v3 v4 v5 v6 v7 v8) square.3d`

**Nota:** Os vértices são inseridos pela ordem que quisermos, logo vamos inseri-los pela ordem certa e segundo a regra da mão direita.

Não é preciso meter os ficheiros numa cena XML, basta gerar ficheiros forma.3d com as informações que quisermos.

#### Tipo de ficheiro forma.3d:

```text
FORMA
v1
v2
v3
...
radius
slices
stack
(o que precisarmos)
```

### 2: Engine

* Recebe ficheiro XML (feito à mão por nós) e corre-o.
* Lê cada um dos ficheiros que nele estão inseridos e aplica os vértices nas formas que tem definidas.

Usar: http://www.grinninglizard.com/tinyxml2/index.html
