#include <stdio.h>
#include <string>
#include <cstring>
#include <vector>
#include <algorithm>

using namespace std;

struct Entry
{
    std::string name;
    std::vector<std::string> dependencies;
    std::string recipe;
};

std::string replace(std::string subject, const std::string& search,
                          const std::string& replace) {
    size_t pos = 0;
    while ((pos = subject.find(search, pos)) != std::string::npos) {
         subject.replace(pos, search.length(), replace);
         pos += replace.length();
    }
    return subject;
}

std::string strip(std::string str)
{
    //str.erase(std::remove(str.begin(), str.end(), '\n'), str.end());
    //return str;
	return replace(str, "\n", "");
}


void readRecipe(FILE* pipe, char buffer[], size_t bufferSize, Entry &entry)
{
    
    do
    {
        std::string line = strip(std::string(buffer));
        if(!line.length()) 
        {
//            printf("end\n");
            return;
        }
        if(line[0] == '#') continue;
        if(line[0] == '\t')
        {
			if(entry.recipe.size()) entry.recipe += "\n";
            entry.recipe += std::string("    ") + line.substr(1);
            continue;
        }
        if(line.find(':') != string::npos)
        {
            char* first = strtok(buffer, ":");
            entry.name = std::string(first);
            while(char* dependencies = strtok(NULL, " \n"))
            {
				//printf("!%s!\n", dependencies);
                entry.dependencies.push_back(std::string(dependencies));
            }
        }
    }while((fgets(buffer, bufferSize, pipe)));
}

std::string nodeName(std::string a_name)
{
	for(int i=0;i<a_name.size();i++)
	{
		if(!isalnum(a_name[i])) a_name[i] = '_';
	}
	return a_name;
}

std::string makeDisplay(const Entry& entry)
{
	std::string display = std::string("{") + entry.name + " | ";
	std::string recipe = replace(entry.recipe, "\n", "\\n");
	recipe = replace(recipe, "&", "&amp;");
	recipe = replace(recipe, "<", "&lt;");
	recipe = replace(recipe, ">", "&gt;");
	recipe = replace(recipe, "'", "&apos;");
	recipe = replace(recipe, "\"", "&quot;");
	recipe = replace(recipe, "|", "\\|");
	display += recipe + "}";
	return display;
}

int main(int argc, char* argv[])
{
	std::string command;
	for(int c = 1; c < argc; c++)
	{
		command+= std::string(argv[c]) + " ";
	}
	command+=std::string(" -qprR | grep -A999999 \"# Files\"");
   
   fprintf(stderr, "Command: %s\n", command.c_str());
	
    std::string line;
    FILE* inPipe = popen(command.c_str(), "r");
    char buffer[1234];
	std::vector<Entry> entries;
    while(fgets(buffer, sizeof(buffer), inPipe))
    {
        line = std::string(buffer);
        if(line[0] == '#') continue;
        if(line.length())
        {
//            printf("woooh\n");
			Entry entry;
            readRecipe(inPipe, buffer, sizeof(buffer), entry);
            if(entry.name == "") continue;
            if(entry.name == ".PHONY") continue;
            if(entry.name == ".SUFFIXES") continue;
            if(entry.name == ".DEFAULT") continue;
			entries.push_back(entry);
            //printf("%s <- ", entry.name.c_str());
            for(int i = 0;i<entry.dependencies.size();++i)
            {
                //printf("%s ", entry.dependencies[i].c_str());                
            }
            //printf("\n%s\n\n", entry.recipe.c_str());
            entry = Entry();
        }        
    }
    pclose(inPipe);
	
	//FILE* outFile = fopen("out.dot", "w");
	
	FILE* outFile = stdout;
	
	fprintf(outFile, "graph Makefile {\n");
	fprintf(outFile, "   edge[dir=forward];\n");
	
	for(int i=0;i<entries.size();i++)
	{
		const Entry& entry = entries[i];
		std::string display = makeDisplay(entry);
		fprintf(outFile, "   %s [label=\"%s\" shape=\"record\"];\n", nodeName(entry.name).c_str(), display.c_str());
	}
	fprintf(outFile, "   \n");

	for(int i=0;i<entries.size();i++)
	{
		const Entry& entry = entries[i];
		std::string entryName = nodeName(entry.name);
		for(int depIdx = 0; depIdx < entry.dependencies.size(); depIdx++)
		{
			fprintf(outFile, "   %s -- %s;\n", nodeName(entry.dependencies[depIdx]).c_str(), entryName.c_str());
		}
	}
	
	
	fprintf(outFile, "}\n");
	fflush(outFile);
	//fclose(outFile);
	
    return 0;
}