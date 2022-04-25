#include <iostream>
#include <fstream>

using namespace std;

extern ifstream sourceFile;
extern ofstream outFile, listFile;

#include "FScan.h"
#include "FCodeGen.h"


extern Scanner scan; //Global scanner

CodeGen::CodeGen(){
    maxTemp =0;
	intoff = 0;
	fakoff = 0;
	stroff = 0;
	booloff =0;
	fifId = 0;
	whlId = 0;
}
//-------Private-Methods-------
string CodeGen::kindtoStr(const ExprKind& t){
	switch(t){
		case ID_EXPR:
            return "ID_EXPR";
		case IDF_EXPR:
			return "IDF_EXPR";
		case IDB_EXPR:
			return "IDB_Expr";
		case IDS_EXPR:
			return "IDS_EXPR";
		case LITERAL_INT:
			return "LITERAL_INT";
		case TEMP_EXPR:
			return "TEMP_EXPR";
		case TEMPF_EXPR:
            return "TEMPF_EXPR";
		case LITERAL_STR:
			return "LITERAL_STR";
		case LITERAL_FAKE:
			return "LITERAL_FAKE";
		case LITERAL_BOOL: 
			return "LITERAL_BOOL";
        default: return "";
	}

}
void CodeGen::CheckId(const string & s, ExprKind & t)
{
	if (!LookUp(s,t))  // variable not declared yet
        Enter(s,t);
}
void CodeGen::Enter(const string & s, ExprKind & t )
{
	//fakes, bools and ints
    //symbolTable.push_back(s);

    Symbol thisSym;
    cout << s << " Enter(s) before pushback symbolTable\n";
    thisSym.label = s;
    thisSym.kind = t;
	
	switch(t){
		//case LITERAL_INT:
		case ID_EXPR:
			thisSym.off = intoff;
			intoff +=2;
			break;
		case IDF_EXPR:
		//case LITERAL_FAKE:
			//fakeTable.push_back("0.0");
			thisSym.off = fakoff;
			fakoff +=4;
			break;
		case IDB_EXPR:
			thisSym.off = booloff;
			booloff +=2;
			break;
		case IDS_EXPR:
		case LITERAL_STR:
			//cerr<<"here ids expr" << stringTable[0];
			thisSym.off = stroff;
			stroff += 34;
			//stroff+= StringSamDistance(stringTable.size());
            break;
        default: 
			cout<<"here";
			break;
	}
    symbolTable.push_back(thisSym);


}
int CodeGen::getOff(string& s){
	for (unsigned i=0; i< symbolTable.size(); i++){
		if(symbolTable[i].label ==s){
			return symbolTable[i].off;
		}
	}
	return 0;
}
bool CodeGen::LookUp(const string & s, ExprKind & t)
{
	//loop through all symbol tables int bool string fake
    for (unsigned i =0; i < symbolTable.size(); i++){
		
        if (symbolTable[i].label == s){
            t = symbolTable[i].kind;
			switch(t){
				case LITERAL_INT:
					t = ID_EXPR;
					break;
				case LITERAL_FAKE:
					t=IDF_EXPR;
					break;
				case LITERAL_BOOL:
					t = IDB_EXPR;
					break;
				case LITERAL_STR:
					t = IDS_EXPR;
					break;
                default: break;
			}
			return true;
		}
		
    }


	return false;
}
void CodeGen::IntToAlpha(int val, string& str)
{
	int k;
	char temp;

	str = "";
	if (val == 0) str = "0";
	while (val > 0)
	{
		str.append(1, (char)(val % 10 + (int)'0'));
		val /= 10;
	}
	k = int(str.length());
	for (int i = 0; i < k/2; i++)
	{
		temp = str[i];
		str[i] = str[k-i-1];
		str[k-i-1] = temp;
	}
}
void CodeGen::FakeToAlpha(float val, string& str)
{

    str = "";

    cout << "\nFakeToAlpha\n";

    string fakeStr(std::to_string(val));

    str = fakeStr;
    cout << str;



}
void CodeGen::ExtractExpr(const ExprRec & e, string& s){
	string t;
    int k, n;
	//cout<< kindtoStr(e.kind);
	n=0;
	s = e.name;
	switch (e.kind)
	{
	case ID_EXPR:
		//cout<<"id expr";
	case TEMP_EXPR:  // operand form: +k(R15)
        while (symbolTable[n].label != s) n++;
		IntToAlpha(symbolTable[n].off, t); // offset: 2 bytes per variable
		s = "+" + t + "(R15)";
		break;
	case IDF_EXPR:
    case TEMPF_EXPR:  // operand form +k(R14)
        while(symbolTable[n].label != s) n++;
		k = symbolTable[n].off;
		IntToAlpha(k,t);
		//IntToAlpha(symbolTable[n].off, t);
        s = "+" + t + "(R14)";
        break;
	case IDB_EXPR:
		while(symbolTable[n].label != s) n++;
		k = symbolTable[n].off;
		IntToAlpha(k,t);
		s = "+" + t + "(R12)";
		break;
	case IDS_EXPR:
		while(symbolTable[n].label != s) n++;
		k = symbolTable[n].off;
		IntToAlpha(k,t);
		s = "+" + t + "(R12)";
		break;
	case LITERAL_INT:
		IntToAlpha(e.val,s);
		s = "#" + s;
		break;
	case LITERAL_STR:
		//s ="+" + to_string(StringSamDistance(stringTable.size()-1)) +"(R13)";
		//cout<<"str";
		s = "+" + to_string(StringSamDistance(stringTable.size()-1)) + "(R10)";
		break;
		//s = "+" + to_string(StringSamDistance(stringTable.size()-1))+ "(R14)";
		//s ="+" + to_string(StringSamDistance(stringTable.size()-1)) +"(R14)";
	//WIP case for bool and fakes
    case LITERAL_BOOL:
		IntToAlpha(e.val,t);
		s = "#" + t;
        break;
    case LITERAL_FAKE:
        //cout << "---literal fake\n";
		//while(symbolTable[n].label!=e.name)n++;
		IntToAlpha(symbolTable[n].off,s);
        s = "+" + s + "(R11)";
        break;
	default:
		cout<<"--Default";
		break;
	}
}
void CodeGen::Generate(const string & s1, const string & s2, const string & s3){
	
	listFile.width(20);
	listFile << ' ' << s1;
	outFile << s1;
	if (s2.length() > 0)
	{
		listFile << s2;
		outFile << s2;
		if (s3.length() > 0)
		{
			listFile << ',' << s3;
			outFile << ',' << s3;
		}
	}
	listFile << endl;
	outFile << endl;
}
string CodeGen::ConvertToSam(string s){
	string tmp ="\"";
    for(unsigned i =0; i<s.length(); i++){
		if(s[i]=='"' || s[i]==':'){
			tmp+=":";
		}
		tmp += s[i];
	}
	tmp +="\"";
	return tmp;
}
int CodeGen::StringSamDistance(int index){
	if(index ==0){
		return 0;
	}
	int tmp =0;
	for(int i=0; i<index; i++){
		tmp += stringTable[i].length();
		if(tmp%2==0){
			tmp+=2;
		}else{
			tmp+=1;
		}
	}
	stringTable[index];
	return tmp;
}
//--------Public-Methods--------
void CodeGen::Start()
{
	Generate("LDA		", "R15", "INTS");
	Generate("LDA		", "R14", "FAKES");
	Generate("LDA		", "R11", "FAKELIT");
	Generate("LDA		", "R13", "STRS");
	Generate("LDA		", "R10", "STRLIT");
	Generate("LDA		", "R12", "BOOLS");
	//WIP
	//Generate load address for STRS BOOLS fakes
	//R14 , R13, R12
}
void CodeGen::Finish()

{
	string s;
	listFile.width(6);
	listFile << ++scan.lineNumber << " " << scan.lineBuffer << endl;
	Generate("HALT		", "", "");
	//repeat this chunk for fakes, bools, strings
	//keep in mind the differnt sizes of registers
	Generate("LABEL	", "INTS","");
	int tmpSize =0;
    for(unsigned i=0; i< symbolTable.size(); i++){
		if(symbolTable[i].kind == ID_EXPR) tmpSize +=1;
	}
    IntToAlpha(int(2*(tmpSize)),s);
	Generate("SKIP	", s, "");
	//WIP Need tables for str bool and fake
    Generate("LABEL	", "BOOLS","");
	tmpSize =0;
    for(unsigned i=0; i< symbolTable.size(); i++){
		if(symbolTable[i].kind == LITERAL_BOOL) tmpSize +=1;
	}
    IntToAlpha(int(2*(tmpSize)),s);
	Generate("SKIP	", s, "");

    Generate("LABEL	", "FAKELIT", "");
    for(unsigned i=0; i< fakeTable.size(); i++){
		Generate("REAL	",fakeTable[i],"");
	}
	Generate("LABEL	", "FAKES", "");
	tmpSize =0;
    for(unsigned i=0; i< symbolTable.size(); i++){
		if(symbolTable[i].kind == IDF_EXPR) tmpSize +=1;
	}
    IntToAlpha(int(4*(tmpSize)),s);
    Generate("SKIP	", s, "");
	Generate("LABEL	", "STRS", "");
	tmpSize =0;
    for(unsigned i=0; i< symbolTable.size(); i++){
		if(symbolTable[i].kind == IDS_EXPR) tmpSize +=1;
	}
    IntToAlpha(int(34*(tmpSize)),s);
    Generate("SKIP	", s, "");
	Generate("LABEL	", "STRLIT", "");
    //IntToAlpha(int(4*(fakeTable.size()+1)),s);
    //for loop for stings
    for(unsigned i=0; i< stringTable.size(); i++){
		cout<<ConvertToSam(stringTable[i]);
		Generate("STRING	",ConvertToSam(stringTable[i]),"");
	}
	Generate("LABEL	","YAY","");
	Generate("STRING	","\"YAY\"","");
	Generate("LABEL	","NAY","");
	Generate("STRING	","\"NAY\"","");
	outFile.close();
    for(unsigned i =0; i <symbolTable.size(); i++){
		cout<<symbolTable[i].label<<symbolTable[i].off<<endl;

	}


    listFile << endl << endl;
    listFile << " _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_\n";
    listFile << " ||||||||   S Y M B O L   T A B L E   ||||||||\n" << endl;
    listFile << " _____________________________________________" << endl;
    listFile << " Relative" << endl;
    listFile << " Address      Identifier" << endl;
    listFile << " --------     --------------------------------" << endl;

    int curOffSet = 0;

    for (int j = 0; j < 4; j++)
    {
        switch(j)
        {
          case 0: listFile << " INTS----" << endl; break;
          case 1: listFile << " BOOLS---" << endl; break;
          case 2: listFile << " FAKES---" << endl; break;
          case 3: listFile << " STRINGS-" << endl; break;
        }

        for (unsigned i = 0; i < symbolTable.size(); i++)
        {
            switch(j)
            {
            case 0: if (symbolTable[i].kind == ID_EXPR || symbolTable[i].kind == TEMP_EXPR)
                    {
                        listFile.width(7);
                        listFile << curOffSet << "        " << symbolTable[i].label << endl;
                        curOffSet += 2;
                    }
                    break;
            case 1: if (symbolTable[i].kind == LITERAL_BOOL)  //Temporay -- needs to be IDB_EXPR || TEMPB_EXPR
                    {
                    listFile.width(7);
                        listFile << curOffSet << "      " << symbolTable[i].label << endl;
                        curOffSet += 2;
                    }
                    break;
            case 2: if (symbolTable[i].kind == IDF_EXPR || symbolTable[i].kind == TEMPF_EXPR)
                    {
                        listFile.width(7);
                        listFile << curOffSet << "      " << symbolTable[i].label << endl;
                        curOffSet += 4;
                    }
                    break;
            case 3: if (symbolTable[i].kind == LITERAL_STR)
                    {


                    listFile << "        " << symbolTable[i].label << endl;
                        //figure string offset here
                    }
                    break;
            default: break;
                        }

           // if ( symbolTable[i].kind == )
           // listFile.width(7);
           // listFile << 2*i << "       " << symbolTable[i].label << endl;
        }

    }
    listFile << " _-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_-_" << endl;
    listFile << endl;
    listFile << " Normal successful compilation." << endl;


//--- START Debug Vectors
    string p = "######################################################";
    string d = "###-----------------------------------------------";
    listFile << endl << endl << endl;
    listFile << "DEBUG VECTORS" << endl << p << endl;
    listFile << "###  vector<Symbol> symbolTable" << endl << d << endl;
    string st, sk, so, sl, ls, lt, lo;
    unsigned sti = symbolTable.size();
    int o;
    IntToAlpha(sti, st);
    listFile << "### Symbol(KIND, OFF, LABEL)" << endl;
    listFile << "###  symbolTable.size() = " << st << endl << d << endl;
    ls = ""; lt = ""; lo = "";
    for (unsigned j = 0; j < sti; j++)
    {
        ExprKind k = symbolTable[j].kind;
        IntToAlpha(j,st);
        sk = kindtoStr(k);
        o = symbolTable[j].off;
        IntToAlpha(o, so);
        sl = symbolTable[j].label;
        if (j < 10) ls = " "; else ls = "";
        if (k == ID_EXPR) lt = "     "; else if (k == IDF_EXPR) lt = "    "; else if (k == TEMP_EXPR) lt = "   ";
        else if (k == TEMPF_EXPR) lt = "  "; else if (k == LITERAL_INT || k == LITERAL_STR) lt = " ";
        if (o < 10) lo = " +"; else lo = "+";
        listFile << "###" << ls << " [" << st << "]->Symbol(" << sk << "," << lt << lo << so << ", " << sl << " )" << endl;
    }
    listFile << d << endl << p << endl;
    listFile << "### vector<string> fakeTable" << endl << d << endl;
    sti = fakeTable.size();
    IntToAlpha(sti, st);
    listFile << "### fakeTable.size() = " << st << endl << d << endl;

    for (unsigned k = 0; k < sti; k++)
    {
        IntToAlpha(k,st);
        listFile << "### [" << st << "]->string(\"" << fakeTable[k] << "\")" << endl;
    }
    listFile << d << endl << p << endl;
    listFile << "### vector<string> stringTable" << endl << d << endl;
    sti = stringTable.size();
    IntToAlpha(sti, st);
    listFile << "### stringTable.size() = " << st << endl << d << endl;
    for (unsigned l = 0; l < sti; l++)
    {
        IntToAlpha(l,st);
        listFile << "### [" << st << "]->string(\"" << stringTable[l] << "\")" << endl;
    }
    listFile << d << endl << p << endl;
 //--- END Debug Vectors



    listFile.close();


}
void CodeGen::Assign(const ExprRec & target, const ExprRec & source)
{
//needs to check if its an int fake boolean or string
	string s,id,id1;
	int tmp;
	string s2;
	ExprKind sKind,tKind;
	switch(source.kind){
		case LITERAL_INT:
			cout<< "assign int";
			ExtractExpr(source, s);
			Generate("LD		", "R0", s);
			ExtractExpr(target, s);
			id =source.name;
			tmp = getOff(id);
			IntToAlpha(tmp,id);
			cout << tmp;
			Generate("STO		", "R0", "+"+id+"(R15)");
			break;
		case LITERAL_FAKE:
			cout<<"assign fake";
			ExtractExpr(source, s);
			IntToAlpha(source.val,s);
			s = "+" + s + "(R11)";
			Generate("LD		", "R0", s);
			id =target.name;
			tmp = getOff(id);
			IntToAlpha(tmp,id);
			s = "+" + id + "(R14)";
			Generate("STO		", "R0", s);
			break;
		case LITERAL_STR:
		//buggy random mem for r10
			ExtractExpr(source, s);
			//IntToAlpha(source.val,s);
			//s = "+" + s + "(R10)";
			
			Generate("LD		", "R0", "R10");
			//add starting point of str
			
			IntToAlpha(StringSamDistance(stringTable.size()-2),s);
			Generate("IA		", "R0","#"+s);
			//set length of string
			IntToAlpha(stringTable.back().size(),s);
			Generate("LD		","R1","#"+s);
			id =target.name;
			tmp = getOff(id);
			IntToAlpha(tmp,id);
			s = "+" + id + "(R13)";
			Generate("BKT		", "R0", s);
			break;
		case LITERAL_BOOL:
			ExtractExpr(source, s);
			Generate("LD		", "R0", s);
			ExtractExpr(target, s);
			id =target.name;
			tmp = getOff(id);
			IntToAlpha(tmp,id);
			cout << tmp;
			Generate("STO		", "R0", "+"+id+"(R12)");
			break;
		case ID_EXPR:
			//should check actual type.
			
			for(int i=0; i<symbolTable.size();i++ ){
				if(symbolTable[i].label == source.name){
					sKind = symbolTable[i].kind;
				}
				if(symbolTable[i].label == target.name){
					tKind = symbolTable[i].kind;
				}
			}
			id = source.name;
			tmp = getOff(id);
			IntToAlpha(tmp,id);
			id1 =target.name;
			tmp = getOff(id1);
			IntToAlpha(tmp,id1);
			if(sKind ==tKind){
				if(sKind ==ID_EXPR){
					Generate("LD		", "R0", "+"+id+"(R15)");
					Generate("STO		", "R0", "+"+id1+"(R15)");
				}else if(sKind ==IDF_EXPR){
					Generate("LD		", "R0", "+"+id+"(R14)");
					Generate("STO		", "R0", "+"+id1+"(R14)");
				}else if(sKind == IDS_EXPR){
					Generate("LD		", "R0", "+"+id+"(R10)");
					Generate("STO		", "R0", "+"+id1+"(R13)");
				}
			}
			//ExtractExpr(source, s);
			//ExtractExpr(target, s);
			
			break;
    default: break;
	}
	
	
}
void CodeGen::ReadValue(const ExprRec & InVal)
{
	string s,id;
	int tmp;
	
	ExprKind kind;
	CheckId(InVal.name,kind);//gets var type from name
	//LookUp(s,kind);
	switch(kind){
		case ID_EXPR:
		case TEMP_EXPR:
		case LITERAL_INT:
			//tmp.kind = ID_EXPR;
			
			ExtractExpr(InVal, s);//gets address for var
			id =InVal.name;
			tmp = getOff(id);
			IntToAlpha(tmp,id);
			Generate("RDI		", "+"+id+"(R15)", "");
			break;
		case IDF_EXPR:
		case TEMPF_EXPR:
		case LITERAL_FAKE:
			
			ExtractExpr(InVal, s);//gets address for var
			Generate("RDF		", s, "");
			break;
		case LITERAL_STR:
		case IDS_EXPR:
			ExtractExpr(InVal, s);
			id =InVal.name;
			tmp = getOff(id);
			IntToAlpha(tmp,id);
			Generate("RDST	", "+"+id+"(R13)", "");
			break;
		case LITERAL_BOOL:
		case IDB_EXPR:
			Generate("RDST		", s, "");
			//check if yay or nay
			// convert to int zero or one.
			//sto conversion.
			break;
		default:
			break;
	}
	
	
}
void CodeGen::ProcessVariable()
{

}
void CodeGen::WriteExpr(const ExprRec & outExpr)
{
	//WIP .kind of lit str bool and fake
	string s, id;
	int tmp;
    //ExprKind kind;
	ExprKind kind;
	ExtractExpr(outExpr,s);
	//LookUp(outExpr.name,kind);
	
	switch(outExpr.kind){
		case LITERAL_STR:
		case LITERAL_BOOL:
			Generate("WRST	", s, "");
			break;
		case ID_EXPR:
		case IDF_EXPR:
			cout<<"id";
			id =outExpr.name;
            for(unsigned i=0; i<symbolTable.size(); i++){
				if(symbolTable[i].label == id){
					kind = symbolTable[i].kind;
				}
			}
			tmp = getOff(id);
			IntToAlpha(tmp,id);
			if(kind ==ID_EXPR){
				Generate("WRI		", "+"+id+"(R15)", "");
			}else if(kind == IDF_EXPR){
				Generate("WRF		", "+"+id+"(R14)", "");
			}else if(kind == IDS_EXPR){
				Generate("WRST	", "+" + id + "(R13)","");
			}else{
				//boolean check if 1 or 0
				//call label yay or nay
				//if statement
				//increment jump
			}
			break;
		case TEMP_EXPR:
		case LITERAL_INT:
			//outExpr.kind = TEMP_EXPR;
			//ExtractExpr(outExpr,s);
			//id =outExpr.name;
			//tmp = getOff(id);
			//IntToAlpha(tmp,id);
			Generate("WRI		", s, "");
			break;
		case LITERAL_FAKE:
			//s = "+" + s + "(R14)";
		case TEMPF_EXPR:
			//ExtractExpr(outExpr,s);
			
			Generate("WRF		", s, "");
			break;
		default:
			cout<<"\ndefault write\n";
	}
}
void CodeGen::NewLine()
{
	Generate("WRNL		", "", "");
}
bool CodeGen::isFake(ExprKind& kind){
	switch(kind){
		case LITERAL_FAKE:
		case IDF_EXPR:
		case TEMPF_EXPR:
			cout<<"fake";
			return true;
			break;
		default:
			return false;
	}
}
bool CodeGen::isInt(ExprKind& kind){
	switch(kind){
		case LITERAL_INT:
		case ID_EXPR:
		case TEMP_EXPR:
			cout<<"int";
			return true;
			break;
		default:
			return false;
	}
}
void CodeGen::Condition(ExprRec& expr,ConRec& con,ExprRec& expr2){
	string s;
	LookUp(expr.name,expr.kind);
	LookUp(expr2.name,expr2.kind);
	ExtractExpr(expr,s);
	cout<<"\n here "<< kindtoStr(expr.kind)<<"\n";
	if(isInt(expr.kind) && isInt(expr2.kind)){
		Generate("LD		","R0",s);
		ExtractExpr(expr2,s);
		Generate("LD		","R1",s);
		Generate("IC		","R0","R1");
	}else if(isFake(expr.kind) && isFake(expr2.kind)){
		//second fake not offsetting properly
		Generate("LD		","R0",s);
		ExtractExpr(expr2,s);
		Generate("LD		","R2",s);
		Generate("FC		","R0","R2");
	}else{
		cout<<"\n"<< kindtoStr(expr.kind)<<"\n";
	}
}
void CodeGen::Jump(ConRec& con,string& label){
	switch(con.con){
		case NE:
			Generate("JEQ		",label,"");
			break;
		case EQ:
			Generate("JNE		",label,"");
			break;
		case LT:
			Generate("JGE		",label,"");
			break;
		case GT:
			Generate("JLE		",label,"");
			break;
		case LE:
			Generate("JGT		",label,"");
			break;
		case GE:
			Generate("JLT		",label,"");
			break;
	}
}
void CodeGen::NIJump(ConRec& con,string& label){
	switch(con.con){
		case EQ:
			Generate("JEQ		",label,"");
			break;
		case NE:
			Generate("JNE		",label,"");
			break;
		case GE:
			Generate("JGE		",label,"");
			break;
		case LE:
			Generate("JLE		",label,"");
			break;
		case GT:
			Generate("JGT		",label,"");
			break;
		case LT:
			Generate("JLT		",label,"");
			break;
	}
}
/*---fif---
(Left,conditional op, right)
Compare Registers, address , in line literals first
	IC OR FC  R0 or +0(R15) or #0 , compared too
	JEQ, JNE,JGT,JLT,JGE,JLE
	jump the opposite of compared then jmp 
*/
void CodeGen::ProcessIf(ExprRec& expr,ConRec& con,ExprRec& expr2)
{
	//writes conditional stmt
	Condition(expr,con,expr2);
	string s,a;
	s= "ENDIF";
	IntToAlpha(fifId++,a);
	s = s+a;
	fifStack.push_back(s);
	//Gens flipped conditional jump
	Jump(con,s);
}
//Pops from fif stack and gens label
void CodeGen::ProcessEndIf()
{
	//pop from stack
	string s,label;
	s= "LABEL	";
	label = fifStack.back();
	fifStack.pop_back();
	Generate(s,label,"");
}
//pops from stack. 
//gens JMP to new label. 
//creates label from stack.
void CodeGen::ProcessElse()
{
	string s,a,label;
	s= "ENDIF"; // get from stack
	IntToAlpha(fifId++,a);
	s = s+a;
	Generate("JMP		",s,"");
	a= "LABEL	";
	label = fifStack.back();
	fifStack.pop_back();
	Generate(a,label,"");
	fifStack.push_back(s);

}
//---do-while---
//Gen Label
void CodeGen::ProcessDo()
{
	//writes conditional stmt
	string s,a;
	s= "WHL";
	IntToAlpha(whlId++,a);
	s = s+a;
	Generate("LABEL	",s,"");
	whileStack.push_back(s);
	//Gens flipped conditional jump
	
}
//jump back to label
void CodeGen::ProcessEndFwhile(ExprRec& Lexpr,ConRec& con,ExprRec& Rexpr)
{
	Condition(Lexpr,con,Rexpr);
	string label;
	label = whileStack.back();
	whileStack.pop_back();
	NIJump(con,label);
	//Jump(con,label);//jmp needs to be inverted.
}
//---while---
void CodeGen::ProcessWhile(ExprRec& Lexpr,ConRec& con,ExprRec& Rexpr)
{
	string s,a,top,end;
	s = "WHL";
	IntToAlpha(whlId++,a);
	top = s +a;
	Generate("LABEL	",top,"");
	IntToAlpha(whlId++,a);
	end = s +a;
	whileStack.push_back(end);
	whileStack.push_back(top);
	Condition(Lexpr,con,Rexpr);
	Jump(con,end);
}
void CodeGen::ProcessWhileCond()
{

}
void CodeGen::ProcessEndWhile()
{
	//pop from stack
	string s,label;
	label = whileStack.back();
	whileStack.pop_back();
	Generate("JMP		",label,"");//jmp to first
	s= "LABEL	";
	label = whileStack.back();
	whileStack.pop_back();
	Generate(s,label,"");//exit label
}
//---for-loop---
void CodeGen::InitLoopCtrl()
{
	
}
void CodeGen::ProcessForCond(ExprRec& Lexpr,ConRec& con,ExprRec& Rexpr)
{
	string label,a,cond,loop,update,exit;
	label = "FOR";
	//gen label condition label
	IntToAlpha(whlId++,a);
	cond = label + a;
	Generate("LABEL	",cond,"");
	//gen loop label
	IntToAlpha(whlId++,a);
	loop = label + a;
	//gen update label
	IntToAlpha(whlId++,a);
	update = label + a;
	//gen exit label
	IntToAlpha(whlId++,a);
	exit = label + a;
	//push to stack
	whileStack.push_back(exit);
	whileStack.push_back(update);
	whileStack.push_back(loop);
	whileStack.push_back(cond);
	//first jump to loop
	Condition(Lexpr,con,Rexpr);
	NIJump(con,loop);//non inverse
	//if condition not met exit
	Generate("JMP	",exit,"");
	//gen update label
	Generate("LABEL	",update,"");
}
void CodeGen::LoopUpdate()
{
	//updates before
	string label,loop,top,a;
	//jump to condition
	top = whileStack.back();
	whileStack.pop_back();
	Generate("JMP	",top,"");
	//set label for loop
	loop = whileStack.back();
	whileStack.pop_back();
	Generate("LABEL	",loop,"");
}
void CodeGen::ProcessEndFor()
{
	string update,exit;
	//jmp to update
	update = whileStack.back();
	whileStack.pop_back();
	Generate("JMP		",update,"");
	//exit
	exit = whileStack.back();
	whileStack.pop_back();
	Generate("LABEL		",exit,"");
}
//---Conditions---
void CodeGen::SetOrCond()
{

}
void CodeGen::SetAndCond()
{

}
void CodeGen::SetNotCond()
{

}
//---Ops---
void CodeGen::ProcessRelOp()
{

}
void CodeGen::SetRelCond()
{

}
void CodeGen::ProcessOp(OpRec& o)
{
	
	string s = scan.tokenBuffer;
	if(s =="+") o.op=PLUS;
	else if (s=="-") o.op =MINUS;
	else if (s=="*") o.op =MULT;
    else if (s=="/") o.op =DIV;
	else if (s=="%") o.op =MOD;
	cout<<"processop"<<s<<endl;
}
string CodeGen::GetTemp(){
	string s;
	static string t;
	t = "Temp&";
	IntToAlpha(++maxTemp, s);
	t += s;
	ExprRec tmp;
	tmp.kind = TEMP_EXPR;
    CheckId(t, tmp.kind);  // forcing TEMP_EXPR for quick fix
	return t;
}
string CodeGen::GetTempF(){
    string s;
    static string t;
    t = "Temp&";
    IntToAlpha(++maxTemp, s);
    t += s;
    ExprRec tmp;
    tmp.kind = TEMPF_EXPR;
    CheckId(t, tmp.kind);  // forcing TEMPF_EXPR for quick fix
    return t;
}
string CodeGen::ExtractOp(const OpRec& o, ExprKind & k){
	//needs it for floats and floats and ints

    switch(k)
    {
        case ID_EXPR:
        case TEMP_EXPR:
        case LITERAL_INT:
            switch(o.op)
            {
                case PLUS:  return "IA		";
                case MINUS: return "IS		";
                case MULT:  return "IM		";
                case DIV:   return "ID		";
                default:    return "Default IA		";  //--- TEMP default IA
                //WIP
                //find sam instructions for mul div mod
            }
        case IDF_EXPR:
        case TEMPF_EXPR:
        case LITERAL_FAKE:
            switch(o.op)
            {
                case PLUS:  return "FA      ";
                case MINUS: return "FS      ";
                case MULT:  return "FM      ";
                case DIV:   return "FD      ";
                default:    return "Default FA      "; //--- TEMP default FA
            }
        case LITERAL_BOOL:  return "";
        case LITERAL_STR:   return "";

    }

    return "";
}

void CodeGen::GenInfix(const ExprRec & e1, const OpRec & op, const ExprRec & e2, ExprRec& e)
{
    string s;
    string tmp;
    int isE1_int = -1;  //set these, so no warnings below
    int isE2_int = -1;
    int isFakes = -1;

    if (e.kind == TEMP_EXPR || e.kind == ID_EXPR || e.kind == LITERAL_INT)           isFakes = 0;                // int result
    else if ( e.kind == TEMPF_EXPR || e.kind == IDF_EXPR || e.kind == LITERAL_FAKE)   isFakes = 1;                 // fake result
         else cout << "\nGenInfixError: invalid result type assignment:" << e.name << "\n"; // wth result

    switch(e1.kind)
    {
        case LITERAL_INT:
        case TEMP_EXPR:
        case ID_EXPR:
        case LITERAL_BOOL:  isE1_int = 1; break;
        case LITERAL_FAKE:
        case TEMPF_EXPR:
        case IDF_EXPR:      isE1_int = 0; break;
    default: cout << "Default-e1.kind\n"; break;
    }
    switch(e2.kind)
    {
        case LITERAL_INT:
        case TEMP_EXPR:
        case ID_EXPR:
        case LITERAL_BOOL:  isE2_int = 1; break;
        case LITERAL_FAKE:
        case TEMPF_EXPR:
        case IDF_EXPR:      isE2_int = 0; break;
    case LITERAL_STR: break;
    default: cout << "Default-e2.kind\n"; break;
    }

//--- START temp cout
    string sf, se1, se2;
    IntToAlpha(isFakes, sf);
    IntToAlpha(isE1_int, se1);
    IntToAlpha(isE2_int, se2);
    cout << "\nGenInfix{isFakes=" << sf << "; isE1_int=" << se1 << "; isE2_int=" << se2 << "}\n";
    sf = kindtoStr(e.kind);
    se1 = kindtoStr(e1.kind);
    se2 = kindtoStr(e2.kind);
    cout << "GenInfix{e.kind=" << sf << "; e1.kind=" << se1 << "; e2.kind=" << se2 << ";}\n";
//--- END temp cout

    if ((e1.kind == LITERAL_INT && e2.kind == LITERAL_INT) || (e1.kind == LITERAL_FAKE && e2.kind == LITERAL_FAKE))
    {
        if(e1.kind == LITERAL_INT && e2.kind == LITERAL_INT) {
            e.kind = LITERAL_INT;
            switch(op.op){
                case PLUS:  e.val = e1.val + e2.val; break;
                case MINUS:	e.val = e1.val - e2.val; break;
                case MULT:  e.val = e1.val * e2.val; break;
                case DIV:   e.val = e1.val / e2.val; break;
                case MOD:   e.val = e1.val % e2.val; break;
            }
        } else if(e1.kind == LITERAL_FAKE && e2.kind == LITERAL_FAKE){
            e.kind = LITERAL_FAKE;
            switch(op.op){
                case PLUS:  e.valF = e1.valF + e2.valF; break;
                case MINUS: e.valF = e1.valF - e2.valF; break;
                case MULT:  e.valF = e1.valF * e2.valF; break;
                case DIV:   e.valF = e1.valF / e2.valF; break;
                default: break;
            }
        }
    } //-- END of PURE LITERALS
    else{

        if (isFakes == 0)  //--- All INTS
        {
            e.kind = TEMP_EXPR;
            e.name = GetTemp();
            ExtractExpr(e1,s);

			string id;
			int off;
			id =e1.name;
			off = getOff(id);
			IntToAlpha(off,id);

            Generate("LD		","R0","+"+id+"(R15)");
            switch(op.op){
                case PLUS:
                    cout<<"GenInfixPLUS-"; break;
                case MINUS:
                    cout<<"GenInfixMIN-"; break;
                default:
                    cout<<"GenInfixDefaultOP"; break;
            }
            ExtractExpr(e2, s);
            tmp = ExtractOp(op, e.kind);
            cout<< tmp<<"here\n";

            Generate(tmp, "R0", s);
            ExtractExpr(e, s);
			//string id;
			//int off;
            id =e1.name;
			off = getOff(id);
			IntToAlpha(off,id);
			Generate("STO		", "R0", "+"+id+"(R15)");
			Generate("%comment	","break","");
            //Generate("STO		", "R0", s);
        }
        else if (isFakes == 1)//--- Doing Fakes: All Combo's with INT TO FAKE Conversions
        {
            int foff;
            string fid;

            e.kind = TEMPF_EXPR;
            e.name = GetTempF();

            ExtractExpr(e1,s);

            if (isE1_int == 1)  //WIP do int to float conversion for e1
            {
                Generate("e1:FLT     ", "R0", s);
            }
            else // double load from R14
            {
                fid = e1.name;
                foff = getOff(fid);
                IntToAlpha(foff, fid);
                Generate("LD        ", "R0", "+" + fid + "(R14)");
                foff += 2;
                IntToAlpha(foff, fid);
                Generate("LD        ", "R1", "+" + fid + "(R14)");
            }
            // need extra for e1 Literal_Fake in R11 here or in else above


            ExtractExpr(e2,s);

            if (isE2_int == 1) //WIP do int to float conversion for e2 and store it in R2
            {
                Generate("e2:FLT     ", "R2", s);  //needs to be different reg?
            }
            else // double load from R14 put in R2 & R3
            {
                fid = e2.name;
                foff = getOff(fid);
                Generate("LD        ", "R2", "+" + fid + "(R14)");
                foff += 2;
                IntToAlpha(foff, fid);
                Generate("LD        ", "R3", "+" + fid + "(R14)");

            }
            // need extra for e2 Literal_Fake in R11 here or in else above

            Generate(ExtractOp(op, e.kind), "R0", "R2");
            ExtractExpr(e, s);


            Generate("STO       ", "R0", s);  //STORE reult in R14

          }

          if (isFakes == -1) cout << "!Error: GenInfix result exp is INT or FAKE not defined!";
          if (isE1_int == -1) cout << "!Error: GenInfix e1 is INT or FAKE not defined!";
          if (isE2_int == -1) cout << "!Error: GenInfix e2 is INT or FAKE not defined!";



    } //---END else




}
void CodeGen::ProcessMulOp()
{

}
void CodeGen::ProcessId(ExprRec& e)
{
	//switching literal to id breaks
	//assignments and calling
    CheckId(scan.tokenBuffer,e.kind);

//--- START-temp-cout
    string i = kindtoStr(e.kind);
    cout << "\nProcessId{e.kind=" << i << "}\n";
//--- END-temp-cout

}
void CodeGen::ProcessLiteralInit(ExprRec& e)
{

}


void CodeGen::ProcessLiteral(ExprRec& e)
{
	string s;
	switch(e.kind){
		case LITERAL_INT:
			cout << "process lit int\n";
			//cout << scan.tokenBuffer.data();
			//e.kind = LITERAL_INT;
			e.val = atoi(scan.tokenBuffer.data());
			cout << e.val;
			
			//cout << e.val;
			break;
		case LITERAL_STR:
		//case IDS_EXPR:
			//push to string table.
			cerr << "process lit str"<<e.name;
			e.kind = LITERAL_STR;

			stringTable.push_back(scan.tokenBuffer.data());
			 
			//ExtractExpr(e,s);
			//Generate("WRST	", s,"");
			break;
		case LITERAL_BOOL:
			e.kind = LITERAL_BOOL;
			cout<<scan.tokenBuffer.data();
            //string y = "yay";
            if(scan.tokenBuffer.data() == string("yay") ) { e.val = 1; }
            else { e.val = 0; cout<<"false"; }
            break;
		
		case LITERAL_FAKE:
			cout<<"process fake";
            //cout << scan.tokenBuffer.data()<<"\n";
			e.valF = std::stof(scan.tokenBuffer.data());
            //e.valF = atoi(scan.tokenBuffer.data());
			//e.kind = LITERAL_FAKE;
			fakeTable.push_back(to_string(e.valF));
			//fakeTable.push_back(scan.tokenBuffer.data());
			e.val = (fakeTable.size()-1)*4;
			
            cout << scan.tokenBuffer.data();
			break;
		
		case TEMP_EXPR:
            cout << "TEMP expr";
			break;
       
        case TEMPF_EXPR:
            cout << "TEMPF expr";
            break;
		default:
			cout <<"default";
			break;
	}
	
}
/*
void CodeGen::ProcessStringLiteral(ExprRec& e)
{

}
*/
