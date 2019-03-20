#include <iostream>
#include <stdlib.h>
#include<string.h>
#include<unistd.h>
#include<stdio.h>
#include <fstream>
using namespace std;


int k1=10,k2=5,k3=5,k4=20; // -s -n -l -c
//street, number of segment per street, wait, range
int sRand,nRand,wRand;
int streetNumber[50][2]={{0}};
int i=0,j=0,x=0;
int countRandNumber=0;

int circle(); //declaration 

int main(int argc, char *argv[])
{
    //debug format check 
    // for (int i = 0; i < argc; ++i) 
    //     cout << argv[i] << "\n"; 
/*
./ece650-a3
-l
10
-s
2
-n
2
-c
5
*/
    for (int i = 0; i < argc; ++i){
        if (i%2 == 1){
            std::size_t found1 = std::string(argv[i]).find("-s");
            std::size_t found2 = std::string(argv[i]).find("-n");
            std::size_t found3 = std::string(argv[i]).find("-l");
            std::size_t found4 = std::string(argv[i]).find("-c");

            if (found1!=std::string::npos){
                
                if (atoi(argv[i+1]) >= 2)
                k1= atoi(argv[i+1]);
                // cout<<"found s, k1:" <<k1<<endl;
            }
            if (found2!=std::string::npos){
                if (atoi(argv[i+1]) >= 1)
                k2=1+atoi(argv[i+1]);
                // cout<<"found n, k2:" <<k2<<endl;

            }
            if (found3!=std::string::npos){
                if (atoi(argv[i+1]) >= 5)
                k3= atoi(argv[i+1]);
                // cout<<"found l, k3:" <<k3<<endl;

            }
            if (found4!=std::string::npos){
                k4=atoi(argv[i+1]);
                // cout<<"found c, k4:" <<k4<<endl;

            }

        }
    }
    //     cout << argv[i] << "\n"; 


    // } 
    // for(int i=0;i<argc;i++)
    // {
    //     if(i%2==1) // odd 
    //     {
    //         char cmd[5]={"\0"};
    //         strcpy(cmd,argv[i]);
    //         if(cmd[1]=='s')k1=atoi(argv[i+1]);
    //         if(cmd[1]=='n')k2=atoi(argv[i+1]);
    //         if(cmd[1]=='l')k3=atoi(argv[i+1]);
    //         if(cmd[1]=='c')k4=atoi(argv[i+1]);
    //     }
    // }
    while(true)
    {
        circle();
    }
    return 0;
}
    
    
    
//definition
int circle(){
    std::ifstream urandom("/dev/urandom");
    
    if (urandom.fail())
    {
        std::cerr << "Error: cannot open /dev/urandom\n";
        return 1;
    }
    char ch = 'a';
    // temp = ($urandom % (max - min + 1)) + min;
    urandom.read(&ch, 1);
    sRand=((unsigned int)ch%(k1-2+1))+2;
    urandom.read(&ch, 1);
    wRand=((unsigned int)ch%(k3-5+1))+5;
    for(i=0;i<sRand;i++)
    {
        urandom.read(&ch, 1);
        nRand=(unsigned int)ch%(k2-2+1)+2; // number of segment 
        for(j=0;j<2*nRand;j++)
        {
            urandom.read(&ch, 1);
            streetNumber[j][0]=(((unsigned int)ch%(2*k4+1))-k4); // + - range
            urandom.read(&ch, 1);
            streetNumber[j][1]=(((unsigned int)ch%(2*k4+1))-k4); // generate coordinate street for jth segment
            while(streetNumber[j][0]==streetNumber[j][1])   // detect self loop 
            {
                urandom.read(&ch, 1);
                streetNumber[j][1]=(((unsigned int)ch%(2*k4+1))-k4); // regenerate untill no slf loop 
                countRandNumber++;
                if(countRandNumber>=25)
                {
                    cerr<<"Error: failed to generate valid input for 25 simultaneous attempts"<<endl;
                    return 1;
                }
            }
            int tempStatus=0;
            countRandNumber=0;
            while(tempStatus==0)
            {
                tempStatus=1;
                for(x=0;x<j;x++)
                {   // detect zero length 
                    if((streetNumber[x][0]==streetNumber[j][0])&&(streetNumber[x][1]==streetNumber[j][1]))
                    {
                        urandom.read(&ch, 1);
                        streetNumber[j][0]=(((unsigned int)ch%(2*k4+1))-k4);
                        urandom.read(&ch, 1);
                        streetNumber[j][1]=(((unsigned int)ch%(2*k4+1))-k4);
                        tempStatus=0;
                        break;
                    }
                    // detect ?
                    if((streetNumber[x][0]==streetNumber[j][1])&&(streetNumber[x][1]==streetNumber[j][0]))
                    {
                        urandom.read(&ch, 1);
                        streetNumber[j][0]=(((unsigned int)ch%(2*k4+1))-k4);
                        urandom.read(&ch, 1);
                        streetNumber[j][1]=(((unsigned int)ch%(2*k4+1))-k4);
                        tempStatus=0;
                        break;
                    }
                }
                countRandNumber++;
                if(countRandNumber>=25)
                {
                    cerr<<"Error: failed to generate valid input for 25 simultaneous attempts"<<endl;
                    return 1;
                }
            }
        }
        // cout<<"a \"street"<<(i+1)<<"\"";
        string stn = "a \"";
        stn.append(1, 'a' + i);
        stn += "\"";
        for(x=0;x<nRand;x++)
        {
            // cout<<" ("<<streetNumber[x][0]<<","<<streetNumber[x][1]<<")";
            stn+=" (" + to_string(streetNumber[x][0]) +"," +to_string(streetNumber[x][1]) +")";
        }
        cout<<stn<<endl;
    }
    cout<<"g"<<endl;
    
    sleep(wRand);
    
    for(i=0;i<sRand;i++)
    {
        string stn = "r \"";
        stn.append(1, 'a' + i);
        stn += "\"";
        cout<<stn<<endl;
    }
    
    
    
    return 0;
}

