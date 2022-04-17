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
			return "ID_Expr";
		case IDF_EXPR:
			return "IDF_EXPR";
		case LITERAL_INT:
			return "LITERAL_INT";
		case TEMP_EXPR:
			return "TEMP_EXPR";
		case TEMPF_EXPR:
			return "TEMP_EXPR";
		case LITERAL_STR:
			return "LITERAL_STR";
		case LITERAL_FAKE:
			return "LITERAL_FAKE";
		case LITERAL_BOOL: 
			return "LITERAL_BOOL";
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
		case LITERAL_INT:
			thisSym.off = intoff;
			//thisSym.kind = ID_EXPR;//tset
			intoff +=2;
			break;
		case LITERAL_FAKE:
			thisSym.off = fakoff;
			fakoff +=4;
			break;
		case LITERAL_BOOL:
			thisSym.off = booloff;
			booloff +=2;
			break;
		case LITERAL_STR:
			thisSym.off = stroff;
			stroff+= StringSamDistance(stringTable.size()-1);
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

	switch (e.kind)
	{
	case ID_EXPR:
		//cout<<"id expr";
	case TEMP_EXPR:  // operand form: +k(R15)
		
		s = e.name;
		n = 0;
		//cout<<"temp";
        while (symbolTable[n].label != s) n++;
        //k = 2 * n;
		//IntToAlpha(k,t);
		IntToAlpha(symbolTable[n].off, t); // offset: 2 bytes per variable
		s = "+" + t + "(R15)";
		break;
	case IDF_EXPR:
    case TEMPF_EXPR:  // operand form +k(R14)
        s = e.name;
        n = 0;
        while(symbolTable[n].label != s) n++;
        //k = 2 * n;
		k = symbolTable[n].off;
		IntToAlpha(k,t);
		//IntToAlpha(symbolTable[n].off, t);
        s = "+" + t + "(R14)";
        break;
	case LITERAL_INT:
		cout <<"lit int\n";
		n=0;
		while(symbolTable[n].label!=e.name)n++;
		//keeps getting last var as e.name
		//while(symbolTable[++n].label!=e.name);
		//cout<< symbolTable[--n].off<<endl;
		//IntToAlpha(symbolTable[--n].off,t);
		//IntToAlpha(e.val, t);
		
		IntToAlpha(e.val,s);
		s = "#" + s;
		//s = "+" + t+"(R15)";
		break;
	case LITERAL_STR:
		s ="+" + to_string(StringSamDistance(stringTable.size()-1)) +"(R13)";
		//cout<<"str";
		break;
		//s = "+" + to_string(StringSamDistance(stringTable.size()-1))+ "(R14)";
		//s ="+" + to_string(StringSamDistance(stringTable.size()-1)) +"(R14)";
	//WIP case for bool and fakes
    case LITERAL_BOOL:
		IntToAlpha(e.val,t);
		s = "#" + t;
        break;
    case LITERAL_FAKE:
        cout << "---literal fake\n";
		n = 0;
		while(symbolTable[n].label!=e.name)n++;
		
		IntToAlpha(symbolTable[n].off,s);
		
        s = "+" + s + "(R14)";
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
	Generate("LDA		", "R13", "STRS");
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
		if(symbolTable[i].kind == LITERAL_INT) tmpSize +=1;
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

    Generate("LABEL	", "FAKES", "");
    for(unsigned i=0; i< fakeTable.size(); i++){
		Generate("REAL	",fakeTable[i],"");
	}
	
	tmpSize =0;
    for(unsigned i=0; i< symbolTable.size(); i++){
		if(symbolTable[i].kind == LITERAL_FAKE) tmpSize +=1;
	}
    IntToAlpha(int(4*(tmpSize)),s);
    Generate("SKIP	", s, "");
	
	Generate("LABEL	", "STRS", "");
    //IntToAlpha(int(4*(fakeTable.size()+1)),s);
    //for loop for stings
    for(unsigned i=0; i< stringTable.size(); i++){
		cout<<ConvertToSam(stringTable[i]);
		Generate("STRING	",ConvertToSam(stringTable[i]),"");
	}

	outFile.close();
	for(int i =0; i <symbolTable.size(); i++){
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
            case 0: if (symbolTable[i].kind == LITERAL_INT)
                    {
                        listFile.width(7);
                        listFile << curOffSet << "        " << symbolTable[i].label << endl;
                        curOffSet += 2;
                    }
                    break;
            case 1: if (symbolTable[i].kind == LITERAL_BOOL)
                    {
                        listFile.width(7);
                        listFile << curOffSet << "      " << symbolTable[i].label << endl;
                        curOffSet += 2;
                    }
                    break;
            case 2: if (symbolTable[i].kind == LITERAL_FAKE)
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

    listFile.close();


}
void CodeGen::Assign(const ExprRec & target, const ExprRec & source)
{
//needs to check if its an int fake boolean or string
	string s,id;
	int tmp;
	string s2;
	switch(source.kind){
		case LITERAL_INT:
			cout<< "assign int";
			ExtractExpr(source, s);
			Generate("LD		", "R0", s);
			ExtractExpr(target, s);
			id =target.name;
			tmp = getOff(id);
			IntToAlpha(tmp,id);
			cout << tmp;
			Generate("STO		", "R0", "+"+id+"(R15)");
			break;
		case LITERAL_FAKE:
			cout<<"assign fake";
			ExtractExpr(source, s);
			s = "+" + s + "(R14)";
			//Generate("LD		", "R0", s);
			ExtractExpr(target, s);
			IntToAlpha(stoi(s)+2,s);
			s = "+" + s + "(R14)";
			//Generate("LD		", "R1", s);

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
			
			ExtractExpr(InVal, s);
			Generate("RDST	", s, "");
			break;
		case LITERAL_BOOL:
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
	ExprKind kind;
	
	ExtractExpr(outExpr,s);
	//LookUp(outExpr.name,kind);
	
	switch(outExpr.kind){
		case LITERAL_STR:
		case LITERAL_BOOL:
			Generate("WRST	", s, "");
			break;
		case ID_EXPR:
		case TEMP_EXPR:
		case LITERAL_INT:
			//outExpr.kind = TEMP_EXPR;
			ExtractExpr(outExpr,s);
			
			id =outExpr.name;
			tmp = getOff(id);
			IntToAlpha(tmp,id);
			Generate("WRI		", "+"+id+"(R15)", "");
			break;
		case IDF_EXPR:
		case LITERAL_FAKE:
			//s = "+" + s + "(R14)";
		case TEMPF_EXPR:
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
void CodeGen::ProcessDo()
{

}
void CodeGen::ProcessEndFwhile()
{

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
void CodeGen::ProcessForCond()
{

}
void CodeGen::LoopUpdate()
{

}
void CodeGen::ProcessEndFor()
{

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
	tmp.kind = LITERAL_INT;
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
    tmp.kind = LITERAL_FAKE;
    CheckId(t, tmp.kind);  // forcing TEMP_EXPR for quick fix
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


}
/*
void CodeGen::GenInfix(const ExprRec & e1, const OpRec & op, const ExprRec & e2, ExprRec& e)
{
	string s;
	string tmp;
	cout<< kindtoStr(e.kind);
	if(e1.kind == ID_EXPR && e2.kind == ID_EXPR){
		e.kind = ID_EXPR;
		switch(op.op){
            case PLUS:  e.val = e1.val + e2.val; break;
            case MINUS:	e.val = e1.val - e2.val; break;
            case MULT:  e.val = e1.val * e2.val; break;
            case DIV:   e.val = e1.val / e2.val; break;
            case MOD:   e.val = e1.val % e2.val; break;
		}
	//}//else{ if(e1.kind == IDF_EXPR && e2.kind == IDF_EXPR){
		
        //e.kind = IDF_EXPR;
        //switch(op.op){
        //    case PLUS:  e.valF = e1.valF + e2.valF; break;
        //    case MINUS: e.valF = e1.valF - e2.valF; break;
        //    case MULT:  e.valF = e1.valF * e2.valF; break;
        //    case DIV:   e.valF = e1.valF / e2.valF; break;
        //    default: break;
       // }
    }else{
		
		e.kind = TEMP_EXPR;
		e.name = GetTemp();
		ExtractExpr(e1,s);
		Generate("LD		","R0",s);
		switch(op.op){
			case PLUS:
				cout<<"plust";
			case MINUS:
				cout<<"min";
			default:
				cout<<"default";
		}
		ExtractExpr(e2, s);
        tmp = ExtractOp(op, e.kind);
		cout<< tmp<<"here\n";
		
		Generate(tmp, "R0", s);
		ExtractExpr(e, s);
		Generate("STO		", "R0", s);

	}

    
}*/
void CodeGen::GenInfix(const ExprRec & e1, const OpRec & op, const ExprRec & e2, ExprRec& e)
{
    string s;
    string tmp;
    bool isE1_int = true;  //set these, so no warnings below
    bool isE2_int = true;
    bool isFakes = true;;

    switch(e1.kind)
    {
        case LITERAL_INT:
        case TEMP_EXPR:
        case ID_EXPR:
        case LITERAL_BOOL:  isE1_int = true; break;
        case LITERAL_FAKE:
        case TEMPF_EXPR:
        case IDF_EXPR:      isE1_int = false; break;
    default: cout << "Default-e1.kind\n"; break;
    }
    switch(e2.kind)
    {
        case LITERAL_INT:
        case TEMP_EXPR:
        case ID_EXPR:
        case LITERAL_BOOL:  isE2_int = true; break;
        case LITERAL_FAKE:
        case TEMPF_EXPR:
        case IDF_EXPR:      isE2_int = false; break;
    case LITERAL_STR: break;
    default: cout << "Default-e2.kind\n"; break;
    }
    if (isE1_int && isE2_int) isFakes = false;
    else isFakes = true;


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

        if (!isFakes)  //--- All INTS
        {
            e.kind = TEMP_EXPR;
            e.name = GetTemp();
            ExtractExpr(e1,s);
            Generate("LD		","R0",s);
            switch(op.op){
                case PLUS:
                    cout<<"GenInfixPLUS-";
                case MINUS:
                    cout<<"GenInfixMIN-";
                default:
                    cout<<"GenInfixDefaultOP";
            }
            ExtractExpr(e2, s);
            tmp = ExtractOp(op, e.kind);
            cout<< tmp<<"here\n";

            Generate(tmp, "R0", s);
            ExtractExpr(e, s);
            Generate("STO		", "R0", s);
        }
        else //--- Doing Fakes: All Combo's with INT TO FAKE Conversions
        {
            e.kind = TEMPF_EXPR;
            e.name = GetTempF();

            ExtractExpr(e1,s);
            Generate("LD        ", "R0", s);
            if (isE1_int)
                Generate("e1:FLT     ", "R0", s);

            ExtractExpr(e2,s);
            if (isE2_int)
                Generate("e2:FLT     ", "R0", s);  //needs to be different reg?

            Generate(ExtractOp(op, e.kind), "R0", s);
            ExtractExpr(e, s);
            Generate("STO       ", "R0", s);

          }



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
	switch(e.kind){
		case LITERAL_INT:
			//e.kind = ID_EXPR;
			break;
		case LITERAL_FAKE:
			//e.kind = IDF_EXPR;
			break;
	}
	//e.kind = ID_EXPR;
	//cout << scan.tokenBuffer;
	//e.name = scan.tokenBuffer;
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
			e.kind = LITERAL_INT;
			e.val = atoi(scan.tokenBuffer.data());
			cout << e.val;
			
			//cout << e.val;
			break;
		case LITERAL_STR:
			//push to string table.
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
			e.kind = LITERAL_FAKE;
			
			fakeTable.push_back(scan.tokenBuffer.data());
			
            cout << scan.tokenBuffer.data();
			break;
		case ID_EXPR:
            cout << "ID expr";
			break;
		case TEMP_EXPR:
            cout << "TEMP expr";
			break;
        case IDF_EXPR:
            cout << "IDF expr in literal process";
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
