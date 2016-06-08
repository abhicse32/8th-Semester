// Sample LLVM Pass file for A4 (CS6843)

#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Function.h"
#include "llvm/IR/DerivedTypes.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include "llvm/Analysis/LoopIterator.h"
#include "llvm/IR/Constants.h"
#include <set>
#include <map>
#include <iostream>
#include <vector>
#include <string>
#include <cstdio>

using namespace llvm;
using namespace std;

/**
 * This class encapsulates an LLVM instruction along with the source-file line number 
 * from where this instruction has been derived.
 * NOTE: getDebugLoc() used in the constructor will not work if we don't use "-g"
 *       switch in clang.
 */
class DebugInst {
	public:
		Instruction* instruction; 	// pointer to an LLVM instruction.
		int line; 					// line number of that source-code statement from which this instruction has been generated.

		DebugInst(Instruction* inst) {
			instruction = inst;
			line = instruction->getDebugLoc().getLine();
		}

		bool operator<(DebugInst other) const {
		 	if (line < other.line) {
				return true;
			} else if (line > other.line) {
				return false;
			} else {
				return instruction < other.instruction;
			}
		}
};

/**
 * This class represents the notion of a phase, as explained in the Specs.
 * Note a minor change in the set instSet:
 * Instead of just the line number, we also store the pointer to the 
 * Instruction that may get executed in this phase. 
 * This is needed to enable race condition detection.
 */
class Phase {
	public:
		set<int> startSet; 			// line numbers (in source file) of barriers where this phase starts.
		set<int> endSet; 			// line numbers (in source file) of barriers where this phase ends.
		set<DebugInst> instSet; 	// instructions (alongwith their line numbers in source file) that may get executed in this phase.

		// Generally, the argument to this constructor will be the ending set of barriers of the previous phase.
		Phase(set<int> *inSet) {
			for(set<int>::iterator it = inSet->begin(); it != inSet->end(); it++) {
				startSet.insert(*it);
			}
		}

		void setEndSetAndInstSet(set<int> endBarriers, set<DebugInst> phaseInstructions){
			for(set<int>::iterator it= endBarriers.begin(), end= endBarriers.end(); it != end; ++it)
				endSet.insert(*it);
			for(set<DebugInst>::iterator it= phaseInstructions.begin(), end= phaseInstructions.end();
				it != end; ++it)
				instSet.insert(*it);
		}
		
		void dumpInformation() const {
			for (set<int>::iterator it = startSet.begin(); it != startSet.end(); it++) {
				outs() << *it << " ";
			}
			if(startSet.size() != 0) {
				outs() << "\n";
			}

			for (set<int>::iterator it = endSet.begin(); it != endSet.end(); it++) {
				outs() << *it << " ";
			}
			if(endSet.size() != 0) {
				outs() << "\n";
			}

			set<int> allLines;
			for (set<DebugInst>::iterator it = instSet.begin(); it != instSet.end(); it++) {
				allLines.insert(it->line);
			}

			for (set<int>::iterator it = allLines.begin(); it != allLines.end(); it++) {
				outs() << *it << " ";
			}

			if(allLines.size() != 0) {
				outs() << "\n";
			}
			outs() << "---\n";
		}
		

		bool operator<(Phase other) const {
			if(startSet != other.startSet) {
				return startSet < other.startSet;
			}
			if (endSet != other.endSet) {
				return endSet < other.endSet;
			}
			return instSet < other.instSet;
		}
};


/**
 * This class represents the notion of a race condition pair.
 */
class RaceCondition {
	public:
		RaceCondition(int i1, int i2) {
	        if (i1 < i2) {
	            this->i1 = i1;
	            this->i2 = i2;
	        } else {
	            this->i1 = i2;
	            this->i2 = i1;
	        }
		}

		void dumpInformation() const {
			outs() << i1 << " " << i2 << "\n";
		}

		bool operator<(RaceCondition other) const {
			if (i1 == other.i2 && i2 == other.i1) {
				return false;
			}

			if (i1 < other.i1) {
				return true;
			} else if (i1 > other.i1){
				return false;
			} else {
				if (i2 < other.i2) {
					return true;
				} else {
					return false;
				}
			}
		}

	private:
		int i1;
		int i2;
	
};
class SVE{
	public:
		int sveBit;

};

class Happens : public ModulePass {
	public:
		static char ID;
		set<Phase> nonSVE_MHP_set; 			// Set of phases obtained in path-insensitive MHP analysis
		set<Phase> SVE_MHP_set;				// Set of phases obtained in path-sensitive MHP analysis
		set<RaceCondition> nonSVE_RC_set;	// Set of race conditions obtained using path-insensitive MHP analysis
		set<RaceCondition> SVE_RC_set;		// Set of race conditions obtained using path-sensitive MHP analysis
		map<string, bool> globals;
		map<string, bool> paramsList;
		map<string, Instruction*> varsList;

		map<string, vector<int> > readVarMap;
		map<string, vector<int> > writtenVarMap;
		map<string, bool> blockType;
		map<string, map<string, set<pair<Instruction*, bool> > > > blockInfo;
		string currBlockName;
		Happens() : ModulePass(ID) {
		}

		bool runOnModule(Module &M);
		void non_SVE_MHPAnalysis(Function&);
		void SVE_MHP_Analysis(Function &);
		set<int> blockTraversal(vector<DebugInst>&, vector<DebugInst> &, set<int>);
		void blockTraversalSVE(vector<DebugInst>&, set<int>&, vector<BasicBlock*>&, set<int>&, 
				vector<DebugInst>&, set<DebugInst>&);
		void expressionDetermination(BasicBlock*, vector<BasicBlock*>&);
		bool checkForTermination(set<int>, set<Phase>);
		void copyEndToStart(vector<DebugInst> &, vector<DebugInst> &);
		void getGlobals(Module &);
		void getFunctionParamsAndVars(Function &F);
		bool checkIfArg(string);
		void printInfo(set<int> startBarriers, set<int> endBarriers, set<DebugInst> phases);
		void checkAndInsertLastPhase();
		bool checkIfBlockVisited(BasicBlock*, vector<BasicBlock*>);
		void dumpInformation();
		DebugInst getNextInstruction(Instruction*);
		void raceConditionDetection(set<Phase>&, set<RaceCondition>&);
		void getReadOrWrittenVars(DebugInst);
		DebugInst createDebugInst(Value*, DebugInst);
		void generateRaceConditions(set<RaceCondition>&);
		map<string, set<pair<Instruction*, bool> > > fillSVEInfo();
		void calculatePathInfo(vector<BasicBlock*>);
		map<string, set<pair<int, bool> > > cloneVarInfoMap();
		void handleStore(Instruction*, map<string, set<pair<Instruction*, bool > > > &);
		void handleBinaryOps(Instruction*, map<string, set<pair<Instruction*, bool > > > &);
		bool checkIfSVE(set<pair<Instruction*, bool> >);
		bool getVarType(Value*, map<string, set<pair<Instruction*, bool> > >&);
		void mergeBlockInfo(map<string, set<pair<Instruction*, bool> > >);
		void handleBranch(Instruction* inst, map<string, set<pair<Instruction*, bool> > >& varInfo);
		void printSVEInfo();
		void printInitialSVEInfo(map<string, set<pair<Instruction*, bool> > >);
		bool checkIfVisited(vector<BasicBlock*> blockStack, BasicBlock *BB);
		vector<DebugInst> copyDebugInstVector(vector<DebugInst>&);
		set<DebugInst> copyDebugInstSet(set<DebugInst>&);
		set<int> copyIntSet(set<int>&);
		vector<BasicBlock*> copyBlockVector(vector<BasicBlock*>& vect);
		void checkForGlobalWrite(Function&);
};

void Happens::dumpInformation() {
	//----DON'T CHANGE THIS CODE BELOW----//
	outs() << "nonSVE_MHP\n";
	for(set<Phase>::iterator it = nonSVE_MHP_set.begin(); it != nonSVE_MHP_set.end(); it++) {
		it->dumpInformation();
	}

	outs() << "SVE_MHP\n";
	for(set<Phase>::iterator it = SVE_MHP_set.begin(); it != SVE_MHP_set.end(); it++) {
		it->dumpInformation();
	}

	outs() << "nonSVE_RC\n";
	for(set<RaceCondition>::iterator it = nonSVE_RC_set.begin(); it != nonSVE_RC_set.end(); it++) {
		it->dumpInformation();
	}

	outs() << "SVE_RC\n";
	for(set<RaceCondition>::iterator it = SVE_RC_set.begin(); it != SVE_RC_set.end(); it++) {
		it->dumpInformation();
	}
	//----DON'T CHANGE THIS CODE ABOVE----//

}

bool Happens::checkIfBlockVisited(BasicBlock *BB, vector<BasicBlock*> visited){
	for(vector<BasicBlock*>::iterator iter= visited.begin(), end= visited.end(); iter != end; ++iter){
		if(*iter == BB)
			return true;
	}
	return false;
}

void Happens::printInitialSVEInfo(map<string, set<pair<Instruction*, bool> > > varMap){
	for(map<string, set<pair<Instruction*, bool> > >::iterator iter= varMap.begin(), end= 
			varMap.end(); iter != end; ++iter){
		cout << iter->first <<": ";
		set<pair<Instruction*, bool> > tempSet= iter->second;
		for(set<pair<Instruction*, bool> >::iterator iter2= tempSet.begin(), end2= tempSet.end();
					iter2 != end2; ++iter2){
	
			DebugInst debug= DebugInst(iter2->first);
			cout << "(" << debug.line <<","<< iter2->second <<") ";
		}
		cout <<endl;
	}
	for(map<string, bool>::iterator iter= paramsList.begin(), end= paramsList.end();
				iter!= end; ++iter)
		cout << iter->first <<": " << iter->second <<endl;
	for(map<string, bool>::iterator iter= globals.begin(), end= globals.end(); iter != end;	
				++iter)
		cout << iter->first <<": " << iter->second <<endl;
	cout <<endl;
}

void Happens::printSVEInfo(){
	for(map<string, map<string, set<pair<Instruction*, bool> > > >::iterator iter= blockInfo.begin(), 
			end= blockInfo.end(); iter != end; ++iter){
		cout << "blockName: "<< iter->first <<endl;
		map<string, set<pair<Instruction*, bool> > > varInfo= iter->second;
		printInitialSVEInfo(varInfo);
	}
}

DebugInst Happens::getNextInstruction(Instruction* inst){
	BasicBlock::iterator I(inst);
	return DebugInst(++I);
}

set<int> Happens::blockTraversal(vector<DebugInst>& startInstSet, vector<DebugInst> &endInstSet, 
	set<int> startBarriers){
	
	set<int> endBarriers;
	set<DebugInst> phaseInformation;
	string opCode, funcName;
	Instruction* inst;
	vector<BasicBlock*> visitedBlocks;

	while(!startInstSet.empty()){
		DebugInst debug= startInstSet[0];

		while(debug.instruction != NULL){
			inst= debug.instruction;
			opCode= inst->getOpcodeName();
			if((opCode=="store" || opCode =="icmp") && debug.line != 0){
				phaseInformation.insert(debug);
			
			}else if(opCode=="call"){
				CallInst* callInst= dyn_cast<CallInst>(inst);
				funcName= callInst->getCalledFunction()->getName();
				if(funcName=="barrier"){
					endBarriers.insert(debug.line);
					debug= getNextInstruction(debug.instruction);
					endInstSet.push_back(debug);
					break;
				}
			}else if(opCode=="ret"){
				endBarriers.insert(-1);
				break;

			}else if(opCode=="br"){
				BasicBlock *BB= inst->getParent();
				for(succ_iterator iter= succ_begin(BB), end= succ_end(BB); iter != end; ++iter){
					if(!checkIfBlockVisited(*iter, visitedBlocks)){
						visitedBlocks.push_back(*iter);
						Instruction* I= (*iter)->getInstList().begin();
						startInstSet.push_back(DebugInst(I));
					}
				}
				break;
			}
			debug= getNextInstruction(debug.instruction);
		}
		startInstSet.erase(startInstSet.begin(), startInstSet.begin()+1);
	}
	//printInfo(startBarriers, endBarriers, phaseInformation);
	for(set<int>::iterator iter= startBarriers.begin(), end= startBarriers.end(); iter != end; ++iter){
		if(*iter == -1){
			endBarriers.insert(-1);
			break;
		}
	}
	Phase phase= Phase(&startBarriers);
	phase.setEndSetAndInstSet(endBarriers, phaseInformation);
	nonSVE_MHP_set.insert(phase);
	return endBarriers;
}

void Happens::copyEndToStart(vector<DebugInst> &startSet, vector<DebugInst>& endSet){
	for(vector<DebugInst>::iterator iter= endSet.begin(), end= endSet.end(); iter != end; ++iter)
		startSet.push_back(*iter);
}

bool Happens::checkForTermination(set<int> endSet_, set<Phase> MHP_set){
	set<int> startSet_;
	for(set<Phase>::iterator iter=MHP_set.begin(), end= MHP_set.end(); 
			iter != end; ++iter){
		startSet_ = (*iter).startSet;
		if(startSet_ == endSet_)
			return true;
	}
	return false;
}

DebugInst Happens::createDebugInst(Value* val, DebugInst debug){
	Instruction* I= dyn_cast<Instruction>(val);
	DebugInst debug_= DebugInst(I);
	debug_.line= debug.line;
	return debug_;
}

void Happens::generateRaceConditions(set<RaceCondition>& RC_set){
	for(map<string, vector<int> >::iterator write_iter= writtenVarMap.begin(), 
			write_end= writtenVarMap.end(); write_iter != write_end; ++write_iter){
		
		string writtenGlobal= write_iter->first;
		vector<int> writtenLines= write_iter->second;
		size_t len= writtenLines.size();
		for(size_t i=0; i< len; i++){
			for(size_t j=i+1; j< len; j++)
				RC_set.insert(RaceCondition(writtenLines[i], writtenLines[j]));
		}

		if(readVarMap.find(writtenGlobal) != readVarMap.end()){
			vector<int> readLines= readVarMap[writtenGlobal];
			for(vector<int>::iterator vect1_iter= writtenLines.begin(), 
					vect1_end=writtenLines.end(); vect1_iter != vect1_end; ++vect1_iter)
				for(vector<int>::iterator vect2_iter= readLines.begin(), 
						vect2_end= readLines.end(); vect2_iter != vect2_end; ++vect2_iter)
					if(*vect1_iter != *vect2_iter)
						RC_set.insert(RaceCondition(*vect1_iter, *vect2_iter));		
		}	
	}
}

void Happens::getReadOrWrittenVars(DebugInst debug){
	Instruction *inst= debug.instruction;
	vector<Value*> operands;
	User *user= dyn_cast<User>(inst);

	unsigned int nOperands= user->getNumOperands();
	for(unsigned int i=0; i< nOperands; i++)
		operands.push_back(user->getOperand(i));

	if(StoreInst::classof(inst)){
		string destName= operands[1]->getName();
		if(globals.find(destName) != globals.end()){
			if(writtenVarMap.find(destName) == writtenVarMap.end())
				writtenVarMap[destName]= vector<int>{};
			writtenVarMap[destName].push_back(debug.line);
		}
		if(!ConstantInt::classof(operands[0]))
			getReadOrWrittenVars(createDebugInst(operands[0], debug));

	}else{
		for(unsigned int i=0; i< nOperands; ++i){
			if(!ConstantInt::classof(operands[i])){
				if(GlobalVariable::classof(operands[i])){
					string globalName= operands[i]->getName();
					if(readVarMap.find(globalName) == readVarMap.end())
						readVarMap[globalName] = vector<int>{};
					readVarMap[globalName].push_back(debug.line);
				}else
					getReadOrWrittenVars(createDebugInst(operands[i], debug));
			}
		}
	}
}

void Happens::raceConditionDetection(set<Phase>& MHP_set, set<RaceCondition>& RC_set){
	for(set<Phase>::iterator iter= MHP_set.begin(), end= MHP_set.end(); 
			iter != end; ++iter){
		set<DebugInst> phaseInsts= (*iter).instSet;
		for(set<DebugInst>::iterator info_iter= phaseInsts.begin(), info_end = phaseInsts.end(); 
				info_iter != info_end; ++info_iter){
			DebugInst debug= *info_iter;
			getReadOrWrittenVars(debug);		
		}		
		generateRaceConditions(RC_set);
		writtenVarMap.clear();
		readVarMap.clear();
	}
}	

void Happens::non_SVE_MHPAnalysis(Function &F){
	getFunctionParamsAndVars(F);
	BasicBlock & entryBlock= F.getEntryBlock();
	Instruction *I= entryBlock.getInstList().begin();
	DebugInst debugInst= DebugInst(I);
	vector<DebugInst> startInstSet{debugInst};
	vector<DebugInst> endInstSet;
	set<int> startBarriers{0};
	while(true){
		startBarriers=blockTraversal(startInstSet,endInstSet,startBarriers);
		startInstSet.clear();
		copyEndToStart(startInstSet, endInstSet);
		endInstSet.clear();
		if(checkForTermination(startBarriers, nonSVE_MHP_set))
			break;
	}
	raceConditionDetection(nonSVE_MHP_set, nonSVE_RC_set);
}

map<string, set<pair<Instruction*, bool> > >  Happens::fillSVEInfo(){
	map<string, set<pair<Instruction*, bool> > > varInfoMap;

	for(map<string, Instruction*>::iterator iter= varsList.begin(), end= varsList.end(); 
				iter != end; ++iter)
		varInfoMap[iter->first]= set<pair<Instruction*, bool> >{make_pair(iter->second, true)};

	return varInfoMap;
}

bool Happens::checkIfSVE(set<pair<Instruction*,bool> > infoSet){
	bool flag= true, retVal=true;
	for(set<pair<Instruction*, bool> >::iterator iter= infoSet.begin(), end= infoSet.end();
			iter!= end; ++iter){
		if(!iter->second){
			retVal= flag= false;
			break;
		}
	}
	if(flag){
		if(infoSet.size()==1)
			return true;
		for(set<pair<Instruction*, bool> >::iterator iter= infoSet.begin(), end= infoSet.end();
					iter != end; ++iter){
			string parentBlockName= iter->first->getParent()->getName();
			if(!blockType[parentBlockName]){
				retVal= false;
				break;
			}
		}
	}
	return retVal;
}

void Happens::mergeBlockInfo(map<string, set<pair<Instruction*, bool> > > varMap){
	map<string, set<pair<Instruction*, bool> > >& currBlockInfo= blockInfo[currBlockName];
	string varName;
	set<pair<Instruction*, bool> > localSet;
	for(map<string, set<pair<Instruction*, bool> > >::iterator iter= varMap.begin(),
			end= varMap.end(); iter != end; ++iter){
		varName= iter->first;
		localSet= iter->second;
		if(currBlockInfo.find(varName) == currBlockInfo.end())
			currBlockInfo[varName]= set<pair<Instruction*, bool> >{};
		for(set<pair<Instruction*, bool> >::iterator iter_set= localSet.begin(),end_set= localSet.end();
				iter_set != end_set; ++iter_set)
			currBlockInfo[varName].insert(*iter_set);
	}
}

void Happens::handleStore(Instruction *inst, map<string, set<pair<Instruction*, bool> > >& varInfo){
	User *user= dyn_cast<User>(inst);
	Value *val0= user->getOperand(0);
	Value *val1= user->getOperand(1);
	string destName= val1->getName();
	string blockName= inst->getParent()->getName();
	pair<Instruction*, bool> localPair;
	
	if(globals.find(destName) != globals.end())
		globals[destName]= false;

	else if(paramsList.find(destName) == paramsList.end()){
		if(ConstantInt::classof(val0)){
			localPair= make_pair(inst, true);

		}else{
			string srcName= val0->getName();
			if(srcName=="")
				srcName = dyn_cast<User>(val0)->getOperand(0)->getName();
			
			if(globals.find(srcName) != globals.end())
				localPair= make_pair(inst, globals[srcName]);
			else if(paramsList.find(srcName) != paramsList.end())
				localPair= make_pair(inst, paramsList[srcName]);
			else 
				localPair= make_pair(inst, checkIfSVE(varInfo[srcName]));
		}
		varInfo[destName] = set<pair<Instruction*, bool> >{localPair};
	}
}

bool Happens::getVarType(Value* val, map<string, set<pair<Instruction*, bool> > >& varInfo){
	bool retVal= true;
	if(ConstantInt::classof(val))
		retVal= true;
	else{
		string varName= val->getName();
		if(varName =="")
			varName= dyn_cast<User>(val)->getOperand(0)->getName();
		
		if(globals.find(varName) != globals.end())
			retVal= globals[varName];
		else if(paramsList.find(varName) != paramsList.end())
			retVal= false;
		else
			retVal= checkIfSVE(varInfo[varName]);
	}
	return retVal;
}

void Happens::handleBinaryOps(Instruction *inst, map<string, set<pair<Instruction*, bool > > > & varInfo){
	User *user= dyn_cast<User>(inst);
	Value *val0= user->getOperand(0);
	Value *val1= user->getOperand(1);
	string destName= inst->getName();
	string blockName= inst->getParent()->getName();

	varInfo[destName]= set<pair<Instruction*, bool> >{make_pair(inst, getVarType(val0,varInfo) && 
				getVarType(val1, varInfo))};
}

void Happens::handleBranch(Instruction* inst, map<string, set<pair<Instruction*, bool> > >& varInfo){
	BasicBlock* BB= inst->getParent();
	string blockName= BB->getName();
	User *user= dyn_cast<User>(inst);
	int nOpds= user->getNumOperands();
	Value* val0= user->getOperand(0);
	bool sveBit=true;
	if(nOpds==3){
		string condExpr= val0->getName();
		sveBit= checkIfSVE(varInfo[condExpr]);	
	}

	for(succ_iterator iter= succ_begin(BB), end= succ_end(BB); iter != end; ++iter){
		string blockName= iter->getName();
		if(blockType.find(blockName) == blockType.end())
			blockType[blockName]= true;
		blockType[blockName]= (sveBit && blockType[blockName]);		
	}
}

void Happens::calculatePathInfo(vector<BasicBlock*> blockStack){
	int len= blockStack.size();
	map<string, set<pair<Instruction*, bool> > > varMap= fillSVEInfo();
	// for(int i=0; i< len; i++)
	// 	outs() << blockStack[i]->getName() <<"  ";
	// cout <<endl;
	for(int i= len-1; i>=0; i--){
		BasicBlock* BB= blockStack[i];
		for(BasicBlock::iterator iter= BB->begin(), end= BB->end(); iter != end; ++iter){
			string opCode= iter->getOpcodeName();
			//cout << opCode <<endl;
			if(opCode=="add" || opCode=="sub" || opCode=="mul" || opCode=="icmp")
				handleBinaryOps(iter, varMap);
			else if(opCode=="store")
				handleStore(iter, varMap);
			else if(opCode=="br"){
				handleBranch(iter, varMap);
			}else if(opCode=="ret")
				blockType[BB->getName()]= false;
		}
	}
	mergeBlockInfo(varMap);
}

bool Happens::checkIfVisited(vector<BasicBlock*> blockStack, BasicBlock *BB){	
	for(vector<BasicBlock*>::iterator iter= blockStack.begin(), end= blockStack.end();
			iter != end; ++iter){
		if(*iter== BB)
			return true;
	}
	return false;	
}

vector<DebugInst> Happens::copyDebugInstVector(vector<DebugInst>& vect){
	vector<DebugInst> temp;
	for(vector<DebugInst>::iterator iter= vect.begin(), end =vect.end(); 
			iter != end; ++iter)
		temp.push_back(*iter);
	return temp;
}

set<DebugInst> Happens::copyDebugInstSet(set<DebugInst>& set_){
	set<DebugInst> temp;
	for(set<DebugInst>::iterator iter= set_.begin(), end= set_.end(); 
			iter != end; ++iter)
		temp.insert(*iter);
	return temp;
}

set<int> Happens::copyIntSet(set<int>& set_){
	set<int> temp;
	for(set<int>::iterator iter= set_.begin(), end= set_.end(); iter != end; ++iter)
		temp.insert(*iter);
	return temp;
}

vector<BasicBlock*> Happens::copyBlockVector(vector<BasicBlock*>& vect){
	vector<BasicBlock*> temp;
	for(vector<BasicBlock*>::iterator iter= vect.begin(), end= vect.end(); iter != end; ++iter)
		temp.push_back(*iter);
	return temp;
}

void Happens::expressionDetermination(BasicBlock *BB, vector<BasicBlock*>& blockStack){
	blockStack.push_back(BB);
	for(pred_iterator iter= pred_begin(BB), end= pred_end(BB); iter != end; ++iter){
		if(!checkIfVisited(blockStack, *iter))
			expressionDetermination(*iter, blockStack);
	}
	if(BB->getName() =="entry")
		calculatePathInfo(blockStack);	
	blockStack.pop_back();
}

void Happens::blockTraversalSVE(vector<DebugInst>& startInstSet, set<int>& startBarriers, 
	vector<BasicBlock*>& visitedBlocks, set<int>& endBarriers, vector<DebugInst>& endInstSet, 
	set<DebugInst>& phaseInformation){

	string opCode, funcName;
	Instruction* inst;
	// cerr <<"endBarriers Starts: ";
	// for(set<int>::iterator iter=startBarriers.begin(), end= startBarriers.end();
	// 			iter != end; ++iter)
	// 	cerr << *iter <<"  ";
	// for(vector<DebugInst>::iterator iter= startInstSet.begin(), end= startInstSet.end();
	// 			iter != end; ++iter)
	// 	outs() << *(iter->instruction) <<" ";

	// cerr <<endl;

	while(!startInstSet.empty()){
		DebugInst debug= startInstSet[0];

		while(debug.instruction != NULL){
			inst= debug.instruction;
			opCode= inst->getOpcodeName();
			if((opCode=="store" || opCode =="icmp") && debug.line != 0){
				phaseInformation.insert(debug);
			
			}else if(opCode=="call"){
				CallInst* callInst= dyn_cast<CallInst>(inst);
				funcName= callInst->getCalledFunction()->getName();
				if(funcName=="barrier"){
					endBarriers.insert(debug.line);
					debug= getNextInstruction(debug.instruction);
					endInstSet.push_back(debug);
					break;
				}
			}else if(opCode=="ret"){
				endBarriers.insert(-1);
				//cerr <<"Came here\n";
				break;

			}else if(opCode=="br"){
				BasicBlock *BB= inst->getParent();
				User* user= dyn_cast<User>(inst);
				int numOpd= user->getNumOperands();
				bool sveBit= false;
				if(numOpd==3){
					Value* val0= user->getOperand(0);
					sveBit= checkIfSVE(blockInfo[BB->getName()][val0->getName()]);
					outs() << val0->getName() <<"  " << sveBit <<"\n";
				}
				if(sveBit){
					for(succ_iterator iter= succ_begin(BB), end= succ_end(BB); iter != end; ++iter){
						if(!checkIfBlockVisited(*iter, visitedBlocks)){
							//outs() << iter->getName() <<"\n";			
							vector<DebugInst> startInstSet_= copyDebugInstVector(startInstSet);
							set<int> startBarriers_ = copyIntSet(startBarriers);
							vector<BasicBlock*> visitedBlocks_ = copyBlockVector(visitedBlocks);
							set<int> endBarriers_ = copyIntSet(endBarriers);
							vector<DebugInst> endInstSet_= copyDebugInstVector(endInstSet);
							set<DebugInst> phaseInformation_ = copyDebugInstSet(phaseInformation);

							visitedBlocks_.push_back(*iter);
							Instruction* I= (*iter)->getInstList().begin();
							startInstSet_.push_back(DebugInst(I));
							startInstSet_.erase(startInstSet_.begin(), startInstSet_.begin()+1);
			
							blockTraversalSVE(startInstSet_, startBarriers_, visitedBlocks_,
										endBarriers_, endInstSet_, phaseInformation_);
						}
					}	
					return;
				}else{
					for(succ_iterator iter= succ_begin(BB), end= succ_end(BB); iter != end; ++iter){
						if(!checkIfBlockVisited(*iter, visitedBlocks)){
							visitedBlocks.push_back(*iter);
							Instruction* I= (*iter)->getInstList().begin();
							startInstSet.push_back(DebugInst(I));
						}
					}
				}
				break;
			}
			debug= getNextInstruction(debug.instruction);
		}
		startInstSet.erase(startInstSet.begin(), startInstSet.begin()+1);
	}
	//printInfo(startBarriers, endBarriers, phaseInformation);
	for(set<int>::iterator iter= startBarriers.begin(), end= startBarriers.end(); iter != end; ++iter){
		if(*iter == -1){
			endBarriers.insert(-1);
			break;
		}
	}
	Phase phase= Phase(&startBarriers);
	phase.setEndSetAndInstSet(endBarriers, phaseInformation);
	SVE_MHP_set.insert(phase);

	if(!checkForTermination(endBarriers, SVE_MHP_set)){
		set<int> endBarriers_;
		vector<DebugInst> endInstSet_;
		set<DebugInst> phaseInformation_;
		blockTraversalSVE(endInstSet, endBarriers, visitedBlocks, 
				endBarriers_, endInstSet_, phaseInformation_);		
	}
}

void Happens::SVE_MHP_Analysis(Function &F){
	//F.viewCFG();
	BasicBlock &entryBlock= F.getEntryBlock();
	string entryName= entryBlock.getName();
	blockType[entryName]=true;
	readVarMap.clear();
	writtenVarMap.clear();

	for(Function::iterator iter= F.begin(), end= F.end(); iter != end; ++iter){
		currBlockName= iter->getName();
		vector<BasicBlock*> blockStack;
		blockInfo[currBlockName]= map<string, set<pair<Instruction*, bool> > >{};
		expressionDetermination(iter,blockStack);
		// cout <<currBlockName <<":\n";
		// printInitialSVEInfo(blockInfo[currBlockName]);	
	}
	// for(map<string, bool>::iterator iter= blockType.begin(), end= blockType.end();
	// 			iter != end; ++iter){
	// 	cout << iter->first <<" " << iter->second <<endl;
	// }
	Instruction *I= entryBlock.getInstList().begin();
	DebugInst debugInst= DebugInst(I);
	vector<DebugInst> startInstSet{debugInst};
	set<int> startBarriers{0};
	vector<BasicBlock*> visitedBlocks{&entryBlock};
	set<int> endBarriers;
	vector<DebugInst> endInstSet;
	set<DebugInst> phaseInformation;
	blockTraversalSVE(startInstSet,startBarriers, visitedBlocks, endBarriers, endInstSet, 
		phaseInformation);
	raceConditionDetection(SVE_MHP_set, SVE_RC_set);
}

bool Happens::checkIfArg(string var){
	for(map<string, bool>::iterator begin= paramsList.begin(), end= paramsList.end(); 
		begin != end; ++begin){
		if(begin->first == var)
			return true;
	}
	return false;
}

void Happens::getFunctionParamsAndVars(Function &F){
	int numArgs=0;
	for(Function::arg_iterator iter= F.arg_begin(), iter_end= F.arg_end(); iter!= iter_end; ++iter){
		numArgs++;
		paramsList[iter->getName()]= false;
	}

	BasicBlock &BB= F.getEntryBlock();
	
	for(BasicBlock::iterator iter= BB.begin(),end = BB.end(); iter!= end; ++iter){
		string opCode= iter->getOpcodeName();
		if(opCode=="alloca"){
			if(numArgs>0)
				paramsList[iter->getName()]= false;
			else 
				varsList[iter->getName()] = iter;
			numArgs--;
		}
	}
}

void Happens::getGlobals(Module &M){
	for(Module::global_iterator I= M.global_begin(), E= M.global_end(); I != E; ++I)
		globals[I->getName()]= true;
}

void Happens::checkForGlobalWrite(Function &F){
	for(Function::iterator BB_iter= F.begin(),BB_end= F.end();
				BB_iter != BB_end; ++BB_iter){
		for(BasicBlock::iterator inst_iter= BB_iter->begin(), inst_end= BB_iter->end();
					inst_iter!= inst_end; ++inst_iter){
			string opCode= inst_iter->getOpcodeName();
			if(opCode=="store"){
				User* user= dyn_cast<User>(inst_iter);
				Value* val1= user->getOperand(1);
				string destName= val1->getName();
				if(globals.find(destName) != globals.end())
					globals[destName]= false;
			}
		}
	}
}

bool Happens::runOnModule(Module &M) {
	getGlobals(M);
	for(Module::iterator F= M.begin(), E= M.end(); F!= E; ++F){
		if(F->getName()=="parRegion"){
			non_SVE_MHPAnalysis(*F);
			checkForGlobalWrite(*F);
			SVE_MHP_Analysis(*F);
		}
	}
	dumpInformation();
	return false;
}

// Register the pass.
char Happens::ID = 0;
static RegisterPass<Happens> X("happens", "MHP Analysis and Race Condition Detection");
