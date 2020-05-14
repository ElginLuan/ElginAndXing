// 10720101 阮彥吉吉 10720137 陳微心 請助教下載github版本 
#include <iostream>
#include <fstream>
#include <string>
#include <cstdlib>
#include <iomanip>
#include <cstring>
#include <math.h>

using namespace std;

#define COLUMNS 6
#define MAX_LEN 10
#define BIG_INT 255

typedef struct sT{
	char sid[MAX_LEN];
	char sname[MAX_LEN];
	unsigned char score[COLUMNS];
	float mean;
} studentType;

typedef struct r{
	char sid[MAX_LEN];
	char sname[MAX_LEN];
	float mean;
	int hvalue;
	bool isOccupied;
}record;

int Text2Binary(string fileName);
void readBinary(string fileName);
void linearProbing(string fileName, string fileNumber) ;//任務一 
void quadraticProbing(string fileName, string fileNumber) ;//任務二 
int hash( char sid[MAX_LEN], int stNo );
int get_first_prime( int n ); 
float searchNotExist( record recordArray[], int size );//任務一 
float searchExist( record recordArray[], int size );//任務一 
void writeFile(string fileNumber, record recordArray[], int size ); //任務一 
void writeQuadraticFile(string fileNumber, record recordArray[], int size );//任務二 
float searchQuadraticNotExist( record recordArray[], int size );//任務二 
float searchQuadraticExist( record recordArray[], int size ); //任務二 
void linearSearch(string fileName, char sidd[MAX_LEN] );//任務三 
void quadraticSearch(string fileName, char sidd[MAX_LEN]); //任務三 

//--------------------------------------------------

int main() {
	int command ;
    bool command1First = false;
	string fileName; 
	while(true){
		
		cout << endl << "**           Hash Table                 **" ;
    	cout << endl << "* 0.  QUIT                               *" ;
    	cout << endl << "* 1.  Linear probing                     *" ;
    	cout << endl << "* 2.  Quadratic probing                  *" ;
    	cout << endl << "* 3.  Search                             *" ;
    	cout << endl << "******************************************" ;
    	cout << endl << "Input a choice(0, 1, 2, 3 ):" ;
   		cin >> command ;
		
   		if( command == 0 )
   			break ;    			
   		else if ( command == 1 ) {
   			command1First = true;
        	cout<<"Input a file number:";
        	cin >> fileName;
			Text2Binary(fileName);
			linearProbing("input"+ fileName +".bin",fileName);
	    } // else if	    
		else if (command == 2 ) {
			if( command1First )
				quadraticProbing("input"+ fileName +".bin",fileName);			
			else
			    cout << "### Command 1 first. ###";
	    } // else if	
		else if ( command == 3 ) {
        	string fileName;
        	cout<<"Input a file number:";
        	cin >> fileName;
			Text2Binary(fileName);
			char sidd[MAX_LEN];
			
			cout << endl << "Input a student number:" ;
			cin >> sidd;
			cout << endl << "Input search type linear or quadratic 'l' or 'q': ";
			char c;
			cin >> c;
			if( c == 'l' || c == 'L'){ 
			    while(true) {
					linearSearch("input"+ fileName +".bin", sidd );
					cout << endl << "Input a student number:" ;
					cin >> sidd;
				} // while
			} // if 
			else if( c == 'q' || c == 'Q' )
			    while(true) {
					quadraticSearch("input"+ fileName +".bin", sidd);
					cout << endl << "Input a student number:" ;
					cin >> sidd;
				} // while
			else
				cout<<"No such search function";
		} // else if
				
		    
	    else
	        cout<<"command does not exist";
	        
	} // while
	return 0;
} // end main

void linearProbing(string fileName, string fileNumber) {
	fstream binFile ;
	studentType oneSt ;
	int stNo = 0 ;
	binFile.open(fileName.c_str(),fstream::in | fstream::binary) ;
	if(binFile.is_open()) {
		binFile.seekg(0,binFile.end) ;
		stNo = binFile.tellg()/sizeof(oneSt) ;
		binFile.seekg(0,binFile.beg) ;
		int newArraySize = get_first_prime(int(1.2*stNo));
		record recordArray[newArraySize]; // 宣告hashTable 
		for(int i = 0 ; i <newArraySize ; i++ ) { // 初始化hashTable
			recordArray[i].isOccupied = false;
		} // for
		for(int i = 0 ; i < stNo ; i++ ) {
			binFile.read((char*)&oneSt,sizeof(oneSt)) ;
			int tempHvalue = hash( oneSt.sid, newArraySize );
			int finalHvalue = tempHvalue;
			bool run = true;
			
			while( run ) { // 找空的位置放資訊 
				if( recordArray[tempHvalue].isOccupied == false ) {
					strcpy( recordArray[tempHvalue].sid, oneSt.sid); 
					strcpy( recordArray[tempHvalue].sname, oneSt.sname);
					recordArray[tempHvalue].mean = oneSt.mean;
					recordArray[tempHvalue].hvalue = finalHvalue;
					recordArray[tempHvalue].isOccupied = true;
					run = false;
					//cout << "[" << tempHvalue << "]" << recordArray[tempHvalue].sid << "," << recordArray[tempHvalue].sname <<"," << recordArray[tempHvalue].mean <<","<< recordArray[tempHvalue].hvalue <<","/*<< recordArray[i].isOccupied*/ << endl ;
		        } 
		        else
		        	tempHvalue = ( tempHvalue + 1 )% newArraySize;
		    	
		    } // end while
		    
		} // end for
		
		writeFile( fileNumber.c_str(), recordArray, newArraySize );
		
		
		//for(int i = 0 ; i < newArraySize ; i++ ){		
		//	if( recordArray[i].isOccupied )
		//		cout << "[" << i << "]" << recordArray[i].hvalue <<","<< recordArray[i].sid << "," << recordArray[i].sname <<"," << recordArray[i].mean <<","<< /*recordArray[i].isOccupied <<*/ endl ;
		//	else
		//		cout << "[" << i << "]" << endl;
	    //} // for
	    
	    cout<<setiosflags(ios::fixed)<<setprecision(4); // 調整輸出至小數點後四位 
	    cout<<"Hash table has been successfully created by Linear probing\n" << "unsuccessful search: ";
	    cout<< searchNotExist( recordArray, newArraySize )/newArraySize <<" comparisons on average"<< endl;
	    cout<< "successful search: " << searchExist( recordArray, newArraySize )/stNo << " comparisons on average" << endl;
	    
	} // end if
	binFile.close() ;
	
} // end linearProbing()

void quadraticProbing(string fileName, string fileNumber) {
	//cout << fileNumber << "xddd";
	fstream binFile ;
	studentType oneSt ;
	int stNo = 0 ;
	binFile.open(fileName.c_str(),fstream::in | fstream::binary) ;
	if(binFile.is_open()) {
		binFile.seekg(0,binFile.end) ;
		stNo = binFile.tellg()/sizeof(oneSt) ;
		binFile.seekg(0,binFile.beg) ;
		int newArraySize = get_first_prime(int(1.2*stNo));
		record recordArray[newArraySize]; // 宣告hashTable 
		for(int i = 0 ; i <newArraySize ; i++ ) { // 初始化hashTable
			recordArray[i].isOccupied = false;
		} // for
		for(int i = 0 ; i < stNo ; i++ ) {
			binFile.read((char*)&oneSt,sizeof(oneSt)) ;
			int tempHvalue = hash( oneSt.sid, newArraySize );
			int finalHvalue = tempHvalue;
			bool run = true;
			int time = 1;
			while( run ) { // 找空的位置放資訊 
				if( recordArray[tempHvalue].isOccupied == false ) {
					strcpy( recordArray[tempHvalue].sid, oneSt.sid); 
					strcpy( recordArray[tempHvalue].sname, oneSt.sname);
					recordArray[tempHvalue].mean = oneSt.mean;
					recordArray[tempHvalue].hvalue = finalHvalue;
					recordArray[tempHvalue].isOccupied = true;
					run = false;
					//cout << "[" << tempHvalue << "]" << recordArray[tempHvalue].sid << "," << recordArray[tempHvalue].sname <<"," << recordArray[tempHvalue].mean <<","<< recordArray[tempHvalue].hvalue <<","/*<< recordArray[i].isOccupied*/ << endl ;
		        } 
		        else
		        	tempHvalue = ( finalHvalue + time*time )% newArraySize;
		        	time ++;
		    	
		    } // end while
		    
		} // end for
		
		writeQuadraticFile( fileNumber.c_str(), recordArray, newArraySize );		
		
		//for(int i = 0 ; i < newArraySize ; i++ ){		
		//	if( recordArray[i].isOccupied )
		//		cout << "[" << i << "]" << recordArray[i].hvalue <<","<< recordArray[i].sid << "," << recordArray[i].sname <<"," << recordArray[i].mean <<","<< /*recordArray[i].isOccupied <<*/ endl ;
		//	else
		//		cout << "[" << i << "]" << endl;
	    //} // for
	    
	    cout<<setiosflags(ios::fixed)<<setprecision(4); // 調整輸出至小數點後四位 
	    cout<<"Hash table has been successfully created by Quadratic probing\n" << "unsuccessful search: ";
	    cout<< searchQuadraticNotExist( recordArray, newArraySize )/newArraySize <<" comparisons on average"<< endl;
	    cout<< "successful search: " << searchQuadraticExist( recordArray, newArraySize )/stNo + 1 << " comparisons on average" << endl;
	} // end if
	binFile.close() ;
	
} // end quadraticProbing()

float searchNotExist( record recordArray[], int size ){
	float sum = 0;
	for( int i = 0 ; i < size ; i ++ ){
		int j = i;
		while( recordArray[j].isOccupied ){
			sum ++;
			j = (j + 1) % size;
		} // while
	} // for
	return sum;
} // searchNotExist()

float searchExist( record recordArray[], int size ){
	float sum = 0;
	for( int i = 0 ; i < size ; i ++ ) {
		if( recordArray[i].isOccupied ) { // 有資料再看
		    if( i < recordArray[i].hvalue )
			    sum += ( size - recordArray[i].hvalue + i + 1 );
			else
				sum +=( i - recordArray[i].hvalue + 1 );
		} // if
	} // for
	return sum;
} // searchNotExist()

float searchQuadraticNotExist( record recordArray[], int size ) {
	float sum = 0;
	for( int i = 0 ; i < size ; i ++ ){
		int j = i;
		int k = 1;
		while( recordArray[j].isOccupied ){
			j = (i + k*k ) % size;
			sum++;
			k++;
		} // while
	} // for
	return sum;
} // searchNotExist()

float searchQuadraticExist( record recordArray[], int size ){
	float sum = 0;
	for( int i = 0 ; i < size ; i ++ ) {
		int j = i;
		if( recordArray[j].isOccupied ) { // 有資料再看
			int count = 0;
			int go = recordArray[j].hvalue;			
			while( strcmp(recordArray[go].sid, recordArray[j].sid) != 0 ) {
				count ++;
				sum ++;
				go = (recordArray[j].hvalue + count*count) % size;
			} // while
			
		} // if
	} // for
	return sum;
} // searchNotExist()

int Text2Binary(string fileName){ 
	fstream inFile, outFile;
	int stNo = 0;
	do{
		//cout<<"Input a file number:";
		//cin>>fileName;
		if(!fileName.compare("0"))
			return false;
		inFile.open(("input"+fileName+".txt").c_str(),fstream::in) ;
		if(inFile.is_open())
			break ;		
	}while(true);//end while
	
	fileName = "input" + fileName + ".bin" ;
	outFile.open(fileName.c_str(),fstream::out|fstream::binary);
	if(outFile.is_open()) { // 目前 
		char rBuf[BIG_INT];
		while(inFile.getline(rBuf,BIG_INT,'\n')) {
			string temp ;
			studentType oneSt ;
			int cNo = 0,pre = 0,pos = 0 ;
			stNo++ ;
			temp.assign(rBuf) ;
			pos = temp.find_first_of('\t',pre) ;
			while(pos != string::npos ) 
			{   switch(++cNo) 
			    {   case 1 :strcpy(oneSt.sid,temp.substr(pre,pos-pre).c_str()) ;
						break ;
					case 2 :strcpy(oneSt.sname,temp.substr(pre,pos-pre).c_str()) ;
						break ;
					default :oneSt.score[cNo-3] = atoi(temp.substr(pre,pos-pre).c_str()) ;
						break ;
												
				} // end switch
				pre = ++pos ;
				pos = temp.find_first_of('\t',pre);
				
			} // end inner while
			pos = temp.find_last_of('\t') ;
			oneSt.mean = atof(temp.substr(pos + 1).c_str()) ;
			outFile.write((char*)&oneSt,sizeof(oneSt)) ;
		} // end outer while
		outFile.close() ;
	} // end else
	inFile.close() ;
	return stNo ;
} // Text2Binary()

void readBinary(string fileName) { // 讀二進制檔 
	fstream binFile ;
	studentType oneSt ;
	int stNo = 0 ;
	binFile.open(fileName.c_str(),fstream::in | fstream::binary) ;
	if(binFile.is_open()) {
		binFile.seekg(0,binFile.end) ;
		stNo = binFile.tellg()/sizeof(oneSt) ;
		binFile.seekg(0,binFile.beg) ;
		for(int i = 0 ; i < stNo ; i++ ) {
			binFile.read((char*)&oneSt,sizeof(oneSt)) ;
			cout << "[" << i+1 << "]" << oneSt.sid << "," << oneSt.sname << endl ;
			
		} // end for
		
	} // end if
	binFile.close() ;
	
} // end readBinary

int hash( char sid[MAX_LEN], int stNo ) { // hash function
	unsigned long long key = 1;
	int i = 0;
	while( i < strlen(sid) ){
		key *= int(sid[i]);
		i++;
	} // while
	return key % stNo;
} // hash()

int get_first_prime(int n){ // 尋找大於n的最小半澤質數 
int i,j;
int res;
int order=0;
for(i=n+1;i<2*n;i++)
        {
        for(j=2;j<(i/j+1);j++)
                {
                order++;
                res=i/j;
                if((res-j)<=1 && i%j!=0)
                        {
                        return i;
                        }
                if(i%j == 0)
                        {
                        break;
                        }
                else
                        continue;
                }
        }
return 0;
} // get_first_prime()

void writeFile(string fileNumber, record recordArray[], int size ){
	    fstream file ;
	    string fileName;
        fileName = "linear" + fileNumber + ".txt" ; //設定檔名 
		file.open( fileName.c_str() , ios::out ) ;//讓文件開啟為寫入狀態 
		file << " --- Hash table created by Linear probing    ---\n";
        for ( int i = 0 ; i < size ; i++ ) {
  
			if( recordArray[i].isOccupied )
				file << "[" << i << "]" <<"\t"<< recordArray[i].hvalue <<","<< recordArray[i].sid << "," <<"\t"<< recordArray[i].sname <<"," <<"\t"<< recordArray[i].mean << endl ;
			else
			file << "[" << i << "]" << endl;
	    } // for  
	    
        file.close() ;//關閉檔案 
        
} // writeFile()

void writeQuadraticFile(string fileNumber, record recordArray[], int size ){
	    //cout << fileNumber;
	    fstream file ;
	    string fileName;
        fileName = "quadratic" + fileNumber + ".txt" ; //設定檔名 
		file.open( fileName.c_str() , ios::out ) ;//讓文件開啟為寫入狀態 
		file << " --- Hash table created by Quadratic probing    ---\n";
        for ( int i = 0 ; i < size ; i++ ) {
  
			if( recordArray[i].isOccupied )
				file << "[" << i << "]" <<"\t"<< recordArray[i].hvalue <<","<< recordArray[i].sid << "," <<"\t"<< recordArray[i].sname <<"," <<"\t"<< recordArray[i].mean << endl ;
			else
			file << "[" << i << "]" << endl;
	    } // for  
	    
        file.close() ;//關閉檔案 
        
} // writeFile()

void linearSearch(string fileName, char sidd[MAX_LEN] ) {
	fstream binFile ;
	studentType oneSt ;
	int stNo = 0 ;
	binFile.open(fileName.c_str(),fstream::in | fstream::binary) ;
	if(binFile.is_open()) {
		binFile.seekg(0,binFile.end) ;
		stNo = binFile.tellg()/sizeof(oneSt) ;
		binFile.seekg(0,binFile.beg) ;
		int newArraySize = get_first_prime(int(1.2*stNo));
		record recordArray[newArraySize]; // 宣告hashTable 
		for(int i = 0 ; i <newArraySize ; i++ ) { // 初始化hashTable
			recordArray[i].isOccupied = false;
		} // for
		for(int i = 0 ; i < stNo ; i++ ) {
			binFile.read((char*)&oneSt,sizeof(oneSt)) ;
			int tempHvalue = hash( oneSt.sid, newArraySize );
			int finalHvalue = tempHvalue;
			bool run = true;
			
			while( run ) { // 找空的位置放資訊 
				if( recordArray[tempHvalue].isOccupied == false ) {
					strcpy( recordArray[tempHvalue].sid, oneSt.sid); 
					strcpy( recordArray[tempHvalue].sname, oneSt.sname);
					recordArray[tempHvalue].mean = oneSt.mean;
					recordArray[tempHvalue].hvalue = finalHvalue;
					recordArray[tempHvalue].isOccupied = true;
					run = false;
					//cout << "[" << tempHvalue << "]" << recordArray[tempHvalue].sid << "," << recordArray[tempHvalue].sname <<"," << recordArray[tempHvalue].mean <<","<< recordArray[tempHvalue].hvalue <<","/*<< recordArray[i].isOccupied*/ << endl ;
		        } 
		        else
		        	tempHvalue = ( tempHvalue + 1 )% newArraySize;
		    	
		    } // end while
		} // end for
		//cout<<"11111";
	    unsigned long long sum = 1;
		int key;
		int i = 0;
		while( i < strlen(sidd) ){
			sum *= int(sidd[i]);
			i++;
		} // while
		key = sum % newArraySize;
		//cout<<"222222";
		
		int searchTime = 0;
		bool found = false;
		while( recordArray[key].isOccupied ) {
			searchTime ++;
			//cout << "sssss" << searchTime;
			if( strcmp( sidd, recordArray[key].sid ) == 0 ){
				//cout<<"33333";
				cout << recordArray[key].sid << "," << recordArray[key].sname << "," << recordArray[key].mean << "," << "probeTime:" <<searchTime;
				found = true;
	    	} // if
	    	key ++;
	    	
		} // while
		if (!found)
	    	cout << "not found after:" <<searchTime + 1 << " probes";
	    
	} // end if
	
	binFile.close() ;
	
} // end linearProbing()

void quadraticSearch(string fileName, char sidd[MAX_LEN] ) {
	fstream binFile ;
	studentType oneSt ;
	int stNo = 0 ;
	binFile.open(fileName.c_str(),fstream::in | fstream::binary) ;
	if(binFile.is_open()) {
		binFile.seekg(0,binFile.end) ;
		stNo = binFile.tellg()/sizeof(oneSt) ;
		binFile.seekg(0,binFile.beg) ;
		int newArraySize = get_first_prime(int(1.2*stNo));
		record recordArray[newArraySize]; // 宣告hashTable 
		for(int i = 0 ; i <newArraySize ; i++ ) { // 初始化hashTable
			recordArray[i].isOccupied = false;
		} // for
		for(int i = 0 ; i < stNo ; i++ ) {
			binFile.read((char*)&oneSt,sizeof(oneSt)) ;
			int tempHvalue = hash( oneSt.sid, newArraySize );
			int finalHvalue = tempHvalue;
			bool run = true;
			int time = 1;
			while( run ) { // 找空的位置放資訊 
				if( recordArray[tempHvalue].isOccupied == false ) {
					strcpy( recordArray[tempHvalue].sid, oneSt.sid); 
					strcpy( recordArray[tempHvalue].sname, oneSt.sname);
					recordArray[tempHvalue].mean = oneSt.mean;
					recordArray[tempHvalue].hvalue = finalHvalue;
					recordArray[tempHvalue].isOccupied = true;
					run = false;
					//cout << "[" << tempHvalue << "]" << recordArray[tempHvalue].sid << "," << recordArray[tempHvalue].sname <<"," << recordArray[tempHvalue].mean <<","<< recordArray[tempHvalue].hvalue <<","/*<< recordArray[i].isOccupied*/ << endl ;
		        } 
		        else
		        	tempHvalue = ( finalHvalue + time*time )% newArraySize;
		        	time ++;
		    	
		    } // end while
		    
		} // end for

	    unsigned long long sum = 1;
		int key;
		int i = 0;
		while( i < strlen(sidd) ){
			sum *= int(sidd[i]);
			i++;
		} // while
		key = sum % newArraySize;

		
		int searchTime = 0;
		bool found = false;
		int go = key;
		while( recordArray[go].isOccupied && (searchTime < newArraySize) && !found ) {
			searchTime ++;

			if( strcmp( sidd, recordArray[go].sid ) == 0 ){

				cout << recordArray[go].sid << "," << recordArray[go].sname << "," << recordArray[go].mean << "," << "probeTime:" <<searchTime;
				found = true;
	    	} // if
	    	go = key + searchTime*searchTime; 
	    	
		} // while
		
		if (!found)
	    	cout << "not found after:" <<searchTime + 1 << " probes";
	    
	} // end if
	
	binFile.close() ;
	
} // end quadraticProbing()













