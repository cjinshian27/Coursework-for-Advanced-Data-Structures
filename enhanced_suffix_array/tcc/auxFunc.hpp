/*
	Marco Alves de Alcantara
	NUSP 11221904
*/

#pragma once

#include <iostream>
#include <string>

#define CHAREXTRA '\0' //Caractere especial que precede todos os chars do alfabeto
#define ARRAYMAXPRINT 20 //N�mero m�ximo de valores de um array que devem ser impressos
#define ALPHABETSIZE 256 //Tamanho do alfabeto

/*Faz uma c�pia de text, mas acrescentando o caractere '\0'
ao final, caso a string original n�o terminava com '\0'*/
std::string appendNull(std::string* text);

/*Compara os sufixos das strings strA e strB iniciados nas posi��es iniA e iniB.
Retorna 0 se os sufixos s�o iguais, 1 se (*strA)[iniA..] > (*strB)[iniB..], e -1 caso contr�rio.
Al�m disso, se comp != nullptr, a fun��o armazena quantos caracteres foram iguais nas compara��es em *comp*/
int comparaSufixos(std::string* strA, int iniA, std::string* strB, int iniB, int* comp);

/*Faz o mergeSort dos sufixos de (*s)[ini;fim], armazenando os �ndices no vetor de sufixos vet[]*/
void mergeSortSufixos(std::string* s, int* vet, int ini, int fim);

/*Recebe um vetor de sufixos vet[] ordenado em vet[ini;meio] e vet[meio+1;fim],
e ordena vet[ini;fim] baseando-se nos sufixos da string *s*/
void mergeSufixos(std::string* s, int* vet, int ini, int meio, int fim);

/*Recebe uma string str e dois �ndices, iniA e iniB na string, e
retorna o LCP dos sufixos de str iniciados em iniA e iniB.*/
int contaLCP(std::string* str, int iniA, int iniB);

/*Retorna uma string para representar um sufixo de s a partir de ini.
Usada apenas para imprimir resultados de uma busca.*/
std::string sufString(std::string s, int ini);

/*Imprime at� ARRAYMAXPRINT valores de um array*/
void printArray(int* arr, int size, std::string printName, int startIndex);