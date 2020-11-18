
#include <iostream>
#include <vector>
#include "SimpleAnomalyDetector.h"
#include <fstream>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */
#include <math.h>

// this is a simple test to put you on the right track
void generateTrainCSV(float a1,float b1, float a2, float b2){
	ofstream out("trainFile1.csv");
	out<<"A,B,C,D"<<endl;
	Line ac(a1,b1);
	Line bd(a2,b2);
	for(int i=1;i<=100;i++){
		float a=i;
		float b=rand()%40;
		out<<a<<","<<b<<","<<ac.f(a)-0.02+(rand()%40)/100.0f<<","<<bd.f(b)-0.02+(rand()%40)/100.0f<<endl;
	}
	out.close();
}

int main() {
    srand (time(NULL));
	float a1=1+rand()%10, b1=-50+rand()%100;
	float a2=1+rand()%20 , b2=-50+rand()%100;
	generateTrainCSV(a1,b1,a2,b2);
	SimpleAnomalyDetector sa;
	TimeSeries ts("trainFile1.csv");
	sa.learnNormal(ts);
	sa.detect(ts);
    return 0;
}
