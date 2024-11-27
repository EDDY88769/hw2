#include<iostream>
#include<cmath>
using namespace std;
class Polynomial;

class Term{
	friend Polynomial;
	friend ostream& operator<<(ostream &output,const Polynomial &Poly);
	private:
		float coef;  //係數
		int exp;     //指數
};

class Polynomial{
	private:
		Term *termArray;  //陣列本體 
		int capacity;     //陣列大小 
		int terms;        //陣列內非零數字 
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
		if(termArray[aPos].exp==b.termArray[bPos].exp){             //如果指數相等就相加 
			float t=termArray[aPos].coef+b.termArray[bPos].coef;    //係數相加 
			if(t) c.newTerm(t,termArray[aPos].exp);
			aPos++; bPos++;                                         //往下一個數字移動 
		}
		else if(termArray[aPos].exp<b.termArray[bPos].exp){         //如果b的指數較大 
			c.newTerm(b.termArray[bPos].coef,b.termArray[bPos].exp);//就把b的係數與指數加入到結果多項式 
			bPos++;
		}
		else{
			c.newTerm(termArray[aPos].coef,termArray[aPos].exp);    //把a的係數與指數加入到結果多項式 
			aPos++;
		}
		
	for(;aPos;aPos++)
		c.newTerm(termArray[aPos].coef,termArray[aPos].exp);        //把剩餘的項加進去   
	for(;bPos;bPos++)
		c.newTerm(b.termArray[bPos].coef,b.termArray[bPos].exp);    //把剩餘的項加進去  
	return c;	                                                    //返回結果多項式 
}
Polynomial Polynomial::Mult(Polynomial b){
	Polynomial c;
	for(int aPos=0;aPos<terms;aPos++){                                 //確保會對每一項進行相乘 
		for(int bPos=0;bPos<terms;bPos++){ 
			float newcoef=termArray[aPos].coef*b.termArray[bPos].coef; //係數相乘 
			int newexp=termArray[aPos].exp*b.termArray[bPos].exp;      //指數相加 
			c.newTerm(newcoef,newexp);                                 //加入多項式 
		}     	                           		
	}
	return c;
}
float Polynomial::Eval(float x){
	float sum=0;                                                 
	for(int aPos=0;aPos<terms;aPos++){
		sum=sum+termArray[aPos].coef*pow(x,termArray[aPos].exp);  //把每個數都計算出來並相加 
	}
	return sum;                                                   //回傳結果 
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
        if (isdigit(ch)) {                    //檢查是否為數字 
            input.putback(ch);                //放回去後續讀取完整數字
            input >> coef;
            if (isNegative) coef = -coef;
            isNegative = false;
        } else if (ch == 'X') {               //處理變數X
            if (input.peek() == '^') {
                input.get();                  //跳過 '^'
                input >> exp;                 //讀取次方
            } else {
                exp = 1;                      //如果沒寫次方，就當作是X^1
            }
        } else if (ch == '+' || ch == '-') {  //判斷運算子 
            Poly.newTerm(coef, exp);          //加入陣列 
            coef = 0; exp = 0;
            isNegative = (ch == '-');
        }
    }
    // 加入最後一項
    Poly.newTerm(coef, exp);
    return input;
}
ostream& operator<<(ostream &output, const Polynomial &Poly) {
    for (int aPos = 0; aPos < Poly.terms; aPos++) {
        if (Poly.termArray[aPos].coef > 0 && aPos != 0) {
            output << "+";                                //係數為正時輸出正號 
        }
        output<<Poly.termArray[aPos].coef;

        if (Poly.termArray[aPos].exp != 0) {              // 若指數為 0，就不用輸出 X 
            output<<"X";
            if (Poly.termArray[aPos].exp != 1) {          // 指數為 1 時省略 ^1
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
