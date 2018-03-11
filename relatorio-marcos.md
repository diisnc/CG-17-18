# Geral

* Para o engine foram criados dois ficheiros (em adição ao main.cpp): engine.cpp e xml-loader.cpp (cada um com um ficheiro .h correspondente).
* Cada um destes 2 ficheiros adicionais tem também o seu namespace, de modo a observar boas práticas de programação em C++.
* O xml-loader encarrega-se de ler o ficheiro XML correspondente a uma cena que se quer apresentar, carregando para a memória a informação relativa aos vértices dos modelos envolvidos.
* O engine, por sua vez, trata de receber a informação relativa aos vértices, e a partir deles apresentar os modelos no ecrã.

# Detalhes

## main

O ficheiro principal, `main.cpp`, trata de correr o código de inicialização necessário, assim como de certificar que todas as partes do programa funcionam em conjunto. Isto inclui:

* Inicializar a biblioteca openGL através do GLUT;
* Carregar o documento XML através do `xml-loader`;
* Passar o vetor de modelos carregados ao `engine` de maneira a apresentá-los ao utilizador;

## xml-loader

### Formato do documento XML

O formato do ficheiro XML utilizado para representar uma cena é o seguinte:

```xml
<scene>
    <model file="circle.3d"/>
    <model file="circle.3d"/>
</scene>
```

O formato consiste num elemento pai `<scene>`, que contém um ou mais elementos filho `<model>`, com um atributo `file="ficheiro.3d"` que indica
o nome do ficheiro que contém os vértices correspondentes a um dado modelo.

### Formato do ficheiro .3d

O formato de ficheiro escolhido para os ficheiros `.3d` foi o mais simples possível, contando apenas com as coordenadas X, Y e Z dos vértices, um vértice por linha.

Exemplo:

```text
1.000000 1.000000 0.000000
0.000000 1.000000 0.000000
0.000000 1.000000 1.000000
1.000000 1.000000 0.000000
```

### Leitura do XML

Como sugerido pelo professor, foi usada a biblioteca `tinyxml2` de maneira a facilitar a leitura do documento.

Apenas foi criado um método para este efeito, declarado como `xmlLoader::loadSceneXML(const char* path)`, que recebe como único parâmetro o caminho
até ao ficheiro XML.

Este método faz o seguinte:

1. Começa por tentar carregar o documento no caminho recebido, terminando com um erro se não o tiver encontrado ou se tiver ocorrido um erro.
2. Procura o elemento pai `<scene>`
3. Itera pelos elementos filho `<model>`, guardando o valor do atributo `file` num vetor `std::vector<string> modelPaths`.
4. Tendo obtido todos os caminhos dos ficheiros `.3d`, o xml-loader itera por estes e para cada um:
    1. Carrega o ficheiro `.3d` para uma `ifstream`.
    2. Cria um `engine::model` (classe criada por nós) que contém um vetor onde são guardados os vértices que compõem o modelo.
    3. Até ao end of file, lê linha a linha e por cada uma cria um `engine::vertex` (classe criada por nós) com as coordenadas correspondentes (`modelFile >> vertex.x >> vertex.y >> vertex.z`). Uma vez criado o vértice, este é adicionado ao `engine::model` criado para o efeito.
    4. Tendo terminado de percorrer o ficheiro `.3d`, adiciona o modelo resultante a um vetor `std::vector<engine::model> loadedModels`.
5. No fim, é retornado o vetor `loadedModels`.

## engine


### Classes auxiliares

Foram criadas duas classes auxiliares:

#### engine::vertex

```cpp
struct vertex {
    float x, y, z;
};
```

Guarda a informação relativa a um vértice em 3 `floats`, cada uma correspondente a uma coordenada: X, Y, e Z.

#### engine::model

```cpp
struct model {
    std::vector<vertex> vertices;
};
```

Guarda a informação relativa a um modelo, sendo colocados num vetor os vértices que o compõem.

### Métodos

O `engine` dispõe de dois métodos:

* `void engine::loadScene(vector<model> scene)`, que é usado para guardar a cena a ser representada na memória, para que esta seja acessível localmente;
* `void engine::drawFrame()`, que é chamado a cada frame e se encarrega de renderizar os modelos presentes na `scene`.

#### loadScene

O método `loadScene` limita-se a colocar o vetor `scene` recebido numa variável local, para que o método `drawFrame` lhe consiga aceder.

#### drawFrame

O método `drawFrame` itera pelo vetor `scene` e renderiza os modelos um a um.

Começa por executar apenas `glBegin(GL_TRIANGLES)`, uma vez que as outras configurações já foram executadas pelo ficheiro `main.cpp`.

De seguida, por cada modelo, itera pelo vetor dos seus vértices e executa `glVertex3f(vertex->x, vertex->y, vertex->z)`. Como os vértices são guardados na ordem certa, não é necessário qualquer cuidado adicional neste passo.

Entre a renderização dos triângulos, são realizadas algumas chamadas ao método `glColor3f` de maneira a variar a cor destes, facilitando a sua identificação pelo utilizador.

Por fim, é executado o método `glEnd`.
