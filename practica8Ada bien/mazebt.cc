//15424611Y
#include<iostream>
#include<fstream>
#include<cstring>
#include<stdio.h>
#include<unistd.h>
#include<vector>
#include<chrono>

using namespace std;
//using namespace chrono;


int crides=0;
int laberinto[10000][10000];
bool b_lab[10000][10000];
int i_lab[10000][10000];
int row, col;
typedef struct{
    int x;
    int y;
}cor;
bool read_file(string);
int chebyshov(int, int);
void print2D(const vector<cor> v);
bool voraz2(int x, int y, int &millor, vector<cor> &v);
void backtracking3(int x, int y, int recorrido, int &mejor_recorrido,vector<cor> &v,vector<cor> &vv);



int main(int argc, char* argv[]){
    int c;
    bool args=false;
    //bool f_flag=false;
    bool p_flag=false;
    string n_file;
    vector<cor> best_sol,current,tmp,tmp2;
    const string s1="Shortest path length= ";
    const string s2="CPU elapsed time= ";
    const string s3="NO EXIT";
    const string s4=" ms.";
    const string s5="Path 2D:\n";
    //cout<<"hola"<<endl;
    while ((c=getopt(argc,argv,":pf:"))!=-1){
        switch ( c )
        {
            case 'f':
            if(optarg!=0){
                //f_flag=true;
                n_file=optarg;
                args=true;
            }
            else{
                args=false;
            }
            break;
            case 'p':
            p_flag=true;
            args=true;
            break;
            case '?':
            cerr<<"Usage:\nmaze-bt [-p] -f fichero_entrada"<<endl;
            return -1;
            break;
            default:
            cerr<<"Usage:\nmaze-bt [-p] -f fichero_entrada"<<endl;
            return -1;
        }
    }
    if(!args){
        cerr<<"Usage:\nmaze-bt [-p] -f fichero_entrada"<<endl;
        return -1;
    }
    else{
        if(read_file(n_file)){
            int vsol,mejor_recorrido=9999999;
            auto start= clock();
            if(voraz2(0,0,vsol,tmp)){
                best_sol=tmp;
                mejor_recorrido=vsol;
            }
            backtracking3(0,0,0,mejor_recorrido,current,best_sol);
            auto end=clock();
            if(best_sol.size()>0){
                cout<<s1<<mejor_recorrido<<endl;
                cout<<s2<<1000.0*(end-start)/CLOCKS_PER_SEC <<s4<<endl;
                if(p_flag){
                cout<<s5;
                print2D(best_sol);
                }
            }
            else{
                cout<<s3<<endl;
            }
        }

    }
}

bool voraz2(int x, int y, int &millor, vector<cor> &v){
    bool res=false;
    int dreta,baix;
    //para saber si ir por la derecha o abajo
    dreta=chebyshov(x+1,y);
    baix=chebyshov(x,y+1);

    cor cc;
    cc.x=x;
    cc.y=y;

    if(x>=0&&x<row&&y>=0&&y<col&&laberinto[x][y]==1&&x==row-1&&y==col-1){
        v.push_back(cc);
        millor++;
        res=true;
    }
    else if(laberinto[x][y]==1){
        v.push_back(cc);
        if(x+1>=0&&x+1<row&&y+1>=0&&y+1<col&&laberinto[x+1][y+1]==1){
        millor++;
        res=voraz2(x+1,y+1, millor,v);
        }
        else{
            if(x+1>=0&&x<row&&y>=0&&y<col&&laberinto[x+1][y]==1&&dreta<=baix){
                millor++;
                res= voraz2(x+1,y, millor,v);

            }
            else if(x>=0&&x<row&&y+1>=0&&y+1<col&&laberinto[x][y+1]==1){
                millor++;
                res=voraz2(x,y+1,millor,v);
            }
            else{
				if(x+1>=0&&x+1<row&&y-1>=0&&y-1<col&&laberinto[x+1][y-1]==1){
					millor++;
					res=voraz2(x+1,y-1,millor,v);
				}
			}

        }
    }
    return res;
}
void backtracking3(int x, int y, int recorrido, int &mejor_recorrido,vector<cor> &v,vector<cor> &vv){
    if(x>=0&&x<row&&y>=0&&y<col){//factible
       //cout<<i_lab[x][y]<<endl;
       //si ha trobat solució
        if(laberinto[x][y]==1&&b_lab[x][y]==false&&recorrido<i_lab[x][y]&&recorrido+1<mejor_recorrido){//accessible,visitat,recorregut casella menor, recorregut menor que millor recorregut
            b_lab[x][y]=true;
            i_lab[x][y]=recorrido;
            crides++;
            cor c;
            c.x=x;
            c.y=y;
            v.push_back(c);
            if(x==row-1&&y==col-1){//fulla
                if(recorrido+1<mejor_recorrido){//millor solucio que la actual
                    mejor_recorrido=recorrido+1;
                    vv=v;//vector de solucion=vector actual
                }
            }
            else{
              //fa els backtrackings
                if(recorrido+chebyshov(x,y)<mejor_recorrido){//prometedora
                        backtracking3(x+1,y+1,recorrido+1,mejor_recorrido,v,vv);//diagonal
                        backtracking3(x,y+1,recorrido+1,mejor_recorrido,v,vv);//baix
                        backtracking3(x+1,y,recorrido+1,mejor_recorrido,v,vv);//dreta
                        backtracking3(x-1,y+1,recorrido+1,mejor_recorrido,v,vv);//diagonal superior
                        backtracking3(x+1,y-1,recorrido+1,mejor_recorrido,v,vv);//diagonal inferior
                        backtracking3(x,y-1,recorrido+1,mejor_recorrido,v,vv);//dalt
                        backtracking3(x-1,y,recorrido+1,mejor_recorrido,v,vv);//baix
                        backtracking3(x-1,y-1,recorrido+1,mejor_recorrido,v,vv);//diagonal contraria
                }
            }
            v.pop_back();//para sacar del vector actual limpiar buffer
            b_lab[x][y]=false;//ja ha recorregut eixa posició
        }
    }
}
void print2D(const vector<cor> v){
    int k=v.size();
    int d=0;
    bool p=false;
    for(int i=0;i<row;i++){
        for(int j=0;j<col;j++){
            //comparar que i, j esta
            for(int l=d;l<k&&!p;l++){
                if(v[l].x==i&&v[l].y==j){
                    p=true;
                }
            }
            if(p){
                cout<<"*";
                d++;
                p=false;
            }
            else{
                cout<<laberinto[i][j];
            }
        }
        if(i!=row-1)
         cout<<"\n";
    }
}
int chebyshov(int x, int y){//cota optimista
    if(row-x>col-y){
        return row-x;
    }
    else return col-y;
}
bool read_file(string file_name){
    ifstream fi;
    int r;
    bool b=false;
    fi.open(file_name.c_str(),ios::in);
    if(fi.is_open()){
        b=true;
        fi>>row;
        fi>>col;
        while(!fi.eof()){
            for(int i=0;i<row;i++){
                for(int j=0;j<col;j++){
                    fi>>laberinto[i][j];
                    i_lab[i][j]=999999;
                    b_lab[i][j]=false;
                }
            }
            fi>>r;
        }
        fi.close();
    }
    else{
        cerr<<"ERROR: can't open file: "<<file_name<<".\nUsage:\nmaze-bt [-p] -f fichero_entrada"<<endl;
    }
    return b;
}
