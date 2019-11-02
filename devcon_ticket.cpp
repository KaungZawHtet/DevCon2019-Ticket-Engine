//
// Created by Kaung Zaw Htet on 2019-10-30.
//

#include "devcon_ticket.h"

#include <iostream>
#include "csv_writer.h"
#include <random>
#include "qr/QrCode.hpp"// chrono::system_clock
#include <ctime>
#include <iomanip>
#include <opencv2/opencv.hpp>
#include <Magick++.h>
#include <fstream>
#include <stdlib.h>
#include <string>

using namespace qrcodegen;
using namespace cv;


void ticket::generateID() {
    mt19937 generatorObject(static_cast<unsigned int>(time(nullptr)));

    std::uniform_int_distribution<long> idDistribution(8000000000, 8999999999);
    long generatedID = idDistribution(generatorObject);
    ostringstream strStream;
    strStream << generatedID;

    ticket::id = "DC" + strStream.str();

}

void ticket::generateQRCode(std::string key) {
    std::cout << key << '\n';
    QrCode qr0 = QrCode::encodeText(key.c_str(), QrCode::Ecc::HIGH);
    std::string svg = qr0.toSvgString(4);
    fstream file;
    ticket::qr_filename_svg = key + ".svg";
    file.open(ticket::qr_filename_svg, ios::out);
    file << svg;
    file.close();

    ticket::qr_filename_png = key + ".png";
    std::string commend = "./convert -density 450 " + ticket::qr_filename_svg + " " + ticket::qr_filename_png;
    system(commend.c_str());

    /*Magick::Geometry geometry(135,135,0,0);
    Magick::ColorspaceType sp();

    Magick::Image svgImage(qr_filename_svg);


   // svgImage.fillColor(Magick::Color(0,0,0));
   // svgImage.backgroundColor(Magick::Color(255,255,255));
    svgImage.magick("png");

    svgImage.write(qr_filename_png);*/




}

int ticket::mergeQRCodeNtemplate(std::string key) {

    Mat ticket_template, qr_image;
    ticket_template = imread("lib/workspace_template.jpg", 1);
    qr_image = imread(ticket::qr_filename_png, 1);
    if (!ticket_template.data) {
        cout << "Error loading src1" << endl;
        return -1;
    }
    if (!qr_image.data) {
        cout << "Error loading src2" << endl;
        return -1;
    }

    qr_image.copyTo(ticket_template(cv::Rect(1000, 400, qr_image.cols, qr_image.rows)));
    qr_image.copyTo(ticket_template(cv::Rect(1450, 300, qr_image.cols, qr_image.rows)));


    if (ticket::is_ticket_id_displayed) {
        putText(ticket_template, key, Point(1000, 590),
                FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(255, 255, 255), 2);
        putText(ticket_template, key, Point(1450, 265),
                FONT_HERSHEY_COMPLEX_SMALL, 1, Scalar(255, 255, 255), 2);
    }


    std::string ticket = key + ".jpg";
    imwrite(ticket, ticket_template);

    return 0;


}

void ticket::writeCSV() {

    auto now = std::chrono::system_clock::now();
    auto in_time_t = std::chrono::system_clock::to_time_t(now);

    std::stringstream ss;
    ss << std::put_time(std::localtime(&in_time_t), "%Y-%m-%d %H:%M:%S");


    string line;

    int id_count[2]; //user first, ticket second


    ifstream file_in("lib/persistence.txt");
    if (file_in.is_open()) {

        int i = 0;
        while (getline(file_in, line)) {
            id_count[i] = stoi(line);
            i++;
        }
        file_in.close();
    }

    CSVWriter csv;
    csv << id_count[0] << id_count[1] << "NULL" << ticket::id << "0" << ss.str() << ss.str();
    csv.writeToFile("ticket_id.csv", true);


    ofstream file_out;
    file_out.open("lib/persistence.txt", ios::out);
    file_out << ++id_count[0] << '\n' << ++id_count[1];
    file_out.close();
}

void ticket::generateRawTicket() {
    ticket::generateID();
    ticket::generateQRCode(ticket::id);
    ticket::writeCSV();
    int err_flag = ticket::mergeQRCodeNtemplate(ticket::id);
    if (err_flag == -1) exit(err_flag);

    std::remove(ticket::qr_filename_svg.c_str());
    std::remove(ticket::qr_filename_png.c_str());
}


void ticket::generateTicketByCSV(std::string csv_file_name) {

    std::ifstream data(csv_file_name);
    // std::cout<<csv_file_name;
    std::string line;

    while (std::getline(data, line)) {
        int i = 0;
        std::stringstream lineStream(line);
        std::string cell;
        std::string parsedRow[6];
        while (std::getline(lineStream, cell, ',')) {
            std::string actual_data(cell.begin() + 1, cell.end() - 1);

            parsedRow[i] = actual_data;
            // std::cout<<actual_data<<',';
            i++;
        }
        ticket::ticket_id = parsedRow[2];


        ticket::generateQRCode(ticket::ticket_id);
        int err_flag = ticket::mergeQRCodeNtemplate(ticket::ticket_id);
        if (err_flag == -1) exit(err_flag);

        std::remove(ticket::qr_filename_svg.c_str());
        std::remove(ticket::qr_filename_png.c_str());


    }
}