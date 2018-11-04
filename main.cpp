#define _WIN32_WINNT 0x0500
#include <cstdlib>
#include <stdlib.h>
#include <iostream>
#include <conio.h>
#include <cmath>
#include <ctime>
#include <fstream>
# ifdef __WINNT__
    #include <windows.h>
# else
    // unfortunately we are not ready for OS other than Windows
# endif

using namespace std;

const int NSTATE = 7; // liczba stanów komórki
COORD cur = {0, 0};
int counter [7];

//-------------------------------PARAMETRY----------------------------
double epsilon = 0.2;              //prawdopodobienstwo E->I
double beta = 0.2;                  //prawdopodobienstwo S->E by I
double beta2 = 0.1;                 //prawdopodobienstwo S->E by A
double r = 0.3; 					//prawdopodobienstwo E->A
double gamma1 = 0.05; 			//prawdopodobienstwo wyzdrowienia
double mu = 0.00001;			//prawdopodobienstwo naturalnych narodzin i smierci
double d = 0.01;					//prawdopodobienstwo smierci z powodu choroby
double moveProb = 0.4;                //prawdopodobientwo poruszania sie komorek (0 = wylacz ruch)
double sponInf = 0;                 //prawdopodobienstwo spontanicznych infekcji (0 = wylacz)
int movementRange = 20;				//zasieg komorek
int resDur = 0;                     //czas odpornosci (0 = odpornosc trwala)
int radius = 2;                     //sasiedztwo

const int MAXINT =  2147483647;//    		//"plus nieskonczonosc"
int keyPressed;        						//SPRAWDZ CZY UZYTKOWNIK WCISNAL KLAWISZ
int pattern;                                //wybrany wzorzec sciezek

//-------------------------------PARAMETRY SRODOWISKA----------------------------
int rows = 50;
int cols = 50;
float populationDensity = 0.1;			//poczatkowa gestosc populacji
int nI = 1; 							//pocztkowa liczba chorych
int period = 365;								//czas trwania symulacji
bool moveOn;
int day;


int StateColor[NSTATE] = {
			 0xFFFFFF,
             0xFFFF00,
             0x000030FF,
             0xCC66FF,
             0x33CC33,
             0x000000,
             0x0066FF
};

class Cell {

	public:

    enum CellState{
        Suspectible = 0,
        Exposed = 1,
        Infected = 2,
        Asymptomatic = 3,
        Recovered = 4,
        Dead = 5,
        Empty = 6
    };

	//-------------------------------PARAMETRY KOMORKI----------------------------
		 CellState state;
		 int recDay;                            // dzien, w ktorym komorka nabyla odpornosc
    	 bool inMotion;   						// 0 - stoi w miejscu 1 - jest w trakcie podrozy
    	 bool moved;      						// 0 - nie wykonala jeszcze ruchu 1 - wykonala ruch
    	 int destX;       						// X celu
    	 int destY;       						// Y celu
    	 int nextX;
    	 int nextY;

		 void Infect(){
			 state = Infected;
		 }
		 void Expose(){
			 state = Exposed;
		 }
		 void MakeAsymptomatic(){
			 state = Asymptomatic;
		 }
		 void Recover(){
			 state = Recovered;
		 }
		 void MakeSuspectible(){
			 state = Suspectible;
		 }
		 void Die(){
			 state = Dead;
		 }
		 void Born(){
			 state = Suspectible;
		 }
		 void MakeEmpty(){
			 state = Empty;
		 }
};

Cell **cells;
Cell ** cellsCopy;

int **weights;
int **paths;

double Losuj() {
    return (double)rand() / (double)(RAND_MAX);
}

//------------------------EXPOSED -> INFECTED---------------------------------------
void Infection(int i, int j){
			double random = Losuj();
				if(random < epsilon){
					cells[i][j].Infect();
				}
				else if(random < r){
					cells[i][j].MakeAsymptomatic();
				}
}
//------------------------SUSPECTIBLE -> EXPOSED------------------------------------
void Exposition(int i, int j){
				for(int m=-radius;m<=radius;m++){
					for(int n=-radius;n<=radius;n++){
						if (i + m < rows && j + n < cols && i + m >= 0 && j + n >= 0){
                            double random = Losuj();
							if (cellsCopy [i + m] [j + n].state == Cell::Infected && random < beta){
								cells [i] [j].Expose();
							}
							else if (cellsCopy [i + m] [j + n].state == Cell::Asymptomatic && random < beta2){
                                cells [i] [j].Expose();
							}
						}
					}
				}

}


void SpontaneousInfection(int i, int j){

    double random = Losuj();
    if (random < sponInf){
        cells[i][j].Expose();
    }

}
//------------------------INFECTED -> RECOVER---------------------------------------
void Recovery(int i, int j){
			//------------------------I/A -> Rec---------------------------------------
			double random = Losuj();
			if(cellsCopy[i][j].state == Cell::Infected && random < gamma1){
				cells [i] [j].Recover();
				cells [i] [j].recDay = day;
			}
			else if(cellsCopy[i][j].state == Cell::Asymptomatic && random < gamma1){
				cells[i] [j].Recover();
				cells[i] [j].recDay = day;
			}
}

//------------------------RECOVERED -> SUSPECTIBLE---------------------------------------
void EndResistance(int i, int j){
			//------------------------Rec -> Sus---------------------------------------
			if(cellsCopy[i][j].recDay + resDur == day){
				cells [i] [j].MakeSuspectible();
			}
}
//------------------------SMIERC Z POWODU CHOROBY-----------------------------------
void DeathByIlness(int i, int j){
			double random = Losuj();
			if(random < d){
				cells [i] [j].Die();
			}
}
//------------------------NATURALNE NARODZINY I SMIERC------------------------------
void NaturalDeathAndBirth(int i, int j){
			double random = Losuj();
			if(cellsCopy[i][j].state == Cell::Dead && random < mu){
				cells [i] [j].Born();
			}
			else if(cellsCopy[i][j].state != Cell::Empty && random < mu){
				cells [i] [j].Die();
			}
}


//------------------------RUCH KOMOREK------------------------------

//------------------ZAPISZ NAJKROTSZA DROGE----------------------------
void FWPath(int i, int j, int x, int y){

     if(paths [x][y] == -1){//nie ma drogi do celu - przerwij funkcje
        cells [i][j].nextX = -1;
		cells [i][j].nextY = -1;
        return;}
	if (paths [x][y] == x) {
		cells [i][j].nextX = y/cols;
		cells [i][j].nextY = y%cols;
	}
	else FWPath(i, j, x, paths[x][y]);
}

//-----------------------USTAL CEL PODROZY KOMORKI----------------------------
void setDirection(int i, int j){

    int minX = 1;
    int minY = 1;
    int maxX = rows-2;
    int maxY = cols-2;

	do{
		cells [i][j].destX = rand()%((2*movementRange)+1)-(movementRange)+i;
		cells [i][j].destY = rand()%((2*movementRange)+1)-(movementRange)+j;
	}while(cells [i][j].destX == i && cells [i][j].destY == j ||cells [i][j].destX < minX || cells [i][j].destX > maxX || cells [i][j].destY < minY || cells [i][j].destY > maxY);
}

void MoveCell(int i, int j){

     int x = cols * i + j; 																		//nr komorki
     int y = cols * cells[i][j].destX + cells[i][j].destY; 									//nr komorki, ktora jest celem



     FWPath(i,j,x,y);
     if(cells [i][j].nextX==-1){return;}//przerwij ruch, jezeli nie ma drogi do celu

     cells [i][j].moved = 1;

     if(cells [i][j].destX - i == 0 && cells [i][j].destY - j == 0){ 							// WYLACZ RUCH KOMORKI, JESLI DOTARLA DO CELU
     	cells [i][j].inMotion=0;
     	return;
     }

     Cell copy = cells [cells [i][j].nextX][cells [i][j].nextY]; 								// ZAPISZ PARAMETRY KOMORKI NA DRODZE PORUSZAJACEJ SIE KOMORKI
     if(copy.state == Cell::Empty){
      	cells [i][j].inMotion=0;
     	return;
     }

    if(copy.state == Cell::Dead){
        cells [cells [i][j].nextX][cells [i][j].nextY] = cells [i][j];
        cells [i][j] = copy;
    }
    else{
        if(copy.nextX == i && copy.nextY == j && copy.moved == 0){ //jesli zamierzaja sie wyminac, zamien je miejscami, tylko w wypadku, jezeli obie komorki, jeszcze sie nie poruszyly
            copy.moved = 1;                     //komorka wymijana rownierz wykonala ruch!
            cells [cells [i][j].nextX][cells [i][j].nextY] = cells [i][j];
            cells [i][j] = copy;
        }

        else{                                   //jesli nie zamierzaja sie wyminac, szukaj obejscia (tylko takiego, ktore zblizy do celu, inaczej stój)
            for(int m=-1;m<=1;m++){
					for(int n=-1;n<=1;n++){
						if(i+m<rows && j+n<cols && i+m>=0 && j+n >=0){
							if(cells[i+m][j+n].state == Cell::Dead){
                                    if(abs(cells[i][j].destX-(i+m))<=abs(cells[i][j].destX-(i)) && abs(cells[i][j].destY-(j+n))<=abs(cells[i][j].destY-(j))){
                                        copy = cells[i+m][j+n];
                                        cells[i+m][j+n] = cells [i][j];
                                        cells [i][j] = copy;
                                        return;
                                    }
							}
						}
					}
            }
        }
    }

 }

void SetupArrays(){
    //--------------------------2 TABLICE KOMOREK----------------------------------
	cells = new Cell*[cols];
	cellsCopy = new Cell*[cols];
	for (int i = 0; i < cols; i++) {
		cells[i] = new Cell [rows];
		cellsCopy[i] = new Cell [rows];
	}
	//-----------------------2 TABLICE WAG I SCIEZEK--------------------------------

	int dim = rows*cols; 					//wymiary tablicy wag i sciezek

	weights = (int **) malloc(dim * sizeof(int *));
	paths = (int **) malloc(dim * sizeof(int *));;
	for (int i = 0; i < dim; i++){
		weights[i]  = (int*) malloc(dim * sizeof(int));
		paths[i]  = (int*) malloc(dim * sizeof(int));
		if(weights[i]==0){break;}
	}

	//--------------------------USTAL POCZATKOWE WAGI PRZEJSC----------------------------------
	for (int i=0;i<dim;i++){
		for(int j=0;j<dim;j++){
			weights[i][j]=MAXINT;
			paths[i][j]=-1;
		}
		weights[i][i] = 0;
		paths[i][i] = i;
	}
 }
void GetConditions(){

	cout << "Simulation duration (days): ";
	cin >> period;
	cout << "Initial density of population (%): ";
	cin >> populationDensity;
	populationDensity=populationDensity/100;
	cout << "Initial number of infected individuals: ";
	cin >> nI;
	cout << "Probability of S->E by Infected: ";
	cin >> beta;
	cout << "Probability of S->E by Asymptomatic: ";
	cin >> beta2;
	cout << "Probability of E->I: ";
	cin >> epsilon;
	cout << "Probability of E->A: ";
	cin >> r;
	cout << "Probability of recover: ";
	cin >> gamma1;
	cout << "Probability of natural deaths and births: ";
	cin >> mu;
	cout << "Probability of death from a disease: ";
	cin >> d;
	cout << "Probability of spontaneous infection (0 = OFF): ";
	cin >> sponInf;
	cout << "Duration of resistance period (0 = inf): ";
	cin >> resDur;
	cout << "Probability of individual movement (0 = OFF): ";
	cin >> moveProb;
	if(moveProb!=0){
		cout << "Individual scope: ";
		cin >> movementRange;
	}
	cout << "Neighborhood range: ";
	cin >> radius;

	system("CLS");

	//--------------------------WYBOR SCIEZEK---------------------------
	if(moveProb!=0){
    cout << "Choose your path pattern:"<<endl<<endl<<endl<<endl<<endl;
    cout <<"1)              2)              3)"<<endl<<endl<<endl<<endl<<endl;
    cout << "Your choice: ";
    int choice;
    int a = 40;
    HWND myconsole = GetConsoleWindow();
	HDC hdc = GetDC( myconsole );
    for(int i=0;i<50;i++){
        for(int j=0;j<50;j++){
            if(i==0 || i == 49 || j == 0 || j == 49){
                SetPixel(hdc,j+37,i+a,0xFFFFFF);
                SetPixel(hdc,j+180,i+a,0xFFFFFF);
                SetPixel(hdc,j+300,i+a,0xFFFFFF);
            }
            else if(i > 21 && i < 26){
                    SetPixel(hdc,j+180,i+a,StateColor[6]);
                    SetPixel(hdc,j+300,i+a,StateColor[6]);
            }
            else if(j > 21 && j < 26){
                SetPixel(hdc,j+300,i+a,StateColor[6]);
            }
        }
    }
    do{
        cin >> pattern;
    }while(pattern<1 || pattern>3);

	fstream file("parameters.log", ios::out);//stworz plik log
	file << "Simulation parameters:"<<endl<<endl;
    file << "Simulation duration (days): ";
	file << period;
    file << endl;
	file << "Initial density of population (%): ";
	file << populationDensity*100;
    file << endl;
	file << "Initial number of infected individuals: ";
	file << nI;
    file << endl;
	file << "Probability of S->E by Infected: ";
	file << beta;
    file << endl;
	file << "Probability of S->E by Asymptomatic: ";
	file << beta2;
    file << endl;
	file << "Probability of E->I: ";
	file << epsilon;
    file << endl;
	file << "Probability of E->A: ";
	file << r;
    file << endl;
	file << "Probability of recover: ";
	file << gamma1;
    file << endl;
	file << "Probability of natural deaths and births: ";
	file << mu;
    file << endl;
	file << "Probability of death from a disease: ";
	file << d;
    file << endl;
	file << "Probability of spontaneous infection (0 = OFF): ";
	file << sponInf;
    file << endl;
	file << "Duration of resistance period (0 = inf): ";
	file << resDur;
    file << endl;
	file << "Probability of individual movement (0 = OFF): ";
	file << moveProb;
    file << endl;
	file << "Individual scope: ";
	file << movementRange;
    file << endl;
	file << "Neighborhood range: ";
	file << radius;
	file << endl << "Pattern: " << pattern;
    file.close();

    for(int i=40;i<90;i++){//WYCZYSC EKRAN
        for(int j=0;j<350;j++){
            SetPixel(hdc,j,i,Cell::Dead);
        }
    }
	}
}
void SetInitialConditions(){
    day=1;                                                  //wyzeruj licznik dni
	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
            cells[i][j].inMotion=0;                         //wyzeruj ruch komorek
            cells[i][j].recDay=0;                           //wyzeruj dzien nabycia odpornosci dla komorek
			cells [i][j].Die();								//wypelnij siatke martwymi komorkami
			double random = Losuj();		//wstaw podatne komorki zgodnie z gestoscia populacji


            if (random < populationDensity) {
				cells [i][j].Born();
			}
			if (i == 0 || j == 0 || i == rows-1 || j == cols-1) {	//warunki brzegowe
				cells [i][j].MakeEmpty();
			}
		}
	}
	//--------------------wstaw zainfekowane komorki--------------------
	for (int i = 0; i < nI; i++) {
		int x = rand()%(cols-2)+1;
		int y = rand()%(rows-2)+1;
		cells [x][y].Infect();
	}
    for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			cellsCopy [i] [j] = cells [i] [j];	//kopia komorek z poprzedniej iteracji
		}
    }

}

void DrawGrid() {
	//system("CLS");

	HWND myconsole = GetConsoleWindow();
	HDC hdc = GetDC( myconsole );


	int a =0;
	int b=0;
	for(int x = 0; x < rows; x++){
		for(int y = 0; y < cols; y++){
                for(int i=0;i<4;i++){
                        for(int j=0;j<4;j++){
                        SetPixel(hdc,i+a,j+b,StateColor[cells[x][y].state]);
                        }
                }a+=4;
		}a=0;b+=4;
	}
}
void ShowLog(HANDLE handle) {
    /*
    zapis w logu do analiz
    */
	cout << endl<< endl<< endl<< endl<< endl<<endl<< endl<< endl<< endl<< endl<< endl<< endl<< endl<< endl<< endl<<endl<< endl<< endl<< endl<< endl<< endl<< endl<< endl<< endl<< endl<< endl<< endl<< endl<< endl<< endl<< endl<< endl<< endl<< endl<< endl<< endl<< endl;
    SetConsoleTextAttribute(handle, 7);
	cout << "Day: " << day << "            " << endl;
	cout << "Population: " << rows*cols - counter[5] - counter[6] << "            "<< endl;

	SetConsoleTextAttribute(handle, 159);
	cout << char(254);
	SetConsoleTextAttribute(handle, 7);
	cout << "Suspectible: " << counter[0] << "            "<< endl;

	SetConsoleTextAttribute(handle, 155);
	cout << char(254);
	SetConsoleTextAttribute(handle, 7);
	cout << "Exposed: " << counter[1] << "            "<< endl;

	SetConsoleTextAttribute(handle, 157);
	cout << char(254);
	SetConsoleTextAttribute(handle, 7);
	cout << "Asymptomatic: " << counter[3] << "            "<< endl;

	SetConsoleTextAttribute(handle, 156);
	cout << char(254);
	SetConsoleTextAttribute(handle, 7);
	cout << "Infected: " << counter[2]  << "            " <<endl;

	SetConsoleTextAttribute(handle, 154);
	cout << char(254);
	SetConsoleTextAttribute(handle, 7);
	cout << "Recovered: " <<counter[4]  << "            "<< endl;

	SetConsoleTextAttribute(handle, 144);
	cout << char(254);
	SetConsoleTextAttribute(handle, 7);
	cout << "Dead: " << counter[5] << "            " <<endl;
    cout << endl<< endl<< endl<< "[space] - PAUSE/RESUME"<<endl << "[m] - main menu" << endl<<"[q] - quit program";

}

//------------------------RUCH KOMOREK------------------------------
void Move(){
for (int i=0;i<rows;i++){
        for(int j = 0;j<cols;j++){
           double random = Losuj();
           if(cells[i][j].state != Cell::Dead && cells[i][j].state != Cell::Empty && cells[i][j].inMotion == 0){
			   if(random<moveProb){
				   cells[i][j].inMotion = 1;
				   setDirection(i,j);
			   }
		   }

		   if(cells[i][j].inMotion == 1 && cells[i][j].moved == 0){
			   MoveCell(i,j);
		   }
        }
    }
}

void setInitialweights(){

	for(int i=0;i<rows*cols;i++){
                       for(int j=0;j<rows*cols;j++){

                                if(i==j){
                                weights[i][j]=0;
                               }
                               if(i/cols == j/cols && (j%cols)+1==i%cols || i/cols == j/cols && (j%cols)-1==i%cols){
                                    weights[i][j]=40;
                                    if(pattern == 2 || pattern == 3){
                                        if(i/cols > 21 && i/cols < 26){
                                            weights[i][j]=1;
                                        }
                                    }
                                    paths[i][j]=i;
                               }
                               else if(j%cols==i%cols && (j/cols)+1==(i/cols) || j%cols==i%cols && (j/cols)-1==(i/cols)){
                                    weights[i][j]=40;
                                    if(pattern == 3){
                                        if(i%cols >21 && i%cols < 26){
                                            weights[i][j]=1;
                                        }
                                    }
                                    paths[i][j]=i;
                               }
                               else if((j%cols)+1==i%cols && (j/cols)+1==(i/cols)){
                                    weights[i][j]=40;
                                    paths[i][j]=i;
                               }
                               else if((j%cols)+1==i%cols && (j/cols)-1==(i/cols)){
                                    weights[i][j]=40;
                                    paths[i][j]=i;
                               }
                               else if((j%cols)-1==i%cols && (j/cols)+1==(i/cols)){
                                    weights[i][j]=40;
                                    paths[i][j]=i;
                               }
                               else if((j%cols)-1==i%cols && (j/cols)-1==(i/cols)){
                                    weights[i][j]=40;
                                    paths[i][j]=i;
                               }


                       }
	}
}

void Floyd(){

    int w = 0;
	int dim = rows*cols;

	for(int k=0;k<dim;k++){
		for(int i=0;i<dim;i++){
			for(int j=0;j<dim;j++){
				if((weights[i][k] == MAXINT) || (weights[k][j] == MAXINT)) {continue;}
				w = weights[i][k]+weights[k][j];


				if(weights[i][j]>w){
					weights[i][j]=w;
					paths[i][j]=paths[k][j];
				}
			}
		}



		if(k%(dim/100)==0){system("CLS");cout<<"Creating a shortest paths matrix: " << k/(dim/100) << "%";}
	}
	cout << endl << "Saving matrix to the file, please wait..." << endl;
fstream file("weights.txt", ios::out);
	for(int i =0;i<dim;i++){
            for(int j=0;j<dim+1;j++){
                file << weights[i][j] << "   ";
            }file << endl;
		}file.close();

const char* name;
switch(pattern){
        case 1: name="shortest_paths.txt"; break;
        case 2: name="shortest_paths_2.txt"; break;
        case 3: name="shortest_paths_3.txt"; break;
        }
    fstream file2(name, ios::out);
	for(int i =0;i<dim;i++){
            for(int j=0;j<dim;j++){
                file2 << paths[i][j] << endl;
            }
		}file2.close();
}
void CountCells(){
for(int i = 0;i<NSTATE;i++){
             counter [i] = 0;
     }                               //zeruj licznik
     for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
            counter[cells[i][j].state]++;       //licz komorki
		}
     }
}
void Update(){
    	for (int i = 0; i < rows; i++) {
		for (int j = 0; j < cols; j++) {
			cells[i][j].moved=0;				//wyzeruj falge ruchu
			cellsCopy [i] [j] = cells [i] [j];	//kopia komorek z poprzedniej iteracji

			switch(cellsCopy[i][j].state){
			case 0: Exposition(i,j);
                    if(sponInf!=0){                                                        //suspectible
                        SpontaneousInfection(i,j);
                    }
                    break;
			case 1: Infection(i,j); break;                                                    //exposed
			case 2: Recovery(i,j); DeathByIlness(i,j); break;                      //infected
			case 3: Recovery(i,j); break;                                       //asymptomatic
			case 4: if(resDur!=0){                                                         //recovered
                        EndResistance (i,j);
                    }
                    break;
			}
			NaturalDeathAndBirth (i,j);    //wykonaj dla wszystkich komorek
		}
	}
	if(moveProb!=0){
                    Move();
                }

    CountCells();
	//zapisz dane do pliku
	fstream file("output.txt", ios_base::app);
    file << day << "    " << counter[0] << "    " << counter[1] << "    " << counter[2] << "    " << counter[3] << "    " << counter[4] << "    " << counter[5] << endl;
    file.close();

}

void LoadData(){
    int i=0;
    int j=0;
    string line;
    fstream plik;

    switch(pattern){
        case 1: plik.open("shortest_paths.txt",ios::in); break;
        case 2: plik.open("shortest_paths_2.txt",ios::in); break;
        case 3: plik.open("shortest_paths_3.txt",ios::in); break;
        }

    if(plik.good()==false){
            cout << "There is no file with the shortest paths matrix in your directory! Do you want me to create sample file for you (it may take a while)? (y/n)";
            keyPressed = getch();
            if(keyPressed==89 || keyPressed==121){setInitialweights();Floyd();}
            else{exit(0);}
    }
    else{cout << "Loading the shortest paths. Please wait..."<<endl;}
    while(getline(plik,line)){
        paths[i][j]=atoi(line.c_str());
        j++;
        if(j==rows*cols){j=0;i++;}
    }plik.close();
    cout << "Loaded. Press any key to continue...";getch();
}
void ShowMsg(string msg) {
    cout << msg;
    cout << endl << "Press any key to go back to main menu...";
    getch();
}
void ShowFormattedMsg(HANDLE hand, const char * msg) {
    byte low = 15;
    byte high = 12;
    const char * CONTROL = "@";
    const char * HIGH = "^";
    const char * LOW = "v";
    bool check =0;

    for( char const * ptr = msg; *ptr; ++ptr){
            char c = *ptr;
            if (c == '@'){
                check = 1;
            }
            else if(check == 1){
                if(c == '^'){
                    SetConsoleTextAttribute(hand, high);
                }
                else if(c == 'v'){
                    SetConsoleTextAttribute(hand, low);
                }
                else if(c=='@'){
                    cout << c;
                }
                check=0;
            }
            else{
                cout << c;
            }

    }

}
void Help(){
    const char * msg =
        "Press 's' to start program. After that you will be asked to provide parameter set on\n"
        "the purpose of your simulation. Please pay attention on the units you are asked for.\n"
        "Population density should be a figure between 1 - 100. Be aware that this is only approximate\n"
        "percentage of population and number of individuals may slightly differ from expectations.\n"
        "Remember that probabilities values must be decimals between 0 - 1 interval.\n"
        "For the purpose of individuals movement you are able to provide your own input file with\n"
        "declaration of the shortest paths values between cells for Floyd-Warshall algorithm.\n"
        "Please make sure that your input format is properly formated text document saved in the same\n"
        "catalog as this executable. Each value in the path file should be in a separate line.\n"
        "During simulation you can pause and resume the process by hitting SPACE, return to the\n"
        "menu with 'm' or close the console using 'q'. Have fun!";
    ShowMsg(msg);
}

void Info(){
    const char * msg =
        "Version: 1.0\n"
        "Written by: Eryk Dobkowski\n\n"
        "Epidemy(2017) is simple simulation program based on Cellular Automata aproach.\n"
        "It was written in 2017 on the purpose of bachelor final project by Eryk Dobkowski\n"
        "graduating in this year Bioinfomatics at University of Environmental and Life Sciences.\n"
        "As an author I would like to thank my dissertation thesis Supervisor - Jan Jelowicki\n"
        "for his wise guidance and feedback provided to me during the progress of this project.\n"
        "Many thanks to all my friends supporting me at every point of my life.\n";
    ShowMsg(msg);
}

int ShowMenu(HANDLE handle){
    int keyPressed;        						//SPRAWDZ CZY UZYTKOWNIK WCISNAL KLAWISZ
    const char * hotkeys = "shit";
    int nhotkeys = 4;
    const char * msg =
        "@v---------------------------------------------------------------------------------------\n"
        "*                                                                                     *\n"
        "*                                                                                     *\n"
        "*@^          '||''''|            ||       '||                                           @v*\n"
        "*@^           ||  .    ... ...  ...     .. ||    ....  .. .. ..   .... ...              @v*\n"
        "*@^           ||''|     ||'  ||  ||   .'  '||  .|...||  || || ||   '|.  |               @v*\n"
        "*@^           ||        ||    |  ||   |.   ||  ||       || || ||    '|.|                @v*\n"
        "*@^          .||.....|  ||...'  .||.  '|..'||.  '|...' .|| || ||.    '|                 @v*\n"
        "*@^                     ||                                        .. |                  @v*\n"
        "*@^                   ''''                                        ''                    @v*\n"
        "*                               CELLULAR AUTOMATA                                     *\n"
        "*                                      2017                                           *\n"
        "*                                                                                     *\n"
        "*                             Author: Eryk Dobkowski                                  *\n"
        "*                                                                                     *\n"
        "*                                                                                     *\n"
        "*                                                                                     *\n"
        "*                                                                                     *\n"
        "*                                                                                     *\n"
        "*                       ===================================                           *\n"
        "*                                                                                     *\n"
        "*                                                                                     *\n"
        "*                               @^[S]@v Start                                             *\n"
        "*                               @^[H]@v Help                                              *\n"
        "*                               @^[I]@v Info                                              *\n"
        "*                               @^[T]@v Terminate                                         *\n"
        "*                                                                                     *\n"
        "*                                                                                     *\n"
        "*                                                                                     *\n"
        "*                                                                                     *\n"
        "*                                                                                     *\n"
        "*                                                                                     *\n"
        "---------------------------------------------------------------------------------------\n";

    const char * wrong = "*                           @^***Incorrect choice***@v                                    *\n";

//------------
    system("CLS");
    do {
        ShowFormattedMsg(handle, msg);
        keyPressed = getch() | 32 ; // only lowercase ascii
        for (int i=0; i<nhotkeys;i++) {
            if (hotkeys[i] == keyPressed) {
                system("CLS");
                return i;
            }
        }
        system("CLS");
        ShowFormattedMsg(handle, wrong);
    } while (true); //nieskonczona petla
}

int Options(int keyPressed, HANDLE okno){
    if(keyPressed == 32){  // SPACJA = PAUZA
                      getch();
                     }
    else if(keyPressed == 109){  // m = MENU
                      return -1;
                     }
    else if(keyPressed == 113){  // q = QUIT
                      exit(0);
                     }
}

void Run(HANDLE okno) {
    fstream file("output.txt", ios::out);//stworz lub wyzeruj plik output
        file << "day" << "    " << "S" << "    " << "E" << "    " << "I" << "    " << "A" << "    " << "R" << "    " << "D" << endl;
        file.close();
    int counter [NSTATE]; // wektor licznikowy komórek
    const char * hotkeys = " mq";
    int nhotkeys = 3;
    COORD cur = {0, 0};
    GetConditions();
    SetupArrays();
    SetInitialConditions ();
    if(moveProb!=0){
                  LoadData();   //Wczytaj macierz najkrotszych sciezek z pliku
    }
    system("CLS");
    CountCells();
    DrawGrid();
    ShowLog(okno);
    do {
        SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), cur);
        Update();
        DrawGrid();
        ShowLog(okno);

        int keyPressed;        						//SPRAWDZ CZY UZYTKOWNIK WCISNAL KLAWISZ
        if (kbhit()) {
            int check=0;
           keyPressed = getch() | 32 ;
           for (int i=0; i<nhotkeys;i++) {
            if (hotkeys[i] == keyPressed) {
                check = Options(keyPressed, okno);
            }
           }
        if(check<0){
            break;
        }
        }

        day++;
        Sleep(100);
    } while (day<=period);
    if(day>=period){
        cout << endl << "End of simulation. Press any key to return to main menu";getch();
    }
    delete [] weights;
    delete [] paths;
    delete [] cells;
    delete [] cellsCopy;
}

int main(int argc, char *argv[]) {
	srand(time(NULL));
    HANDLE okno = GetStdHandle(STD_OUTPUT_HANDLE);
    CONSOLE_CURSOR_INFO info;
    info.dwSize = 100;
    info.bVisible = FALSE;
    COORD size;
    size.X = 600;
    size.Y = 150;
    system("MODE 100");
    SetConsoleCursorInfo(okno, &info);
    do {

        switch(ShowMenu(okno)) {
            case 0: Run(okno); break;
            case 1: Help(); break;
            case 2: Info(); break;
            case 3: exit(0); break;
        }
    } while (true);
}
