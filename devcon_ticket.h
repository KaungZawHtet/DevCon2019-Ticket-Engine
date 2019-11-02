//
// Created by Kaung Zaw Htet on 2019-10-30.
//

#ifndef DEVCON_TICKET_DEVCON_TICKET_H
#define DEVCON_TICKET_DEVCON_TICKET_H

#include <string>
#include "csv_writer.h"

namespace ticket {


    inline std::string id;
    inline std::string ticket_id;

    inline std::string user_id;
    inline std::string created_at;
    inline std::string updated_at;
    inline std::string sale_online;

    inline std::string qr_filename_svg;
    inline std::string qr_filename_png;


    inline bool is_ticket_id_displayed;


    void generateID();

    void generateQRCode(std::string key);

    int mergeQRCodeNtemplate(std::string key);

    void writeCSV();

    void generateRawTicket();

    void generateTicketByCSV(std::string csv_file_name);

}


#endif //DEVCON_TICKET_DEVCON_TICKET_H
