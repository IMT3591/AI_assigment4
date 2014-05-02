//Written by Adrian Alberdi

//includes
#include <stdio.h>              //Standard library for some things
#include <iostream>             //Standard input output cin/cout
#include <fstream>              //Used for file streams
#include <cstdlib>              //Used for rand()
#include <cmath>                //Used for math. abs(), exp()
#include <iomanip>

using namespace std;

//Global values
const int NUM_OF_LETTER		= 26;
const int LEARNING_SET		= 10;
const int NUM_OF_PIXELS		= 100;
const int NUM_OF_OUTPUTS	= 26;
const int NUM_OF_LAYERS		= 2;
const int NUM_IN_HIDDEN		= 120;
const int ANN_PER_GEN		= 4;

class Link;

struct Floaty{
	int base;
	int exponent;
	Floaty(){}
	Floaty( float value ){
		base = frexp(value, &exponent);
	}
	float toFloat(){
		return base*10^exponent;
	}
	void update(float value){
		base = frexp(value, &exponent);
	}
};

class Node{
	private:
		int id;
		float input;
		float output;
		Link* horizontal;
		Link* vertical;
		Node* next;
	public:
		Node( ){};
		Node( int );
		int 	getId(){	return id;	};
		int	getOutput(){	return output;	};
		Node*	getNext(){	return next;	};

		void setInput( float );
		void setNext( Node* );
		void setVertical( Link* );
		void setHorizontal( Link* );

		void pushForward();
		void reset(){		input=output=0;	};
};

class Link{
	private:
		int	id;
		Node* 	to;
		Floaty*	weight;
		Link*	next;
	public:
		Link(){};
		Link( int, float, Node* );

		int getId(){		return id;	};
		Floaty* getWeight(){	return weight;	};
		Link* getNext(){	return next;	};
		Node* getNode(){	return to;	};

		void setNext( Link* );
		void updateWeight( float );
};

class Network{
	private:
		int	id;
		Node*	first;
		Link*	head;
		char calcOutput();
	public:
		Network(){};
		Network( int, char* );

		int getId(){		return id;	};
		Link* getLink(){	return head->getNext();	};
		//char recognise( Picture* );
		//void train( Picture* );
		void save();
		void couple( Network* );
		char pushForward();
};

class Data{
	private:
		char	value;
		float*	pixels;
	public:
		Data();

		char	getValue(){	return value;	};
		float*	getPixels(){	return pixels;	};
};

//Node class implementation

Node::Node( int ident ){
	id = ident;
	reset();
}

void Node::setVertical( Link* vert ){
	vertical=vert;
}

void Node::setHorizontal( Link* hor ){
	horizontal=hor;
}

void Node::setInput( float value ){
	input += value;
	output = input;
}

void Node::setNext( Node* value ){
	next = value;
}

void Node::pushForward(){
	horizontal->getNode()->setInput( horizontal->getWeight()->toFloat()*output );
	vertical->getNode()->setInput( vertical->getWeight()->toFloat()*output );
}

//Link class implementation

Link::Link( int ident, float value, Node* next ){
	id = ident;
	weight = new Floaty((float)value);
	to = next;
}

void Link::setNext( Link* linker ){
	next = linker;
}

void Link::updateWeight( float newW ){
	weight->update(newW);
}

//Network class implementation

char Network::calcOutput(){
	return abs((int)first[119].getOutput());
}

Network::Network( int ident, char* file){
	float value;
	Node* foo;
        ifstream in;
        in.open(file);

	Link* fooler;
	int i;
	foo = first = new Node( 1 );
	for( i=0;i<120;i++){
		foo->setNext( new Node( i+2 ) );
		foo = foo->getNext();
	}
        fooler = head = new Link();
	foo = first;
	for( i=0;i<120;i++ ){
		in >> value;
		if(i<100){
			fooler->setNext( new Link( 2*i , value , &(first[i%10+99]) ) );
			foo->setVertical( fooler );
			fooler = fooler->getNext();
			in.ignore();
			in >> value;
			fooler->setNext( new Link( 2*i+1 , value , &(first[i/10+109]) ) );
			foo->setHorizontal( fooler );
		}else{
			fooler->setNext( new Link( i+100 , value , &(first[119]) ) );
			foo->setVertical( fooler );
			foo->setHorizontal( fooler );
		}
		fooler = fooler->getNext();
		foo = foo->getNext();
		in.ignore();
	}
}
/**
void Network::save(){
	int i;
	std::ifstream out = new ifstream();
	out.open("save%d.dta", id);
	Link* foo = head->getNext();
	for(i=0;i<250;i++){
		out << foo->getWeight() << '\n';
		foo = foo->getNext();
	}
}**/

void Network::couple(Network* net){
	int i, mut;
	Floaty* help;
	Link* foo = net->getLink();
	Link* fooler = head->getNext();
	for(i=0;i<220;i++){
		mut=rand()%32;
		if(rand()%1 == 1){
			help = fooler->getWeight();
			if(mut<18)
				help->base ^= (1<<mut);
			else{
				if(mut > 30)
					help->base = -help->base;
				else
					if(mut>17 && mut<24)
						help->exponent ^= (1<<mut-18);
			}
			foo->updateWeight( help->toFloat() );
		}else{
			mut=rand()%32;
                	if(rand()%1 == 1)
        	        	help = foo->getWeight();
	                if(mut<18)
                	        help->base ^= (1<<mut);
        	        else{
	                        if(mut > 24)
                                	help->base = -help->base;
                        	else
                	                help->exponent ^= (1<<mut-18);
        	        }
	                foo->updateWeight( help->toFloat() );
		}
		foo = foo->getNext();
		fooler = fooler->getNext();
	}
}

char Network::pushForward(){
	int i;
	for( i=0;i<120;i++ ){
		first[i].pushForward();
	}
	return calcOutput();
}

//General functions implementation

int main(){

	return 0;
}
