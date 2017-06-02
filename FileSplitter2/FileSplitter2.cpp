// FileSplitter2.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <cstdio>
#include <memory>
#include <fstream>
#include <string>
#include <fcntl.h>  
#include <io.h> 
#include <iostream>

using namespace std;

typedef uintmax_t BIGINTEGER;

void split(const char*, const char*, const BIGINTEGER&,const size_t&);
void split(istream&, const char*, const BIGINTEGER&, const size_t&);
void make_part(istream&, ofstream&,const BIGINTEGER&,const BIGINTEGER&,const size_t&);
void make_part(istream&, ofstream& , const BIGINTEGER& , const size_t& );
void streamcat(ofstream&,ifstream&,const size_t&);
void join(const char *, const char *, const size_t&);

int main()
{
	try {		
		_setmode(_fileno(stdin), _O_BINARY);
		split(cin, "test.part", 1024 * 1024 * 64, 1024 * 1024 * 1);
		
		split("D:\\characters.vpp_pc", ".part", 1024 * 1024 * 64, 1024 * 1024 * 1);
		printf("\nJoining Files Back:\n");
		join("D:\\out", "D:\\characters.vpp_pc.part", 1024 * 1024 * 1);
		return 0;			
	}
	catch (const char * e) {
		printf("An error occured: %s\n", e);
	}
	catch ( exception& e) {
		printf("An error occured: %s\n", e.what());
	}
	catch (...) {
		printf("An Unknown error occurred\n");
	}
}

inline void split(const char * infilename, const char * suffix, const BIGINTEGER & chunksize, const size_t& bufsize)
{
	ifstream split_this(infilename, ios::binary);
	if (!split_this.is_open()) {
		throw "Can't open input file";
	}
	split(split_this, (string(infilename) + suffix).c_str(), chunksize, bufsize);
}

void split(istream& split_this, const char* outfilename, const BIGINTEGER& chunksize, const size_t& bufsize) {
	static ofstream part;
	static unsigned counter;
	counter = 1;
	while (!split_this.eof()) {
		printf("Spliting %i\n", counter);
		part.open(string(outfilename) + to_string(counter++), ios::binary);
		if (!part.is_open())
			throw "Can't open a part to write splitted data";
		make_part(split_this, part, chunksize, bufsize);
		part.close();
	}
}

inline void make_part(istream& file_to_split, ofstream& write_to, const BIGINTEGER& startpos, const BIGINTEGER& splitsize,const unsigned int& bufsize)
{
	file_to_split.seekg(startpos);
	make_part(file_to_split, write_to, splitsize, bufsize);
}

void make_part(istream& file_to_split, ofstream& write_to, const BIGINTEGER& splitsize, const unsigned int& bufsize)
{
	static BIGINTEGER readed;
	readed = 0;
	static unique_ptr<char[]> buffer{ new char[bufsize] };
	while (readed < splitsize) {
		file_to_split.read(buffer.get(), bufsize);
		write_to.write(buffer.get(), file_to_split.gcount());
		readed += bufsize;
	}
	printf("Finished splitting\n");
}

void streamcat(ofstream& Destination, ifstream& Source, const size_t& bufsize)
{
	static unique_ptr<char[]> buffer{ new char[bufsize] };
	while (!Source.eof())
	{
		Source.read(buffer.get(), bufsize);
		Destination.write(buffer.get(), Source.gcount());
	}
	printf("Finish Merging \n");
}

void join(const char * outfilename, const char * splitted_files, const size_t& bufsize)
{
	ofstream Destination(outfilename, ios::binary);
	if (!Destination.is_open())
		throw "can't open Destination file";
	static unsigned counter ;
	counter = 1;
	ifstream Source(string(splitted_files) + to_string(counter++), ios::binary);
	while (Source.is_open())
	{
		printf("Merging part%i\n", counter-1);
		streamcat(Destination, Source, bufsize);
		Source.close();
		Source.open(string(splitted_files) + to_string(counter++), ios::binary);
	}
} 
