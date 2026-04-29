#include <gtkmm/application.h>
#include "gui.h"

using namespace std;

int main(int argc, char *argv[])
{
    string file_name("");
    if (argc > 1)
    {
        file_name = argv[1];
    }
    auto app = Gtk::Application::create();
    return app->make_window_and_run<My_window>(1, argv, file_name);
}