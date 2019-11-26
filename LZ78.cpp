#include <iostream>
#include <fstream>
#include <sstream>
#include <vector>
#include <cstdlib>
#include <iterator>
#include <string>
#include <algorithm>

using namespace std;



struct dict {
	int id;
	std::string s;
};


//outputs are sent to the receiver (outputs represent compressed data).
struct output {
	int id; 
	uint8_t ch;
};


class functorDict{

private:        
    std::string aux;

public:
    functorDict(std::string s): aux (s){};

    bool operator()(const dict &d) const{
        return d.s == aux;
    }

};


void compress(std::vector<uint8_t> &v) {
	vector<dict> dicts;   
	vector<output> outs;	
	
	ostringstream ss;
	string aux;		// Used in order to "convert" a std::ostringstream to std::string 
	
	// We will use it to set "id" field of "dict" struct (aka, dict.id)
	int id = 0;
	
	//We will use it to set "id"  field of "output" struct (aka, output.id)
	int id_aux = 0;
	
	for (unsigned i = 0 ; i < v.size()-1 ; i++) {
		ss << v[i];
		aux = ss.str();
		
		// Find if the aux string is already present in any instance of dicts
		auto it = find_if(dicts.begin(), dicts.end(), functorDict(aux));
		
		// If not so:
		if (it == dicts.end()) {
			id++;
			dicts.push_back({id, aux});
			if (aux.size() > 1) {			
				outs.push_back({ id_aux, v[i] });
				ss.str(""); //clear ostringstream
				continue;
			}
			else
				outs.push_back({ 0, v[i]});
			ss.str("");

		}
		// If so:
		else {
			id_aux = it->id;
			continue;
		}
	}

    

	for (const auto &x : dicts) {
		cout << " Dizionario: " << x.id << " = " << x.s << "\n";
	}

	for (const auto &y : outs) {
		cout << "Output: (" << y.id << ", " << y.ch << ")\n";
	}

}


int main(int argc, char*argv[]) {
	

	// Open a file (the filename is the first parameter of this program) in binary mode...
	ifstream is(argv[1], ios::binary);
	if (!is)
		return EXIT_FAILURE;
	
	//...and unset the flag "skipws" (because we want to read also bytes that represent a whitespace :) )
	is.unsetf(ios::skipws);

	//a fast method in order to read all bytes of a input file and put them into a vector
	vector<uint8_t> v = { istream_iterator<uint8_t> {is}, istream_iterator<uint8_t>{} };

	compress(v);

	return EXIT_SUCCESS;
}
