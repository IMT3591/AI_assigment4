#include <stdio.h>
#include <bitset>
#include <iostream>
#include <stdlib.h>
#include <climits>
#include <ctime>

using namespace std;

const unsigned int MUTATION_PROBABILITY = 50; 	//Value will be used as follows: rand()%(1000/MUTATION_PROBABILITY)
const int POPULATION = 4;			//Number of chromosomes per generation
const int LIMIT = 9;				//Number of generations
const char STRINGS[] = {0x06, 0xEE, 0x20, 0x34};//First generation
const string GRAPH[] = {" ","_","_-_","_-_-_","_-_-_-_","_-_-_-_-_","_-_-_-_-_-_","_-_-_-_-_-_-_","_-_-_-_-_-_-_-_"};

class Chromosome{
	private:
		char string;
		char label;
		int fitness;
		Chromosome* next;
	public:
		Chromosome();
		Chromosome(char, char);
		~Chromosome();

		Chromosome* getNext(){	return next;	};
		int getFitness(){	return fitness;	};
		char getString(){	return string;	};

		void setNext(Chromosome*);

		void calcFitness();
		void display();
		void mutate();
};

//Chromosome implementation

	/**
	*	\author	20140504 - Adrian Alberdi Ainciburu	
	*	\brief	Constructor of the Chromosome class
	**/

Chromosome::Chromosome(char str, char lab){
	string=str;
	label=lab;
	calcFitness();
	next=NULL;
}

	/**
	*	\author	20140504 - Adrian Alberdi Ainciburu	
	*	\brief	Destructor of the Chromosome class
	**/

Chromosome::~Chromosome(){
	string=0;
	label=0;
	fitness=0;
	if(next==NULL)
		delete next;
	else
		next->~Chromosome();
}

	/**
	*	\author	20140504 - Adrian Alberdi Ainciburu	
	*	\brief	Sets the next Chromosome in the list
	**/

void Chromosome::setNext(Chromosome* nxt){
	next=nxt;
}

	/**
	*	\author	20140504 - Adrian Alberdi Ainciburu	
	*	\brief	Calculates the fitness of the chromosome by counting the number 
	*	of 1's contained in the field string
	**/

void Chromosome::calcFitness(){
	unsigned char sum=string;
	fitness=0;
	while(sum>0)
	{
		if(sum%2 != 0)
		{
			fitness++;   // counting number of ones
		}
		sum=sum/2;
	}
}

	/**
	*	\author	20140504 - Adrian Alberdi Ainciburu	
	*	\brief	Prints the information about the chromosome
	**/

void Chromosome::display(){
	cout << label << "\t" << (bitset<8>)string << "\t" << fitness << endl;
}

	/**
	*	\author	20140504 - Adrian Alberdi Ainciburu	
	*	\brief	Produces a change in the genetic material of the chromosome with a 
	*	probability p=1000/MUTATION_PROBABILITY
	**/

void Chromosome::mutate(){
	if(rand()%(1000/MUTATION_PROBABILITY)==1){
		string^=(1<<rand()%8);
	}
}

//General function implementation

	/**
	*	\author	20140504 - Adrian Alberdi Ainciburu	
	*	\brief	Prints the information about all the chromosomes in the generation
	**/

void displayAll(Chromosome* head, int gen, int avgfit){
	int i;
	Chromosome* foo=head;
	cout << "Generation: " << gen << " average fit: " << avgfit << "\n" << "Label \t String \t Fitness\n";
	for(i=0;i<POPULATION;i++){
		foo->display();
		foo=foo->getNext();
	}
}

	/**
	*	\author	20140504 - Adrian Alberdi Ainciburu	
	*	\brief	Prints the final statistics about the program and the evolution of the chromosomes
	**/

void printAverage(int* avgfit){
	int i;
	for(i=0;i<LIMIT;i++){
		cout << "Gen " << i << " Average fitness: " << avgfit[i] << "\t| " << GRAPH[avgfit[i]] << endl;
	}
}

Chromosome* arrange(Chromosome* head){
	int i, j;
	for (i=0;i<POPULATION;i++){
		Chromosome* foo = head;
		Chromosome* fooler = foo->getNext();
		for(j=0;j<POPULATION;j++){
			if(foo->getFitness() < fooler->getFitness()){
				if(j=0)	head=fooler;
				foo->setNext(fooler->getNext());
				fooler->setNext(foo);
				foo=fooler;
			}
			if(foo->getNext())
				foo=foo->getNext();
			if(foo->getNext()!=NULL)
				fooler=foo->getNext();
		}
	}
}

	/**
	*	\author	20140504 - Adrian Alberdi Ainciburu	
	*	\brief	Calculates the total fitness of the generation. The maximun value is: POPULATION*8
	**/

int totalFit(Chromosome* head){
        int i, totalFit = 0;
        Chromosome* foo=head;
        for(i=0;i<POPULATION;i++){
                totalFit += foo->getFitness();
                foo=foo->getNext();
        }
	return totalFit;
}

	/**
	*	\author	20140504 - Adrian Alberdi Ainciburu	
	*	\brief	Finds a chromosome by the method of spinning wheel fitness.
	**/

Chromosome* find(Chromosome* head, int choice){
	Chromosome* foo=head;
	int tot=foo->getFitness();
	while(choice>tot){
		foo=foo->getNext();
		tot+=foo->getFitness();
	}
	return foo;
}

Chromosome* bried(Chromosome* father[], int gen){
	int crossover, notcrossover;
	crossover=rand()%256;
	notcrossover=crossover^0xFF;
	Chromosome* newgen = new Chromosome((father[0]->getString()&crossover)|(father[1]->getString()&notcrossover), 'A'+gen*POPULATION);
	Chromosome* foo = newgen;
	foo->mutate();
	foo->setNext(new Chromosome((father[1]->getString()&crossover)|(father[0]->getString()&notcrossover), 'A'+gen*POPULATION+1));
	foo=foo->getNext();
	foo->mutate();
        crossover=rand()%256;
        notcrossover=crossover^0xFF;
	foo->setNext(new Chromosome((father[2]->getString()&crossover)|(father[3]->getString()&notcrossover),'A'+gen*POPULATION+2));
	foo=foo->getNext();
	foo->mutate();
        foo->setNext(new Chromosome((father[3]->getString()&crossover)|(father[2]->getString()&notcrossover),'A'+gen*POPULATION+3));
	foo=foo->getNext();
	foo->mutate();
	return newgen;
}

int main(){
	srand(time(NULL));
	int i, j, fit, choice, avgfit[LIMIT], gen=0;
        Chromosome * father[POPULATION];
	//start chomosome creation
	Chromosome* head = new Chromosome(STRINGS[0], 'A');
        Chromosome* foo = head;
        for(i=1;i<POPULATION;i++){
                foo->setNext(new Chromosome(STRINGS[i],'A'+(char)i));
                foo = foo->getNext();
        }
	displayAll(head,gen, avgfit[0]=3);
	//end first generation chromosome creation
	// start brieding
	for(j=0;j<LIMIT;j++){
		gen++;
		fit = totalFit(head);
		avgfit[j+1] = fit/POPULATION;
		i=0;
		do{
			choice=rand()%fit;
			father[i]=find(head, choice);
			i++;
		}while(i<POPULATION);
		arrange(head);
		Chromosome* newgen=bried(father, gen);
		//head->~Chromosome();
		head=newgen;
		newgen = NULL;
		delete newgen;
		displayAll(head, gen, avgfit[j+1]);
	}
	//end of brieding
	//Average fitness variation
	printAverage(avgfit);
	return 0;
}

