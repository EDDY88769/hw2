#include<iostream>
#include<cmath>
#include <sstream>
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
		
	for(;aPos<terms;aPos++)
		c.newTerm(termArray[aPos].coef,termArray[aPos].exp);        //把剩餘的項加進去   
	for(;bPos<b.terms;bPos++)
		c.newTerm(b.termArray[bPos].coef,b.termArray[bPos].exp);    //把剩餘的項加進去  
	return c;	                                                    //返回結果多項式 
}
Polynomial Polynomial::Mult(Polynomial b){
    Polynomial c;
    for(int aPos=0;aPos < terms;aPos++){ 
        for(int bPos=0;bPos<b.terms;bPos++){ 
            float newcoef=termArray[aPos].coef*b.termArray[bPos].coef;  //係數相乘
            int newexp=termArray[aPos].exp+b.termArray[bPos].exp;       //指數相加

            //檢查結果多項式中是否已有相同的指數
            bool found=false;
            for(int cPos=0;cPos<c.terms;cPos++){
                if(c.termArray[cPos].exp == newexp){  //如果已有相同的指數
                    c.termArray[cPos].coef+=newcoef;  //係數相加
                    found=true;
                    break;
                }
            }

            if(!found){  								//如果沒有找到相同的指數,就新增
                c.newTerm(newcoef,newexp);
            }
        }
    }

    //排序,確保按指數順序排列
    for(int i=0;i<c.terms-1;i++){
        for(int j=i+1;j<c.terms;j++){
            if(c.termArray[i].exp<c.termArray[j].exp){  //按指數從大到小排序
                //開始交換 
                Term temp=c.termArray[i];
                c.termArray[i]=c.termArray[j];
                c.termArray[j]=temp;
            }
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
	if (theCoef == 0) return;
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

istream& operator>>(istream &input,Polynomial &Poly) {
    string line;
    getline(input,line);

    istringstream stream(line); 
    char ch;
    int coef=0, exp=0;
    bool isNegative=false;
    bool coefSet=false;  //用來標記係數是否設定

    while(stream>>ch){
        if(isdigit(ch)){                    //檢查是否為數字
            stream.putback(ch);             //放回去後續讀取完整數字
            stream>>coef;
            if(isNegative)coef=-coef;
            isNegative=false;
            coefSet=true;                   //標記係數已設定
        }else if(ch=='X'){                  //處理變數X
            if(!coefSet){                   //如果沒有設定係數
                coef=isNegative ? -1 : 1;   //默認係數為正負1
                isNegative = false;
            }
            if(stream.peek()=='^') {
                stream.get();               //跳過'^'
                stream>>exp;                //讀取次方
            }else{
                exp=1;                      //如果沒寫次方,當作是X^1
            }
            coefSet=false;                  //處理完後重置
        }else if(ch=='+' || ch=='-'){       //判斷運算子
            if(coef!=0 || exp!=0){          //確保不儲存多餘項
                Poly.newTerm(coef,exp);
            }
            coef=0;
			exp=0;
            isNegative=(ch=='-');
        }
    }

    //加入最後一項   如果有的話 
    if(coef!=0 || exp!=0){
        Poly.newTerm(coef,exp);
    }

    return input;
}


ostream& operator<<(ostream &output,const Polynomial &Poly){
    for(int aPos=0;aPos<Poly.terms;aPos++){
        if(Poly.termArray[aPos].coef>0 && aPos!=0){
            output<<"+"; 						    //只有第1項後+才會顯示
        }

        if(Poly.termArray[aPos].coef<0){
            output<<"-"; 							//如果係數為負就加負號 
        }
        if(abs(Poly.termArray[aPos].coef)!=1 || Poly.termArray[aPos].exp==0){
            output<<abs(Poly.termArray[aPos].coef);
        }
        
        if(Poly.termArray[aPos].exp!=0){        //只要指數不為零就輸出X 
            output<<"X";
            if(Poly.termArray[aPos].exp!=1){    //如果次方為1就不輸出^這個符號 
                output<<"^"<<Poly.termArray[aPos].exp;
            }
        }
    }
    return output;
}


int main(){
	Polynomial p1;  //第一個多項式 
	Polynomial p2;  //第二個多項式
	float x1,x2; 
	cout<<"請以這樣的格式輸入5X^4+X^2+1"<<endl;
	cout<<"請輸入多項式"<<endl<<"p1:"; 
	cin>>p1;        //輸入第一個多項式 
	cout<<"p2:";
	cin>>p2;        //輸入第二個多項式  
    cout<<"請輸入欲代入多項式的X值"<<endl<<"x1:";
	cin>>x1;        //輸入x1 
	cout<<"x2:";
	cin>>x2;        //輸入x2
	//計算p1代入x1的值並印出 
	cout<<"p1:"<<p1<<"代入"<<x1<<"的值為:"<<p1.Eval(x1)<<endl;
	//計算p2代入x2的值並印出 
	cout<<"p2:"<<p2<<"代入"<<x2<<"的值為:"<<p2.Eval(x2)<<endl;
	//計算p1+p2的值並印出 
	Polynomial p3=p1.Add(p2);
    cout<<p1<<" 和 "<<p2<<" 相加為:"<<p3<<endl;
    //計算p1xp2的值並印出 
	Polynomial p4=p1.Mult(p2);
    cout<<p1<<" 和 "<<p2<<" 相乘為:"<<p4<<endl;  
    return 0;
}
