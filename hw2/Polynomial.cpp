#include<iostream>
#include<cmath>
using namespace std;
class Polynomial;

class Term{
	friend Polynomial;
	friend ostream& operator<<(ostream &output,const Polynomial &Poly);
	private:
		float coef;  //�Y��
		int exp;     //����
};

class Polynomial{
	private:
		Term *termArray;  //�}�C���� 
		int capacity;     //�}�C�j�p 
		int terms;        //�}�C���D�s�Ʀr 
	public:
		Polynomial():capacity(2),terms(0){
			termArray = new Term[capacity];
		}
		
		Polynomial Add(Polynomial b);
		
		Polynomial Mult(Polynomial b);
		
		float Eval(float x);
		
		void newTerm(const float newcoef,const int newexp);
		
		friend istream& operator>>(istream &input,Polynomial &Poly);
		friend ostream& operator<<(ostream &output,const Polynomial &Poly);
};
Polynomial Polynomial::Add(Polynomial b){
	Polynomial c;
	int aPos=0,bPos=0;
	while((aPos<terms)&&(bPos<b.terms))
		if(termArray[aPos].exp==b.termArray[bPos].exp){             //�p�G���Ƭ۵��N�ۥ[ 
			float t=termArray[aPos].coef+b.termArray[bPos].coef;    //�Y�Ƭۥ[ 
			if(t) c.newTerm(t,termArray[aPos].exp);
			aPos++; bPos++;                                         //���U�@�ӼƦr���� 
		}
		else if(termArray[aPos].exp<b.termArray[bPos].exp){         //�p�Gb�����Ƹ��j 
			c.newTerm(b.termArray[bPos].coef,b.termArray[bPos].exp);//�N��b���Y�ƻP���ƥ[�J�쵲�G�h���� 
			bPos++;
		}
		else{
			c.newTerm(termArray[aPos].coef,termArray[aPos].exp);    //��a���Y�ƻP���ƥ[�J�쵲�G�h���� 
			aPos++;
		}
		
	for(;aPos;aPos++)
		c.newTerm(termArray[aPos].coef,termArray[aPos].exp);        //��Ѿl�����[�i�h   
	for(;bPos;bPos++)
		c.newTerm(b.termArray[bPos].coef,b.termArray[bPos].exp);    //��Ѿl�����[�i�h  
	return c;	                                                    //��^���G�h���� 
}
Polynomial Polynomial::Mult(Polynomial b){
	Polynomial c;
	for(int aPos=0;aPos<terms;aPos++){                                 //�T�O�|��C�@���i��ۭ� 
		for(int bPos=0;bPos<terms;bPos++){ 
			float newcoef=termArray[aPos].coef*b.termArray[bPos].coef; //�Y�Ƭۭ� 
			int newexp=termArray[aPos].exp*b.termArray[bPos].exp;      //���Ƭۥ[ 
			c.newTerm(newcoef,newexp);                                 //�[�J�h���� 
		}     	                           		
	}
	return c;
}
float Polynomial::Eval(float x){
	float sum=0;                                                 
	for(int aPos=0;aPos<terms;aPos++){
		sum=sum+termArray[aPos].coef*pow(x,termArray[aPos].exp);  //��C�ӼƳ��p��X�Өìۥ[ 
	}
	return sum;                                                   //�^�ǵ��G 
} 
void Polynomial::newTerm(const float theCoef,const int theExp){
	if(terms==capacity){
		capacity*=2;
		Term *temp=new Term[capacity];
		copy(termArray,termArray+terms,temp);
		delete []termArray;
		termArray=temp;
	}
	termArray[terms].coef=theCoef;
	termArray[terms++].exp=theExp;
}
istream& operator>>(istream &input,Polynomial &Poly){
	char ch;
    int coef = 0, exp = 0;
    bool isNegative = false;

    while (input >> ch) {
        if (isdigit(ch)) {                    //�ˬd�O�_���Ʀr 
            input.putback(ch);                //��^�h����Ū������Ʀr
            input >> coef;
            if (isNegative) coef = -coef;
            isNegative = false;
        } else if (ch == 'X') {               //�B�z�ܼ�X
            if (input.peek() == '^') {
                input.get();                  //���L '^'
                input >> exp;                 //Ū������
            } else {
                exp = 1;                      //�p�G�S�g����A�N��@�OX^1
            }
        } else if (ch == '+' || ch == '-') {  //�P�_�B��l 
            Poly.newTerm(coef, exp);          //�[�J�}�C 
            coef = 0; exp = 0;
            isNegative = (ch == '-');
        }
    }
    // �[�J�̫�@��
    Poly.newTerm(coef, exp);
    return input;
}
ostream& operator<<(ostream &output, const Polynomial &Poly) {
    for (int aPos = 0; aPos < Poly.terms; aPos++) {
        if (Poly.termArray[aPos].coef > 0 && aPos != 0) {
            output << "+";                                //�Y�Ƭ����ɿ�X���� 
        }
        output<<Poly.termArray[aPos].coef;

        if (Poly.termArray[aPos].exp != 0) {              // �Y���Ƭ� 0�A�N���ο�X X 
            output<<"X";
            if (Poly.termArray[aPos].exp != 1) {          // ���Ƭ� 1 �ɬٲ� ^1
                output<<"^"<<Poly.termArray[aPos].exp;
            }
        }
    }

    return output;
}

int main(){
	Polynomial p1;
    p1.newTerm(3, 2);
    p1.newTerm(-4, 1);  
    p1.newTerm(5, 0);  

    cout << p1 << endl; 
    return 0;
}
