#include <gameplay/App.h>


int main(int argc, char* argv[]) 
{ 
	auto app = gameplay::App::get_app();
	return app->exec(argc, argv);
}
