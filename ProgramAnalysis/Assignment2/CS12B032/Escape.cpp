#include "llvm/Pass.h"
#include "llvm/IR/Module.h"
#include "llvm/IR/Instructions.h"
#include "llvm/Support/raw_ostream.h"
#include <map>
#include <vector>
#include <iostream>
#include <string>
#include <utility>
#include <set>

using namespace std;
using namespace llvm;

class Escape : public ModulePass {
	public:
		map<string, vector<vector<string> > > points_to_info;
		map<string, int> escaped_locations;
		vector<string> malloc_locations;
		map<string, vector<map<string, int> > > myMap;
	
		static char ID;
		Escape() : ModulePass(ID) {
		}

		bool runOnModule(Module &M);
		void createInitialConstraints(Function &F);
		void printVars();
		bool checkIfMallocd(string str);
		void getAllEscapingInfo();
		void printMyMap();
		bool checkIfVisited(string str, int, vector<pair<string, int> >);		
		void printSet(set<string>);
};

bool Escape::checkIfVisited(string str, int num, vector<pair<string, int> >visited){
	for(size_t i=0;i< visited.size();i++){
		if(visited[i].first == str && visited[i].second == num)
			return true;
	}
	return false;
}

void Escape::getAllEscapingInfo(){	
	pair<string, int> temp_pair;
	vector<pair<string, int> > visited;
	vector<pair<string, int> > queue;
	map<string, int> temp_map;
	
	for(map<string, vector<map<string, int> > >::iterator iter= myMap.begin(); iter != myMap.end(); iter++){
		vector<map<string, int> > vect= iter->second;	
		string ptr= iter->first;
		for(size_t i=0; i< vect.size(); i++){
			temp_map= vect[i];
			if(temp_map.size() > 0){
				temp_pair= make_pair(iter->first, i);
				visited.push_back(temp_pair);
				queue.push_back(temp_pair);
				while(queue.size() > 0){
					temp_map= myMap[queue[0].first][queue[0].second];
					for(map<string, int>::iterator iter2=temp_map.begin(); iter2!=temp_map.end(); iter2++){
						string str1= iter2->first;
						int t1= iter2->second;
						if(!checkIfVisited(str1, t1, visited)){
							temp_pair=make_pair(str1, t1);
							visited.push_back(temp_pair);
							queue.push_back(temp_pair);
						    for(size_t k=t1, l=i; k< points_to_info[str1].size(); k++,l++){
						    	for(size_t j=0; j< points_to_info[str1][k].size(); j++){
									points_to_info[iter->first][l].push_back(points_to_info[str1][k][j]);
								}	
						    }
						}						
					}
					queue.erase(queue.begin()+0);
				}
			}	
		}
	}
}

bool Escape::checkIfMallocd(string str){
	size_t i;
	for(i=0;i< malloc_locations.size(); i++){
		if(malloc_locations[i] == str)
			return true;
	}
	return false;
}

bool Escape::runOnModule(Module &M) {
	//First pass to collect the information about the variables that have been malloced
	for (Module::iterator F = M.begin(), E = M.end(); F != E; ++F) {
		if ((*F).getName() == "main") {
			createInitialConstraints(*F);
		}
	}
    //printMyMap();
    getAllEscapingInfo();
    printVars();
	return false;
}

void Escape::printMyMap(){
	for(map<string, vector<map<string, int> > >::iterator iter = myMap.begin(); iter != myMap.end(); iter++){
		cout << iter->first <<":" << endl;
		vector<map<string, int> > vect= iter->second;
		for(size_t i=0; i< vect.size(); i++){
			if(vect[i].size()>0 ){
				cout << i <<":";
				map<string, int> map_ = vect[i];
				for(map<string, int>::iterator iter_= map_.begin(); iter_!=map_.end(); iter_++){
					cout << iter_->first <<"-" << iter_->second <<" " ;
				}
				cout << endl;
			}
		}
		cout <<endl;
	}
}

void Escape::printSet(set<string> set_){
		for(set<string>::iterator iter= set_.begin(); iter!=set_.end(); iter++){
		 	cout << *iter <<" ";
		}	
}
void Escape::printVars(){
	// printing the escaped locations
	unsigned int i;
	set<string> set_;
	for(map<string, int>::iterator iter= escaped_locations.begin(); iter!=escaped_locations.end(); iter++){
		string ptr= iter->first;
		for(i=iter->second; i< points_to_info[ptr].size(); i++){
			vector<string> vect= points_to_info[ptr][i] ;
			for(size_t j=0; j< vect.size(); j++){
				if(checkIfMallocd(vect[j]))
					set_.insert(vect[j]);
			}
		}
	}
	printSet(set_);
	cout <<endl;
	for(map<string, vector<vector<string> > >::iterator iter= points_to_info.begin(); iter!=points_to_info.end(); iter++){
		cout << iter->first << " ";
		set_.clear();
		for(i=0;i< iter->second[0].size(); i++)
			set_.insert(iter->second[0][i]); 
		printSet(set_);
		cout <<endl; 
	}
}

void Escape::createInitialConstraints(Function &F) {
	unsigned int i;
	bool malloc_flag=false;
	vector<string> load_ptrs;
	vector<int> load_counters;
	
	for (Function::iterator BB = F.begin(), E = F.end(); BB != E; ++BB) {
		for (BasicBlock::iterator I = BB->begin(), E = BB->end(); I != E; ++I) {
			switch (I->getOpcode()) {
								
				case Instruction::Alloca:
					{
						if(I->getName() != "retval"){
							points_to_info[I->getName()] = vector<vector<string> >();
							myMap[I->getName()] = vector<map<string, int> >();
							for(i=0;i<30;i++){
								points_to_info[I->getName()].push_back(vector<string>());
								myMap[I->getName()].push_back(map<string, int> ());
							}
						}
						break;
					}			
							
				case Instruction::Call:
					{
						CallInst *CI = dyn_cast<CallInst>(I);
						string function_name= CI->getCalledFunction()->getName();
						if(function_name=="malloc"){
							
							load_ptrs.push_back(CI->getName());
							malloc_flag= true;
							load_counters.push_back(0);
							malloc_locations.push_back(CI->getName());
						}else{
							for(i=0;i< load_ptrs.size(); i++){
								if(escaped_locations.find(load_ptrs[i]) == escaped_locations.end()) 
									escaped_locations[load_ptrs[i]] = 100; 
								if(escaped_locations[load_ptrs[i]] > load_counters[i])
									escaped_locations[load_ptrs[i]] = load_counters[i];
							}
							load_ptrs.clear();
							load_counters.clear();	
						}
						break;
					}
					
				case Instruction::Store:
					{
						StoreInst *SI = dyn_cast<StoreInst>(I);
						//Value *v = SI->getValueOperand();
						Value *ptr = SI->getPointerOperand();
						
						if(malloc_flag){
							if(ptr->getName()!="")
								load_ptrs.push_back(ptr->getName()); 
							points_to_info[load_ptrs[1]][load_counters[0]].push_back(load_ptrs[0]);
							malloc_flag=false;
							load_ptrs.clear();
							load_counters.clear();
							
						}else if(ptr->getName()!="retval"){
							if(load_ptrs.size()==1){
								load_ptrs.push_back(ptr->getName());
								load_counters.push_back(-1);
							}
							int counter1= load_counters[1]+1;
							string load_str= load_ptrs[1];
							//cout << myMap["i"].size() <<endl;
							myMap[load_str][counter1][load_ptrs[0]]= load_counters[0];
							//cout << load_ptrs[0] << "  " << load_counters[0] <<endl;
							//cout << load_ptrs[1] << "  " << load_counters[1] <<endl;
							vector<string> ld_pts1= points_to_info[load_ptrs[0]][load_counters[0]];
							for(i=0;i < ld_pts1.size(); i++)
								points_to_info[load_str][counter1].push_back(ld_pts1[i]);
							
							load_ptrs.clear();
							load_counters.clear();
						}

						break;
					}
					
				case Instruction::Load:
					{
						LoadInst *LI = dyn_cast<LoadInst>(I);
						Value *ptr = LI->getPointerOperand();
						string var= ptr->getName();
						int size= load_ptrs.size();
						
						if(malloc_flag){
							load_counters[0]+=1;
							if(var!="")
								load_ptrs.push_back(var);
						
						}else if(var!=""){
								load_ptrs.push_back(var);
								load_counters.push_back(0);
						
						}else load_counters[size-1]+=1;
					        break;
					}
			}

		}
	}
}


// Register the pass.
char Escape::ID = 0;
static RegisterPass<Escape> X("escape", "Escape Analysis Pass");
