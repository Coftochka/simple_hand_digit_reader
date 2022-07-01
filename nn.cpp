#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <cstdlib>
#include <fstream>
#include <stdio.h>
#include <future>


#pragma optimize("O2");
#define uchar unsigned char

using namespace std;

vector<int> settings = {785, 17, 17, 10};

const double speed = 0.01;
const double moment = 0.005;
const int epsize = 30000;

const int gsize = 10;
const int threads = 2;

uchar data[epsize + 1][784];
uchar anses[epsize];
uchar buf[784 * epsize + 100];
default_random_engine seed;
uniform_real_distribution<double> gen(-1, 1);

double act(double x){
    return 1 / (1 + exp(-x));
}

struct neuron{
	double inp;
	double out;
	double delta;
};

void loadtests(){
	ifstream file1 ("train-images.idx3-ubyte", ios_base::binary);
	file1.read((char*)buf, 16 + epsize * 784);
	file1.close();

	for (int k = 0; k < epsize; ++k)
		for (int i = 0; i < 784; ++i)
			data[k][i] = buf[16 + k * 784 + i];

	ifstream file2 ("train-labels.idx1-ubyte", ios_base::binary);
	file2.read((char*)buf, 8 + epsize);
	file2.close();

	for (int k = 0; k < epsize; ++k)
		anses[k] = buf[k + 8];
}


struct net{
	vector<int> inp;
	neuron** a;
	double*** w;
	double*** dw;
	int n;
	net (){}

	void load(int ind){
		for (int i = 0; i < 784; ++i)
			a[0][i].inp = (data[ind][i] / 256.0 - 0.5) * 5;
	}

	void clear(){
		for (int k = 0; k < n; ++k)
			for (int i = 0; i < inp[k]; ++i)
				a[k][i].inp = a[k][i].delta = 0;
	}

    void count(int test, double* ans){
		clear();
		load(test);

		for (int k = 0; k < n - 1; ++k){
			for (int i = 0; i < inp[k] - 1; ++i){
				a[k][i].out = act(a[k][i].inp);
				for (int j = 0; j < inp[k + 1]; ++j)
					a[k + 1][j].inp += a[k][i].out * w[k][i][j];
			}
			a[k][inp[k] - 1].out = 1;
			for (int j = 0; j < inp[k + 1]; ++j)
				a[k + 1][j].inp += a[k][inp[k] - 1].out * w[k][inp[k] - 1][j];
		}
		for (int i = 0; i < inp[n - 1]; ++i)
			ans[i] = a[n - 1][i].out = act(a[n - 1][i].inp);
	}

	void rebuild(){
		for (int k = n - 2; k >= 0; --k){
			for (int i = 0; i < inp[k]; ++i){
				for (int j = 0; j < inp[k + 1]; ++j){
					a[k][i].delta += w[k][i][j] * a[k + 1][j].delta;
				}
				a[k][i].delta *=  (1 - a[k][i].out) * a[k][i].out;
			}
		}
		for (int k = 0; k < n - 1; ++k){
			for (int i = 0; i < inp[k]; ++i){
				for (int j = 0; j < inp[k + 1]; ++j){
					dw[k][i][j] = (speed * a[k + 1][j].delta * a[k][i].out + moment * dw[k][i][j]);
					w[k][i][j] += dw[k][i][j];
				}
			}
		}
	}

	void ep(){
        double err = 0;
        double ans1[10];
        for (int i = 0; i < epsize; i += 1){
            count(i, ans1);
            for (int j = 0; j < 10; ++j){
                a[n - 1][j].delta = 0;
                if (j == anses[i]){
                    a[n - 1][j].delta += (1 - ans1[j]);
                    err += (1 - ans1[j]) * (1 - ans1[j]);
                }
                else{
                    a[n - 1][j].delta += -ans1[j];
                    err += ans1[j] * ans1[j];
                }
            }
            rebuild();
        }

        cout << err / epsize << '\n';
	}

    void resize(vector<int> input){
        inp = input;
		n = inp.size();
		a = new neuron* [n];
		w = new double** [n - 1];
		dw = new double** [n - 1];

		for (int i = 0; i < n; ++i)
			a[i] = new neuron [inp[i]];

		for (int k = 0; k < n - 1; ++k){
			w[k] = new double* [inp[k]];
			dw[k] = new double* [inp[k]];
			for (int i = 0; i < inp[k]; ++i){
				w[k][i] = new double [inp[k +1]];
				dw[k][i] = new double [inp[k + 1]];
			}
		}
    }
    void rand_fill(){
        for (int k = 0; k < n - 1; ++k){
			w[k] = new double* [inp[k]];
			dw[k] = new double* [inp[k]];
			for (int i = 0; i < inp[k]; ++i){
				w[k][i] = new double [inp[k +1]];
				dw[k][i] = new double [inp[k + 1]];
				for (int j = 0; j < inp[k + 1]; ++j){
					dw[k][i][j] = 0;
					w[k][i][j] = gen(seed);
				}
			}
		}
    }

    void save(){
        ofstream fout ("AI.save", ios_base::binary);
        fout.write((char*)&n, 4);
        for(int i = 0; i < n; ++i)
            fout.write((char*)&inp[i], 4);

        for (int k = 0; k < n - 1; ++k)
            for (int i = 0; i < inp[k]; ++i)
                for (int j = 0; j < inp[k+ 1]; ++j)
                    fout.write((char*)&(w[k][i][j]), sizeof(double));
        fout.close();
    }

    void download(){
        ifstream fin ("AI.save", ios_base::binary);
        fin.read((char*)&n, 4);
        inp.resize(n);
        for (int i = 0; i < n; ++i)
            fin.read((char*)&inp[i], 4);
        resize(inp);

        for (int k = 0; k < n - 1; ++k)
            for (int i = 0; i < inp[k]; ++i)
                for (int j = 0; j < inp[k+ 1]; ++j)
                    fin.read((char*)&(w[k][i][j]), sizeof(double));

        fin.close();
    }

    void my_test(){
        ifstream file ("image.bin", ios_base::binary);
        file.read((char*)buf, 784);
        file.close();
        for (int i = 0; i < 784; ++i){
            data[epsize][i] = buf[i];
        }
    }
};




int main(){
	loadtests();
	cout << "tests are loaded\n";
	cout << "load AI (1) or generate new (2)?\n\n";
	int ans;
	cin >> ans;
	net nn;

	if (ans == 1){
        nn.download();
	}
	else{
        nn.resize(settings);
        nn.rand_fill();
	}
	cout << "how many times should it be trained?\n";
	int mmm;
	cin >> mmm;
	for (int i = 0; i < mmm; ++i){
        nn.ep();
	}
	cout << "test? (0 / 1)\n";
	cin >> mmm;
	if (mmm){
        cout << "test: ";
        double mx;
        int id;
        int corr = 0;
        for (int i = 0; i < epsize; ++i){
            double ans[10];
            nn.count(i, ans);
            mx = -2;
            for (int j = 0; j < 10; ++j){
                if (ans[j] > mx){
                    mx = ans[j];
                    id = j;
                }
            }
            if (id == anses[i]) ++corr;
        }
        cout << corr / (double) epsize;
	}

	while (1){
        cout << "is picture ready already? (to exit 0)\n";
        cin >> mmm;
        if (mmm = 0){
            nn.save();
            return 0;
        }
        nn.my_test();
        double ansans[10];
        for (int i = 0; i < 784; ++i){
            if (i % 28 == 0) cout << '\n';
            cout << ((data[epsize][i] < 128) ? (' ') : ('#')) << ' ';
        }
        cout << '\n';
        nn.count(epsize, ansans);
        double cr;
        for (int j = 0; j < 10; ++j){
            cr = ansans[j];
            if (cr < 1e-5) cr = 0;
            printf("%d :  %.10lf\n", j, cr);
        }
	}
}
