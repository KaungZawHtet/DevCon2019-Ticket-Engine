#include <string>
#include "devcon_ticket.h"

#include <CLI/CLI.hpp>




int main(int argc, char **argv) {


    CLI::App app{"Ticket Engine currently designed for DevCon 2019"};


    app.add_flag("-t,--ticket_id", ticket::is_ticket_id_displayed, "Whether display ticket id or not");

    std::string csv_file_name;
    app.add_option("-c,--csv_file", csv_file_name, "CSV file to generate tickets")
            ->check(CLI::ExistingFile);

    CLI11_PARSE(app, argc, argv);

    if(ticket::is_ticket_id_displayed ) std::cout << "Ticket id is displayed " << std::endl;
    else  std::cout << "Ticket id is not displayed " << std::endl;

    if(csv_file_name.empty()) ticket::generateRawTicket();
    else ticket::generateTicketByCSV(csv_file_name);





    return 0;






/*    if (argc == 1)generateRawTicket();
    else if (argc == 2 or argc == 3) {
        extern std::string is_ticket_id_displayed;

        is_ticket_id_displayed= argv[2];

        generateTicketByCSV(argv[1]);
    }
    else { std::cout << "Too many arguments"; }*/


}