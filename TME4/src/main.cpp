#include "Banque.h"
#include <random>
#include <iostream>

using namespace std;
using namespace pr;

const int NB_THREAD = 10;
const int SOLDEINITIAL = 100;

int transfertWork(Banque &b){
	for (int cpt = 0; cpt<1000;cpt++){
		int i= rand() % 100 + 1;
		int j= rand() % 100 + 1;
		int montant= rand() % 100 + 1;
		b.transfert(i, j, montant);
		int slp = rand() % 20;
		this_thread::sleep_for (chrono::milliseconds(slp));
	}
	return 1;
}


int main () {
	// TODO : creer des threads qui font ce qui est demandé
	Banque b = Banque(100,SOLDEINITIAL);
	vector<thread> threads;
	threads.reserve(NB_THREAD);
	for (int cpt = 0; cpt<NB_THREAD;cpt++){
		threads.emplace_back(transfertWork,ref(b));
	}

	for (auto & t : threads) {
		t.join();
	}

	// TODO : tester solde = NB_THREAD * JP
	::srand(::time(nullptr));
	return 0;
}
