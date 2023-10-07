#include <iostream>
#include <fstream>
#include <regex>
#include <chrono>
#include <vector>
#include "HashMap.hh"

template<typename iterator>
size_t count (iterator begin, iterator end){
	size_t size=0;
	while(begin!=end){
		++begin;
		size++;
	}
	return size;
}

template<typename iterator,typename T>
size_t count_if_equal (iterator begin, iterator end, const T & val){
	size_t size=0;
		while(begin!=end){
			if (*begin==val){
				size++;
			}
			++begin;
		}
		return size;
}

int main () {
	using namespace std;
	using namespace std::chrono;
	using namespace pr;

	ifstream input = ifstream("WarAndPeace.txt");

	auto start = steady_clock::now();
	cout << "Parsing War and Peace" << endl;

	size_t nombre_lu = 0;

	string word;	// prochain mot lu
	regex re( R"([^a-zA-Z])");  // une regex qui reconnait les caractères anormaux (négation des lettres)
	//Question 3
	vector <pair<string,int>> v; 
	HashMap<string, int> m(20500);

	while (input >> word) {

		word = regex_replace ( word, re, "");   // élimine la ponctuation et les caractères spéciaux
		transform(word.begin(),word.end(),word.begin(),::tolower);	// passe en lowercase
		// word est maintenant "tout propre"
		if (nombre_lu % 100 == 0)
			// on affiche un mot "propre" sur 100
			cout << nombre_lu << ": "<< word << endl;
		nombre_lu++;
		
		//Question 3
		/*bool egale = false;
		for (int i = 0; i < v.size(); i++){
			if (word == v[i].first) { 
				v[i].second++;
				egale=true;
				break;
			}
		}

		if (egale == false)
			v.push_back({word,1});*/

		//Question 5
		
		int* value = m.get(word);
		
		if (value == nullptr){
			m.put(word,1); // BUG
		}else{
			m.put(word, (*value)+1);
		}

	}

	
	input.close();

	cout << "Finished Parsing War and Peace" << endl;

	auto end = steady_clock::now();
    cout << "Parsing took "
              << duration_cast<milliseconds>(end - start).count()
              << "ms.\n";
	/*
	//Qustion 2 & 3
	cout << "Found a total of " << v.size() << " differents words." << endl;
	for (size_t i = 0; i < v.size(); i++){
		if (v[i].first=="war" || v[i].first=="peace" || v[i].first=="toto")
		{
			cout << v[i].first << " : "<< v[i].second << endl;
		}
		
	}
	*/

	//Question 5
	/*
	cout << "war : "<< map.get("war")<< endl;
	cout << "peace : "<< map.get("peace")<< endl;
    cout << "Found a total of " << nombre_lu << " words." << endl;
	*/
    return 0;

}


