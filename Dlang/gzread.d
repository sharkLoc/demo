import std.stdio;
import std.process;
import std.string;

void main(string[] args){

	string fileName = args[1];
	string command = "gzip -dc " ~ fileName ;
	auto dmd = executeShell(command);
    
	if(dmd.status != 0){
		writeln("Compilation failed:\n", dmd.output);
	}
	else{
		auto all=chomp(dmd.output).split("\n");
	    writeln(typeid(all));
	    for(int i=0; i<all.length; i++){
		    writeln(all[i]);
		}
	}

}
