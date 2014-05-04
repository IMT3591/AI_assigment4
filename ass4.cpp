#include <stdio.h>
#include <bitset>
#include <iostream>

using namespace std;

const int POPULATION = 4;
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

		Chromosome* getNext(){	return next;	};
		void setNext(Chromosome*);
		void calcFitness();
		void display();
		void copy(Chromosome);
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
}

void Chromosome::display(){
	cout << label << "\t" << (bitset<8>)string << "\t" << fitness << "\n";
}


//General function implementation

void displayAll(Chromosome* head){
	int i;
	Chromosome* foo=head;
	cout << "Label \t String \t Fitness\n";
	for(i=0;i<POPULATION;i++){
		foo->display();
		foo=foo->getNext();
	}
}

int main(){
	int i;
	Chromosome* head = new Chromosome(STRINGS[0], 'A');
	Chromosome* foo = head;
	for(i=1;i<POPULATION;i++){
		foo->setNext(new Chromosome(STRINGS[i],'A'+(char)i));
		foo = foo->getNext();
	}
	displayAll(head);
	return 0;
}

