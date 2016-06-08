#include "llvm/Pass.h"
#include "llvm/IR/CFG.h"
#include "llvm/Analysis/CFG.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/IR/Function.h"
#include "llvm/Analysis/LoopIterator.h"
#include "llvm/IR/Constants.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/ADT/APInt.h"
#include <stdlib.h>
#include <stdio.h>
#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <utility>
#include <set>

using namespace std;
using namespace llvm;

class Bounds : public ModulePass {
	public:
		static char ID;
		vector<string> params;
		map<string, map<string, vector<pair<string, int> > > > blocks_info;
		vector<string> programVars;
		map<string, int> temp_vars;
		map<string, vector<pair<string, int> > > arrays_size;
		bool size_flag;
		std::vector<BasicBlock*> block_vect;
		vector< pair <string, pair< vector< pair<string, int> > , vector< pair<string, int> > > > >result;

		Bounds() : ModulePass(ID) {
		}

		bool runOnModule(Module &M);
		void createInitialConstraints(Function &F);
		void makeBackwardPass(Function &);
		void printPostOrder(BasicBlock*);
		bool checkIfParam(string);
		void handleBinaryOperations(BinaryOperator* ops, map<string, vector<pair<string, int> > >&);
		vector<pair<string, int> > operandCheck(Value*, map<string, vector<pair<string, int> > >);
		vector<pair<string, int> > multiplyOperands(int op1, vector<pair<string, int> > );
		void addParams(map<string, vector<pair<string, int> > >&);
		void handleStore(StoreInst* strInst, map<string, vector<pair<string, int> > >&);
		void handleSext(User*, map<string, vector<pair<string, int> > >&);
		void handleCall(CallInst*, map<string, vector<pair<string, int> > >&);
		bool checkIfDeclared(string);
		map<string, vector<pair<string, int> > > blockTraversal();
		void traverseSucc(BasicBlock*);
		bool stackCheck(string block_name);
		void instructionsEvaluator(Instruction*, map<string, vector<pair<string, int> > >&);
		void printConrtraints();
		void printExpr(vector<pair<string, int> >);
};

void  Bounds::printExpr(vector<pair<string, int> > expr){
	unsigned int len= params.size();
	unsigned int len_expr= expr.size();
	int coeff=0;
	for(unsigned int i=0; i< len; i++){
		coeff = 0;
		for(unsigned int j=0; j< len_expr; j++){
			if(expr[j].first == params[i]){
				coeff= expr[j].second;
				break;
			}
		}
		cout << coeff <<" ";
	}
}

void Bounds::printConrtraints(){
	params.push_back("");
	unsigned int len= result.size();
	for(unsigned int i=0; i< len; i++){
		cout << result[i].first <<" ";
		vector<pair<string, int> > index_expr= result[i].second.first;
		vector<pair<string, int> > size_expr= result[i].second.second;
		printExpr(index_expr);
		printExpr(size_expr);
		cout <<endl;
	}
}

bool Bounds::checkIfDeclared(string temp){
	int len= programVars.size();
	for(int i=0; i< len; i++)
		if(programVars[i]== temp)
			return true;
	return false;
}

void Bounds::addParams(map<string, vector<pair<string, int> > >& temp_map){
	unsigned int len= params.size();
	for(unsigned int i=0; i< len ; i++)
		temp_map[params[i]]= vector<pair<string, int> >{make_pair(params[i],1)};
}

vector<pair<string, int> > Bounds::operandCheck(Value* op, map<string, vector<pair<string, int> > > temp_map){
	vector<pair<string, int> > temp_vect;
	if(ConstantInt::classof(op)){
		uint64_t op_int= dyn_cast<ConstantInt>(op)->getLimitedValue();
		temp_vect.push_back(make_pair("", op_int));
	}else{
		string op_str= op->getName();
		if(op_str=="")
			op_str= dyn_cast<User>(op)->getOperand(0)->getName();
		if(temp_map.find(op_str) != temp_map.end())
			temp_vect= temp_map[op_str];
	}
	return temp_vect;
}

vector<pair<string, int> > Bounds::multiplyOperands(int op1, vector<pair<string, int> > op2_info){
	vector<pair<string, int> > vect_var;
	int len= op2_info.size();
	for(int i=0; i< len; i++)
		vect_var.push_back(make_pair(op2_info[i].first, op2_info[i].second * op1));
	return vect_var;
}

void Bounds::handleBinaryOperations(BinaryOperator *ops, map<string, vector<pair<string, int> > >& temp_map){
	string varName= ops->getName();
	string opCode = ops->getOpcodeName();
	Value* op1= ops->getOperand(0);
	Value* op2= ops->getOperand(1);
	vector<pair<string, int> > vect_var;
	vector<pair<string, int> > op1_info, op2_info;
	op1_info = operandCheck(op1, temp_map);
	op2_info = operandCheck(op2, temp_map);
	int len1= op1_info.size();
	int len2= op2_info.size();
	
	if(len1 > 0 && len2 > 0 ){
		if(opCode=="mul"){	
			if(op1_info[0].first=="")
				vect_var= multiplyOperands(op1_info[0].second, op2_info);
			else vect_var = multiplyOperands(op2_info[0].second, op1_info);
			//exit(0);
		}else if(opCode=="add"){
			for(int i=0; i< len1; i++)
				vect_var.push_back(make_pair(op1_info[i].first, op1_info[i].second));
			
			for(int i=0; i< len2; i++){
				pair<string, int> pair_ = op2_info[i];
				bool flag=false;
				for(int j=0; j< len1; j++){
					if(pair_.first == vect_var[j].first){
						vect_var[j].second += pair_.second;
						flag= true;
						break;
					}
				}
				if(!flag)
					vect_var.push_back(make_pair(pair_.first, pair_.second));
			}	
		}else if(opCode=="sub"){
			for(int i=0; i< len1; i++)
				vect_var.push_back(make_pair(op1_info[i].first, op1_info[i].second));
			
			for(int i=0; i< len2; i++){
				pair<string, int> pair_ = op2_info[i];
				bool flag=false;
				for(int j=0; j< len1; j++){
					if(pair_.first == vect_var[j].first){
						vect_var[j].second -= pair_.second;
						flag= true;
						break;
					}
				}
				if(!flag)
					vect_var.push_back(make_pair(pair_.first, -1*pair_.second));
			}	
		}
	}
	temp_map[varName]= vect_var;
}

bool Bounds::checkIfParam(string operand){
	for(unsigned int i=0; i< params.size(); i++)
		if(params[i]==operand)
			return true;
	return false;
}

void Bounds::handleStore(StoreInst* strInst, map<string, vector<pair<string, int> > >& temp_map){
	Value* src= strInst->getValueOperand();
	Value* dest= strInst->getPointerOperand();
	string destName=  dest->getName();
	vector<pair<string, int> > info_vect;
	if(ConstantInt::classof(src)){
		uint64_t src_int = dyn_cast<ConstantInt>(src)->getLimitedValue();
		info_vect.push_back(make_pair("", src_int));
	}else{
		string srcName= src->getName();
		if(srcName=="")
			srcName= dyn_cast<User>(src)->getOperand(0)->getName();
		if(temp_map.find(srcName) != temp_map.end()){
			vector<pair<string, int> > temp_vect= temp_map[srcName];
			int len=temp_vect.size();
			for(int i=0;i< len; i++)
				info_vect.push_back(temp_vect[i]);
		}	
	}
	if(size_flag){
		arrays_size[destName]= info_vect;
		size_flag=false;
	}else
		temp_map[destName]=info_vect;
}

void Bounds::handleSext(User* inst,map<string, vector<pair<string, int> > >&temp_map){
	string varName= inst->getName();
	Value* opd= inst->getOperand(0);
	vector<pair<string, int> > expr_vect;

	if(ConstantInt::classof(opd)){
		uint64_t val= dyn_cast<ConstantInt>(opd)->getLimitedValue();
		expr_vect.push_back(make_pair("", val));
	
	}else{
		string op_name= opd->getName();
		if(op_name=="")
			op_name= dyn_cast<User>(opd)->getOperand(0)->getName();

		if(temp_map.find(op_name) != temp_map.end()){
			vector<pair<string, int> > temp_vect= temp_map[op_name];
			int len= temp_vect.size();
			for(int i=0; i< len; i++)
				expr_vect.push_back(make_pair(temp_vect[i].first, temp_vect[i].second));
		}	
	}
	temp_map[varName] = expr_vect;
}

void Bounds::handleCall(CallInst* inst, map<string, vector<pair<string, int> > >&temp_map){
	string destName= inst->getName();
	Value* arg= inst->getArgOperand(0);
	vector<pair<string, int> > size_vect;

	if(ConstantInt::classof(arg)){
		uint64_t size= dyn_cast<ConstantInt>(arg)->getLimitedValue();
		size_vect.push_back(make_pair("", size/4));
	
	}else{
		string argName= arg->getName();
		if(argName=="")
			argName= dyn_cast<User>(arg)->getOperand(0)->getName();

		if(temp_map.find(argName) != temp_map.end()){
			vector<pair<string, int> > temp_vect= temp_map[argName];
			int len= temp_vect.size();
			for(int i=0; i< len; i++)
				size_vect.push_back(make_pair(temp_vect[i].first, temp_vect[i].second/4));
		}
	}
	temp_map[destName]= size_vect;
}

bool Bounds::runOnModule(Module &M) {
	//forward pass to collect the information about the assignment expressions
	for (Module::iterator F = M.begin(), E = M.end(); F != E; ++F) {
	 //      if((*F).getName()=="fun")
			createInitialConstraints(*F);
			break;
	} 
	
	return false;
}

void Bounds::instructionsEvaluator(Instruction* I, map<string, vector<pair<string, int> > >& block_info){
	string opCode= I->getOpcodeName();
		if(opCode=="alloca"){
			User* inst= dyn_cast<User>(I);
			programVars.push_back(inst->getName());
		}

		if(opCode=="store"){
			StoreInst* SI= dyn_cast<StoreInst>(I);
			handleStore(SI, block_info);
					
		}else if(opCode=="add"){
			BinaryOperator *ops= dyn_cast<BinaryOperator>(I);
			handleBinaryOperations(ops, block_info);
					
		}else if(opCode=="sub"){
			BinaryOperator* ops= dyn_cast<BinaryOperator>(I);
			handleBinaryOperations(ops, block_info); 
					
		}else if(opCode=="mul"){
			BinaryOperator* ops= dyn_cast<BinaryOperator>(I);
			handleBinaryOperations(ops, block_info);			
					
		}else if(opCode=="sext"){
			User* val= dyn_cast<User>(I);
			handleSext(val, block_info);
					
		}else if(opCode=="call"){
			CallInst* inst= dyn_cast<CallInst>(I);
			handleCall(inst, block_info);
			size_flag=true;
		}				
}

map<string, vector<pair<string, int> > > Bounds::blockTraversal(){

	arrays_size.clear();
	unsigned int len= block_vect.size();
	map<string, vector<pair<string, int> > > block_info;
	addParams(block_info);
	
	for(unsigned int i=0; i< len-1; i++){
		BasicBlock *BB = block_vect[i];
		
		string block_name= BB->getName();
		if(block_name.substr(0,5)=="while" && block_name.substr(6,4)=="body"){
			block_info.clear();
			addParams(block_info);
		}

		for(BasicBlock::iterator I= BB->begin(), E= BB->end(); I != E; I++){
			instructionsEvaluator(I,block_info);
		}
	}
	//cout << arrays_size.size() <<endl;
	return block_info;
}		

bool Bounds::stackCheck(string block_name){
	unsigned int len= block_vect.size();
	for(unsigned int i=0; i<len; i++)
		if(block_vect[i]->getName() == block_name)
			return true;
	return false;
}

void Bounds::traverseSucc(BasicBlock* block){
	block_vect.push_back(block);
	//outs() << block->getName() << "\n";
	for(BasicBlock::iterator I= block->begin(), E= block->end(); I != E; I++){
		string opcode= I->getOpcodeName();
		User* instr= dyn_cast<User>(I);
		string varName= instr->getName();

		if(opcode=="getelementptr"){
			map<string, vector<pair<string, int> > > pred_info = blockTraversal();
			
			for(BasicBlock::iterator I_new = block->begin(), E_new = block->end(); I_new != E_new; I_new++){
				User* instr_new= dyn_cast<User>(I_new);
				if(instr_new->getName() == varName){
					// for(map<string, vector<pair<string, int> > >::iterator iter= pred_info.begin(); 
					// 	iter!= pred_info.end(); iter++){
					// 	//if(checkIfDeclared(iter->first)){
					// 		cout << iter->first <<": ";
					// 		for(unsigned int i=0; i < iter->second.size(); i++)
					// 			cout << iter->second[i].first <<"  " << iter->second[i].second <<" ";
					// 		cout <<endl;
					// 	//}
					// }
					// cout <<endl ;
					Value* arr= instr_new ->getOperand(0);
					Value* index= instr_new->getOperand(1);
					vector<pair<string, int> > size_expr, index_expr;
					
					string arrName= arr->getName();
					if(arrName=="")
						arrName= dyn_cast<User>(arr)->getOperand(0)->getName();
					if(arrays_size.find(arrName) != arrays_size.end())
						size_expr= arrays_size[arrName];

					if(ConstantInt::classof(index)){
						uint64_t size_ = dyn_cast<ConstantInt>(index)->getLimitedValue();
						index_expr.push_back(make_pair("",size_));
					}else{
						string indexVar= index->getName();
						if(indexVar == "")
							indexVar= dyn_cast<User>(index)->getOperand(0)->getName();
						if(pred_info.find(indexVar) != pred_info.end())
							index_expr= pred_info[indexVar];
					}
					int len1= size_expr.size();
					int len2= index_expr.size();
					if(len1==0 || len2==0){
						size_expr.clear();
						unsigned int num_params= params.size();
						for(unsigned int i=0; i< num_params; i++)
							size_expr.push_back(make_pair(params[i],0));
						size_expr.push_back(make_pair("",0));
						result.push_back(make_pair(varName, make_pair(size_expr, size_expr)));
					}else{
						result.push_back(make_pair(varName, make_pair(index_expr, size_expr)));
					}
					break;
				}
				instructionsEvaluator(I_new, pred_info);
			}
		} 
	}
	for(succ_iterator I= succ_begin(block), E= succ_end(block); I != E; I++){
		BasicBlock * succ= *I;
		string succName= succ->getName();
		if(stackCheck(succName)){
			TerminatorInst* inst= succ->getTerminator();
			unsigned int numSucc= inst->getNumSuccessors();
			for(unsigned int i=0; i< numSucc; i++){
				succ= inst->getSuccessor(i);
				if(!stackCheck(succ->getName()))
					break;
			}
		}
		traverseSucc(succ);
	}
	block_vect.pop_back();
}

void Bounds::createInitialConstraints(Function &F) {
	F.viewCFG();
	Function::ArgumentListType &arglist= F.getArgumentList();
	for(Function::ArgumentListType::iterator begin= arglist.begin(), end= arglist.end(); begin!=end; ++begin)
		params.push_back(begin->getName());
	//params.push_back("");
	size_flag=false;
	BasicBlock& BB= F.getEntryBlock();
	traverseSucc(&BB);
	printConrtraints();
}

// Register the pass.
char Bounds::ID = 0;
static RegisterPass<Bounds> X("safe", "ArrayIndex checking pass");