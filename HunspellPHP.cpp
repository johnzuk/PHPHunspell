#include <iostream>
#include <hunspell/hunspell.h>
#include <phpcpp.h>
#include <stdio.h>

using namespace std;

inline bool file_exists(const std::string& name) {
    if (FILE *file = fopen(name.c_str(), "r")) {
        fclose(file);
        return true;
    } else {
        return false;
    }
}

class Hunspell : public Php::Base {
private:
	Hunhandle * handle = nullptr;
	const char * encoding = nullptr;
public:
	Hunspell() = default;
    virtual ~Hunspell() = default;

    void __construct(Php::Parameters &params)
	{
    	string aff_path = params[0].stringValue();
    	string dic_path = params[1].stringValue();
    	if (!file_exists(aff_path)) {
    		throw Php::Exception("Aff file does not found");
    	}

    	if (!file_exists(dic_path)) {
    		throw Php::Exception("Dic file does not found");
    	}

    	handle = Hunspell_create(aff_path.c_str(), dic_path.c_str());

    	if (!handle) {
    		throw Php::Exception("Cannot open dictionary");
    	}

    	encoding = Hunspell_get_dic_encoding(handle);
	}

    Php::Value spell(Php::Parameters &params)
    {
    	const char * word = params[0];

    	return (bool)Hunspell_spell(handle, word);
    }

    Php::Value stem(Php::Parameters &params)
    {
    	Php::Value result;
    	char **slist;

    	const char * word = params[0].stringValue().c_str();
    	int num_slist = Hunspell_stem(handle, &slist, word);

    	for (int i = 0; i < num_slist; i++) {
    		result[i] = slist[i];
    	}

    	return result;
    }
};

extern "C" {

	PHPCPP_EXPORT void *get_module()
	{
		// create extension
		static Php::Extension HunspellExtension("hunspellphp", "1.0");
		Php::Class<Hunspell> hunspell("Hunspell");

		hunspell.method<&Hunspell::__construct>("__construct", {
				Php::ByVal("affFilePath", Php::Type::String),
				Php::ByVal("dicFilePath", Php::Type::String)
		});

		hunspell.method<&Hunspell::spell>("spell", {
				Php::ByVal("word", Php::Type::String)
		});

		HunspellExtension.add(std::move(hunspell));
		return HunspellExtension.module();
	}

	/*int main() {
		const char * aff_path = "/usr/share/hunspell/pl_PL.aff";
		const char * dic_path = "/usr/share/hunspell/pl_PL.dic";
		//Hunhandle *handle = Hunspell_create(aff_path, dic_path);

		cout << "" << endl; // prints
		return 0;
	}*/

}

