#include <stdio.h>
#include <bitset>
#include <iostream>
#include <stdlib.h>
#include <climits>
#include <ctime>

using namespace std;

const int POPULATION = 4;
const int LIMIT = 7;
const char STRINGS[] = {0x06, 0xEE, 0x20, 0x34};

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

void Chromosome::setNext(Chromosome* nxt){
	next=nxt;
}

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

Chromosome::Chromosome(char str, char lab){
	string=str;
	label=lab;
	calcFitness();
	next=NULL;
}

Chromosome::~Chromosome(){
	string=0;
	label=0;
	fitness=0;
	if(next==NULL)
		delete next;
	else
		next->~Chromosome();
}

void Chromosome::display(){
	cout << label << "\t" << (bitset<8>)string << "\t" << fitness << "\n";
}

void Chromosome::mutate(){
	if(rand()%20==1){
		string^=(1<<rand()%8);
	}
}

//General function implementation

void displayAll(Chromosome* head, int gen){
	int i;
	Chromosome* foo=head;
	cout << "Generation: " << gen << "\n" << "Label \t String \t Fitness\n";
	for(i=0;i<POPULATION;i++){
		foo->display();
		foo=foo->getNext();
	}
}

int totalFit(Chromosome* head){
        int i, totalFit = 0;
        Chromosome* foo=head;
        for(i=0;i<POPULATION;i++){
                totalFit += foo->getFitness();
                foo=foo->getNext();
        }
	return totalFit;
}

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
	int i, j, fit, choice, gen=0;
        Chromosome * father[POPULATION];
	//start chomosome creation
	Chromosome* head = new Chromosome(STRINGS[0], 'A');
        Chromosome* foo = head;
        for(i=1;i<POPULATION;i++){
                foo->setNext(new Chromosome(STRINGS[i],'A'+(char)i));
                foo = foo->getNext();
        }
	displayAll(head,gen);
	//end first generation chromosome creation
	// start brieding
	for(j=0;j<LIMIT;j++){
		gen++;
		fit = totalFit(head);
		i=0;
		do{
			choice=rand()%fit;
			father[i]=find(head, choice);
			i++;
		}while(i<POPULATION);
		Chromosome* newgen=bried(father, gen);
		//head->~Chromosome();
		head=newgen;
		newgen = NULL;
		delete newgen;
		displayAll(head, gen);
	}
	return 0;
}

