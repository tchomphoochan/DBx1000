// defined in system/parser.cpp
void parser(int argc, char * argv[]);

// defined in system/run.cpp
int run();

int main(int argc, char* argv[])
{
	parser(argc, argv);

	return run();
}
