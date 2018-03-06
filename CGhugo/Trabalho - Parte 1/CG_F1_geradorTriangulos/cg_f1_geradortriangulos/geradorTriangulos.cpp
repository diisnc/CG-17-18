#include <string>
#define _USE_MATH_DEFINES
#include <math.h>
#include<vector>

using namespace std;

// Estruturas
typedef struct ponto { float x, y, z; } Ponto;
typedef struct triangulo { Ponto p1, p2, p3; } Triangulo;

void gerarPlano(float l, string nomeFicheiro) {

	// gera vertex
	Ponto p1, p2, p3, p4;;
	p1.x = l / 2; p1.y = 0; p1.z = l / 2;
	p2.x = l / 2; p2.y = 0; p2.z = -(l / 2);
	p3.x = -(l / 2); p3.y = 0; p3.z = -(l / 2);
	p4.x = -(l / 2); p4.y = 0; p4.z = l / 2;

	// cria ficheiro
	FILE *f;
	fopen_s(&f, nomeFicheiro.c_str(), "w");

	// Escrever os 2 triangulos
	// Triangulo 1
	fputs("2\n",f);
	fputs(to_string(p1.x).c_str(), f); fputc(' ', f); fputs(to_string(p1.y).c_str(), f); fputc(' ', f); fputs(to_string(p1.z).c_str(), f); fputc('\n', f);
	fputs(to_string(p2.x).c_str(), f); fputc(' ', f); fputs(to_string(p2.y).c_str(), f); fputc(' ', f); fputs(to_string(p2.z).c_str(), f); fputc('\n', f);
	fputs(to_string(p3.x).c_str(), f); fputc(' ', f); fputs(to_string(p3.y).c_str(), f); fputc(' ', f); fputs(to_string(p3.z).c_str(), f); fputc('\n', f);

	// Triangulo 2
	fputs(to_string(p1.x).c_str(), f); fputc(' ', f); fputs(to_string(p1.y).c_str(), f); fputc(' ', f); fputs(to_string(p1.z).c_str(), f); fputc('\n', f);
	fputs(to_string(p3.x).c_str(), f); fputc(' ', f); fputs(to_string(p3.y).c_str(), f); fputc(' ', f); fputs(to_string(p3.z).c_str(), f); fputc('\n', f);
	fputs(to_string(p4.x).c_str(), f); fputc(' ', f); fputs(to_string(p4.y).c_str(), f); fputc(' ', f); fputs(to_string(p4.z).c_str(), f); fputc('\n', f);

	fclose(f); //fechar ligacao
}

void gerarCone(float raioBase, float altura, int slices, int stacks, string nomeFicheiro) {

	int iteracoesCicloFora, iteracoesCicloDentro;
	double alfa, x, xAnt, z, zAnt, novoRaio = raioBase;
	vector <Triangulo> triangulos;
	Ponto pontoOrigem; pontoOrigem.x = 0; pontoOrigem.y = 0; pontoOrigem.z = 0;
	Ponto pontoPico; pontoPico.x = 0; pontoPico.y = altura; pontoPico.z = 0;

	for (iteracoesCicloFora = 0; iteracoesCicloFora < stacks;) {
		for (iteracoesCicloDentro = 0; iteracoesCicloDentro <= slices; iteracoesCicloDentro++) {
			alfa = iteracoesCicloDentro*(2 * M_PI) / slices;	//calcula alfa
			x = novoRaio*sin(alfa);
			z = novoRaio*cos(alfa);

			if (iteracoesCicloDentro > 0) {
				Triangulo novoTrianguloLateral;
				Ponto pontoAgora, pontoAnterior;

				pontoAgora.x = x; pontoAgora.y = altura / stacks*iteracoesCicloFora; pontoAgora.z = z;
				pontoAnterior.x = xAnt; pontoAnterior.y = altura / stacks*iteracoesCicloFora; pontoAnterior.z = zAnt;
				novoTrianguloLateral.p1 = pontoAnterior; novoTrianguloLateral.p2 = pontoAgora; novoTrianguloLateral.p3 = pontoPico;

				triangulos.push_back(novoTrianguloLateral);

				// Calcular os triangulos da base
				if (iteracoesCicloFora == 0) {
					Triangulo novoTrianguloBase;
					novoTrianguloBase.p1 = pontoAnterior; novoTrianguloBase.p2 = pontoOrigem; novoTrianguloBase.p3 = pontoAgora;
					triangulos.push_back(novoTrianguloBase);
				}
			}
			xAnt = x;
			zAnt = z;
		}
		// Calcular novo raio
		iteracoesCicloFora++;
		novoRaio = raioBase*(double)((double)1 - (double)iteracoesCicloFora / stacks);
	}
	FILE *f;
	fopen_s(&f, nomeFicheiro.c_str(), "w");
	vector<Triangulo>::iterator meuIterador;	// usar um iterador sobre o vector

	//1ª linha com o nr de triangulos
	fputs(to_string(triangulos.size()).c_str(), f);
	fputs("\n", f);

	for (meuIterador = triangulos.begin(); meuIterador != triangulos.end(); meuIterador++) {
		//1º ponto
		fputs(to_string(meuIterador->p1.x).c_str(), f); fputc(' ', f);
		fputs(to_string(meuIterador->p1.y).c_str(), f); fputc(' ', f);
		fputs(to_string(meuIterador->p1.z).c_str(), f); fputc('\n', f);
		// 2º
		fputs(to_string(meuIterador->p2.x).c_str(), f); fputc(' ', f);
		fputs(to_string(meuIterador->p2.y).c_str(), f); fputc(' ', f);
		fputs(to_string(meuIterador->p2.z).c_str(), f); fputc('\n', f);
		// 3º
		fputs(to_string(meuIterador->p3.x).c_str(), f); fputc(' ', f);
		fputs(to_string(meuIterador->p3.y).c_str(), f); fputc(' ', f);
		fputs(to_string(meuIterador->p3.z).c_str(), f); fputc('\n', f);
	}
	fclose(f);
}

void gerarCaixa(float x, float y, float z, string nomeFicheiro) {

	
	// gera vertex
	Ponto p1, p2, p3, p4, p5, p6, p7, p8;
	p1.x = x / 2; p1.y = -y / 2; p1.z = z / 2;
	p2.x = -x / 2; p2.y = -y / 2; p2.z = z / 2;
	p3.x = -x / 2; p3.y = -y / 2; p3.z = -z / 2;
	p4.x = x / 2; p4.y = -y / 2; p4.z = -z / 2;
	p5.x = x / 2; p5.y = y / 2; p5.z = z / 2;
	p6.x = -x / 2; p6.y = y / 2; p6.z = z / 2;
	p7.x = -x / 2; p7.y = y / 2; p7.z = -z / 2;
	p8.x = x / 2; p8.y = y / 2; p8.z = -z / 2;

	// cria ficheiro
	FILE *f;
	fopen_s(&f, nomeFicheiro.c_str(), "w");

	// escreve os vertices da caixa
	// Primeira linha-nº triangulos
	fputs(to_string(12).c_str(), f);
	fputc('\n', f);
	// Triangulos
	fputs(to_string(p1.x).c_str(), f); fputc(' ', f); fputs(to_string(p1.y).c_str(), f); fputc(' ', f); fputs(to_string(p1.z).c_str(), f); fputc('\n', f);
	fputs(to_string(p6.x).c_str(), f); fputc(' ', f); fputs(to_string(p6.y).c_str(), f); fputc(' ', f); fputs(to_string(p6.z).c_str(), f); fputc('\n', f);
	fputs(to_string(p2.x).c_str(), f); fputc(' ', f); fputs(to_string(p2.y).c_str(), f); fputc(' ', f); fputs(to_string(p2.z).c_str(), f); fputc('\n', f);

	fputs(to_string(p1.x).c_str(), f); fputc(' ', f); fputs(to_string(p1.y).c_str(), f); fputc(' ', f); fputs(to_string(p1.z).c_str(), f); fputc('\n', f);
	fputs(to_string(p5.x).c_str(), f); fputc(' ', f); fputs(to_string(p5.y).c_str(), f); fputc(' ', f); fputs(to_string(p5.z).c_str(), f); fputc('\n', f);
	fputs(to_string(p6.x).c_str(), f); fputc(' ', f); fputs(to_string(p6.y).c_str(), f); fputc(' ', f); fputs(to_string(p6.z).c_str(), f); fputc('\n', f);

	fputs(to_string(p4.x).c_str(), f); fputc(' ', f); fputs(to_string(p4.y).c_str(), f); fputc(' ', f); fputs(to_string(p4.z).c_str(), f); fputc('\n', f);
	fputs(to_string(p5.x).c_str(), f); fputc(' ', f); fputs(to_string(p5.y).c_str(), f); fputc(' ', f); fputs(to_string(p5.z).c_str(), f); fputc('\n', f);
	fputs(to_string(p1.x).c_str(), f); fputc(' ', f); fputs(to_string(p1.y).c_str(), f); fputc(' ', f); fputs(to_string(p1.z).c_str(), f); fputc('\n', f);

	fputs(to_string(p4.x).c_str(), f); fputc(' ', f); fputs(to_string(p4.y).c_str(), f); fputc(' ', f); fputs(to_string(p4.z).c_str(), f); fputc('\n', f);
	fputs(to_string(p8.x).c_str(), f); fputc(' ', f); fputs(to_string(p8.y).c_str(), f); fputc(' ', f); fputs(to_string(p8.z).c_str(), f); fputc('\n', f);
	fputs(to_string(p5.x).c_str(), f); fputc(' ', f); fputs(to_string(p5.y).c_str(), f); fputc(' ', f); fputs(to_string(p5.z).c_str(), f); fputc('\n', f);

	fputs(to_string(p7.x).c_str(), f); fputc(' ', f); fputs(to_string(p7.y).c_str(), f); fputc(' ', f); fputs(to_string(p7.z).c_str(), f); fputc('\n', f);
	fputs(to_string(p4.x).c_str(), f); fputc(' ', f); fputs(to_string(p4.y).c_str(), f); fputc(' ', f); fputs(to_string(p4.z).c_str(), f); fputc('\n', f);
	fputs(to_string(p3.x).c_str(), f); fputc(' ', f); fputs(to_string(p3.y).c_str(), f); fputc(' ', f); fputs(to_string(p3.z).c_str(), f); fputc('\n', f);

	fputs(to_string(p7.x).c_str(), f); fputc(' ', f); fputs(to_string(p7.y).c_str(), f); fputc(' ', f); fputs(to_string(p7.z).c_str(), f); fputc('\n', f);
	fputs(to_string(p8.x).c_str(), f); fputc(' ', f); fputs(to_string(p8.y).c_str(), f); fputc(' ', f); fputs(to_string(p8.z).c_str(), f); fputc('\n', f);
	fputs(to_string(p4.x).c_str(), f); fputc(' ', f); fputs(to_string(p4.y).c_str(), f); fputc(' ', f); fputs(to_string(p4.z).c_str(), f); fputc('\n', f);

	fputs(to_string(p2.x).c_str(), f); fputc(' ', f); fputs(to_string(p2.y).c_str(), f); fputc(' ', f); fputs(to_string(p2.z).c_str(), f); fputc('\n', f);
	fputs(to_string(p6.x).c_str(), f); fputc(' ', f); fputs(to_string(p6.y).c_str(), f); fputc(' ', f); fputs(to_string(p6.z).c_str(), f); fputc('\n', f);
	fputs(to_string(p3.x).c_str(), f); fputc(' ', f); fputs(to_string(p3.y).c_str(), f); fputc(' ', f); fputs(to_string(p3.z).c_str(), f); fputc('\n', f);

	fputs(to_string(p6.x).c_str(), f); fputc(' ', f); fputs(to_string(p6.y).c_str(), f); fputc(' ', f); fputs(to_string(p6.z).c_str(), f); fputc('\n', f);
	fputs(to_string(p7.x).c_str(), f); fputc(' ', f); fputs(to_string(p7.y).c_str(), f); fputc(' ', f); fputs(to_string(p7.z).c_str(), f); fputc('\n', f);
	fputs(to_string(p3.x).c_str(), f); fputc(' ', f); fputs(to_string(p3.y).c_str(), f); fputc(' ', f); fputs(to_string(p3.z).c_str(), f); fputc('\n', f);

	fputs(to_string(p6.x).c_str(), f); fputc(' ', f); fputs(to_string(p6.y).c_str(), f); fputc(' ', f); fputs(to_string(p6.z).c_str(), f); fputc('\n', f);
	fputs(to_string(p5.x).c_str(), f); fputc(' ', f); fputs(to_string(p5.y).c_str(), f); fputc(' ', f); fputs(to_string(p5.z).c_str(), f); fputc('\n', f);
	fputs(to_string(p7.x).c_str(), f); fputc(' ', f); fputs(to_string(p7.y).c_str(), f); fputc(' ', f); fputs(to_string(p7.z).c_str(), f); fputc('\n', f);

	fputs(to_string(p5.x).c_str(), f); fputc(' ', f); fputs(to_string(p5.y).c_str(), f); fputc(' ', f); fputs(to_string(p5.z).c_str(), f); fputc('\n', f);
	fputs(to_string(p8.x).c_str(), f); fputc(' ', f); fputs(to_string(p8.y).c_str(), f); fputc(' ', f); fputs(to_string(p8.z).c_str(), f); fputc('\n', f);
	fputs(to_string(p7.x).c_str(), f); fputc(' ', f); fputs(to_string(p7.y).c_str(), f); fputc(' ', f); fputs(to_string(p7.z).c_str(), f); fputc('\n', f);

	fputs(to_string(p2.x).c_str(), f); fputc(' ', f); fputs(to_string(p2.y).c_str(), f); fputc(' ', f); fputs(to_string(p2.z).c_str(), f); fputc('\n', f);
	fputs(to_string(p3.x).c_str(), f); fputc(' ', f); fputs(to_string(p3.y).c_str(), f); fputc(' ', f); fputs(to_string(p3.z).c_str(), f); fputc('\n', f);
	fputs(to_string(p1.x).c_str(), f); fputc(' ', f); fputs(to_string(p1.y).c_str(), f); fputc(' ', f); fputs(to_string(p1.z).c_str(), f); fputc('\n', f);

	fputs(to_string(p1.x).c_str(), f); fputc(' ', f); fputs(to_string(p1.y).c_str(), f); fputc(' ', f); fputs(to_string(p1.z).c_str(), f); fputc('\n', f);
	fputs(to_string(p3.x).c_str(), f); fputc(' ', f); fputs(to_string(p3.y).c_str(), f); fputc(' ', f); fputs(to_string(p3.z).c_str(), f); fputc('\n', f);
	fputs(to_string(p4.x).c_str(), f); fputc(' ', f); fputs(to_string(p4.y).c_str(), f); fputc(' ', f); fputs(to_string(p4.z).c_str(), f); fputc('\n', f);

	fclose(f); //fechar ligacao
}

void gerarEsfera(float raio, int slices, int stacks, string nomeFicheiro) {
	int i = 0;
	float alpha = 0;
	float beta = -(M_PI) / 2;
	float angulo_xz = (2 * M_PI) / (slices);
	float angulo_y = (M_PI) / stacks;
	float beta1 = (-(M_PI) / 2) + angulo_y;
	int x = slices*stacks + 2;
	vector <Ponto> pontos;
	Ponto p1;
	p1.x = 0;
	p1.y = -raio;
	p1.z = 0;
	pontos.push_back(p1);
	i++;
	for (int it = 1; it < stacks; it++)
	{
		beta = beta + angulo_y;
		for (int sl = 0; sl < slices; sl++)
		{
			alpha = ((2 * M_PI) / (slices))*sl;
			float x = raio* cos(beta)*sin(alpha);
			float z = raio* cos(beta)*cos(alpha);
			float y = raio*sin(beta);
			Ponto p2;
			p2.x = x;
			p2.y = y;
			p2.z = z;
			pontos.push_back(p2);
			i++;
		}
	}
	p1.x = 0;
	p1.y = raio;
	p1.z = 0;
	pontos.push_back(p1);
	i++;
	int triangulos = 0;

	vector <Triangulo> triangulos_vector;

	//parte de baixo
	int contador = 0;
	int j = 1;
	Triangulo triang;
	for (j = 1; j < i && pontos[j].y == pontos[j + 1].y; j++)
	{
		triang.p1.x = pontos[j].x;
		triang.p1.y = pontos[j].y;
		triang.p1.z = pontos[j].z;
		triang.p2.x = pontos[0].x;
		triang.p2.y = pontos[0].y;
		triang.p2.z = pontos[0].z;
		triang.p3.x = pontos[j + 1].x;
		triang.p3.y = pontos[j + 1].y;
		triang.p3.z = pontos[j + 1].z;
		triangulos_vector.push_back(triang);
		triangulos++;
	}
	// ultimo tringulo da parte de baixo
	triang.p1.x = pontos[j].x;
	triang.p1.y = pontos[j].y;
	triang.p1.z = pontos[j].z;
	triang.p2.x = pontos[0].x;
	triang.p2.y = pontos[0].y;
	triang.p2.z = pontos[0].z;
	triang.p3.x = pontos[1].x;
	triang.p3.y = pontos[1].y;
	triang.p3.z = pontos[1].z;
	triangulos_vector.push_back(triang);
	triangulos++;

	//parte do meio
	float ultimoy = pontos[j].y;
	contador = j;
	j++;
	int fixo = contador;
	contador = j;
	float y1 = 0;
	float yslice = 0;
	bool vez = false;
	for (int h = 1; h < stacks - 1; h++)
	{
		vez = false;

		for (j = contador; j < i && pontos[j].y == pontos[j + 1].y; j++)
		{
			vez = true;
			// 1º triangulo
			triang.p1.x = pontos[j].x;
			triang.p1.y = pontos[j].y;
			triang.p1.z = pontos[j].z;
			triang.p2.x = pontos[j - fixo].x;
			triang.p2.y = pontos[j - fixo].y;
			triang.p2.z = pontos[j - fixo].z;
			triang.p3.x = pontos[j + 1].x;
			triang.p3.y = pontos[j + 1].y;
			triang.p3.z = pontos[j + 1].z;
			triangulos_vector.push_back(triang);
			triangulos++;

			// 2º triangulo
			triang.p1.x = pontos[j + 1].x;
			triang.p1.y = pontos[j + 1].y;
			triang.p1.z = pontos[j + 1].z;
			triang.p2.x = pontos[j - fixo].x;
			triang.p2.y = pontos[j - fixo].y;
			triang.p2.z = pontos[j - fixo].z;
			triang.p3.x = pontos[j + 1 - fixo].x;
			triang.p3.y = pontos[j + 1 - fixo].y;
			triang.p3.z = pontos[j + 1 - fixo].z;
			triangulos_vector.push_back(triang);

			contador++;
			triangulos++;
		}

		if (vez) {

			// ultimos tringulos para cada stack;
			triang.p1.x = pontos[j].x;
			triang.p1.y = pontos[j].y;
			triang.p1.z = pontos[j].z;
			triang.p2.x = pontos[j - fixo].x;
			triang.p2.y = pontos[j - fixo].y;
			triang.p2.z = pontos[j - fixo].z;
			triang.p3.x = pontos[j + 1 - fixo].x;
			triang.p3.y = pontos[j + 1 - fixo].y;
			triang.p3.z = pontos[j + 1 - fixo].z;
			triangulos_vector.push_back(triang);

			triangulos++;

			// 2º triangulo
			triang.p1.x = pontos[j - fixo].x;
			triang.p1.y = pontos[j - fixo].y;
			triang.p1.z = pontos[j - fixo].z;
			triang.p2.x = pontos[j + 1 - (2 * fixo)].x;
			triang.p2.y = pontos[j + 1 - (2 * fixo)].y;
			triang.p2.z = pontos[j + 1 - (2 * fixo)].z;
			triang.p3.x = pontos[j + 1 - fixo].x;
			triang.p3.y = pontos[j + 1 - fixo].y;
			triang.p3.z = pontos[j + 1 - fixo].z;
			triangulos_vector.push_back(triang);

			contador++;
			triangulos++;
		}
		yslice = pontos[j].y - ultimoy;
	}

	//parte de cima da esfera
	int k = j - fixo + 1;
	for (k = j - fixo + 1; k < i - 2; k++)
	{
		triang.p1.x = pontos[k].x;
		triang.p1.y = pontos[k].y;
		triang.p1.z = pontos[k].z;
		triang.p2.x = pontos[k + 1].x;
		triang.p2.y = pontos[k + 1].y;
		triang.p2.z = pontos[k + 1].z;
		triang.p3.x = pontos[i - 1].x;
		triang.p3.y = pontos[i - 1].y;
		triang.p3.z = pontos[i - 1].z;
		triangulos_vector.push_back(triang);
		triangulos++;
	}
	// ultimo triangulo parte de cima
	triang.p1.x = pontos[k].x;
	triang.p1.y = pontos[k].y;
	triang.p1.z = pontos[k].z;
	triang.p2.x = pontos[k + 1 - fixo].x;
	triang.p2.y = pontos[k + 1 - fixo].y;
	triang.p2.z = pontos[k + 1 - fixo].z;
	triang.p3.x = pontos[i - 1].x;
	triang.p3.y = pontos[i - 1].y;
	triang.p3.z = pontos[i - 1].z;
	triangulos_vector.push_back(triang);
	triangulos++;

	// cria ficheiro
	FILE *f;
	fopen_s(&f, nomeFicheiro.c_str(), "w");

	// Primeira linha-nº triangulos
	fputs(to_string(triangulos).c_str(), f);
	fputc('\n', f);

	vector <Triangulo> ::iterator meuIterador;

	for (meuIterador = triangulos_vector.begin(); meuIterador != triangulos_vector.end(); meuIterador++) {
		//1º ponto
		fputs(to_string(meuIterador->p1.x).c_str(), f); fputc(' ', f);
		fputs(to_string(meuIterador->p1.y).c_str(), f); fputc(' ', f);
		fputs(to_string(meuIterador->p1.z).c_str(), f); fputc('\n', f);
		// 2º
		fputs(to_string(meuIterador->p2.x).c_str(), f); fputc(' ', f);
		fputs(to_string(meuIterador->p2.y).c_str(), f); fputc(' ', f);
		fputs(to_string(meuIterador->p2.z).c_str(), f); fputc('\n', f);
		// 3º
		fputs(to_string(meuIterador->p3.x).c_str(), f); fputc(' ', f);
		fputs(to_string(meuIterador->p3.y).c_str(), f); fputc(' ', f);
		fputs(to_string(meuIterador->p3.z).c_str(), f); fputc('\n', f);
	}

	fclose(f); //fechar ligacao
}

int main(int argc, char **argv) {
	string primitiveType;
	if (argc >= 2) primitiveType = argv[1];
	else return 1;

	if (primitiveType == "plano") {
		if (argc == 4) gerarPlano(stof(argv[2]), argv[3]);
		else return 1; 
	}
	else if (primitiveType == "caixa") {
		if (argc == 6) gerarCaixa(stof(argv[2]), stof(argv[3]), stof(argv[4]), argv[5]);
		else return 1;
	}
	else if (primitiveType == "esfera") {
		if (argc == 6) gerarEsfera(stof(argv[2]), stoi(argv[3]), stoi(argv[4]), argv[5]);
		else return 1;
	}
	else if (primitiveType == "cone") {
		if (argc == 7) gerarCone(stof(argv[2]), stof(argv[3]), stof(argv[4]), stof(argv[5]), argv[6]);
		else return 1;
	}
	else return 1; // problema
	return 0;
}