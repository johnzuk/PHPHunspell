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

    Php::Value getEncoding() const
    {
    	return encoding;
    }

    void __construct(Php::Parameters &params)
	{
    	const char * aff_path = params[0];
    	const char * dic_path = params[1];

    	if (!file_exists(aff_path)) {
    		throw Php::Exception("Aff file does not found");
    	}

    	if (!file_exists(dic_path)) {
    		throw Php::Exception("Dic file does not found");
    	}

    	handle = Hunspell_create(aff_path, dic_path);

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

    	const char * word = params[0];
    	int num_slist = Hunspell_stem(handle, &slist, word);

    	for (int i = 0; i < num_slist; i++) {
    		result[i] = slist[i];
    	}

    	Hunspell_free_list(handle, &slist, num_slist);
    	return result;
    }

    Php::Value suggest(Php::Parameters &params)
    {
    	Php::Value result;
    	char **slist;

    	const char * word = params[0];
    	int num_slist = Hunspell_suggest(handle, &slist, word);

    	for (int i = 0; i < num_slist; i++) {
			result[i] = slist[i];
		}

		Hunspell_free_list(handle, &slist, num_slist);
		return result;
    }

    Php::Value analyze(Php::Parameters &params)
    {
    	Php::Value result;
		char **slist;

		const char * word = params[0];
		int num_slist = Hunspell_analyze(handle, &slist, word);

		for (int i = 0; i < num_slist; i++) {
			result[i] = slist[i];
		}

		Hunspell_free_list(handle, &slist, num_slist);
		return result;
    }

    Php::Value add(Php::Parameters &params)
    {
    	const char * word = params[0];
    	return Hunspell_add(handle, word);
    }

    Php::Value remove(Php::Parameters &params)
    {
    	const char * word = params[0];
    	return Hunspell_remove(handle, word);
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

		hunspell.method<&Hunspell::stem>("stem", {
				Php::ByVal("word", Php::Type::String)
		});

		hunspell.method<&Hunspell::suggest>("suggest", {
				Php::ByVal("word", Php::Type::String)
		});

		hunspell.method<&Hunspell::analyze>("analyze", {
				Php::ByVal("word", Php::Type::String)
		});

		hunspell.method<&Hunspell::add>("add", {
				Php::ByVal("word", Php::Type::String)
		});

		hunspell.method<&Hunspell::remove>("remove", {
				Php::ByVal("word", Php::Type::String)
		});

		hunspell.property("encoding", &Hunspell::getEncoding);

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

