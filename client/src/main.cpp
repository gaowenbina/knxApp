/* 
 * File:   main.cpp
 * Author: Joshua Johannson
 *
 * main
 */

// include
#include <unistd.h>
#include <string>
#include <sstream>
#include <stdlib.h>
#include <iostream>
#include <Ui.h>
#include <Text.h>
#include <Box.h>
#include <Style.h>
#include "../../protocol/src/PacketFactory.h"
#include "../../protocol/src/Packet.h"
#include "../../protocol/src/Client.h"


using namespace std;


// var
const string serverIp = "192.168.1.164";
const int serverPort = 3671;

// ui
Ui *ui;
Box *root;
Text *headline;
Text *out;


/*
 * main
 */
int main(int argc, char **argv)
{
    // start
    cout << "[ OK ] start program" << endl;

    // init ui
    Ui::init();


    // ui components
    root     = new Box();
    headline = new Text();
    out      = new Text();

    headline->text("Knx");
    root->addChild(headline);
    root->addChild(out);

    // style
    Style::getRule("*")->textSize->set(50);

    // in android other font path
    #ifdef pl_andr
    Style::getRule("*")->textFamily->set("FreeSans.ttf");
    #endif


    // open screen
    ui = new Ui(500, 600, "Knx Client");
    ui->setRootView(root);
    ui->frameRenderer->start();


    // setup home server connection
    home::Client *client = new home::Client("192.168.1.163", 5049);
    client->onError([&](string msg)
    {
        cout << msg << endl;
        out->text(out->text() + msg + "\n");
    });
    client->onReceive([&](home::Packet *packet)
    {
        cout << "[CLIE] receive packet '" << packet->type << "'" << endl;

        // cast to knx packet
        home::KnxPacket *clientPacket = ((home::KnxPacket*)packet);

        // if knx packet
        if (clientPacket)
        {
            // get data from home packet
            int main, middle, sub,
                    area, groupe, line;
            clientPacket->getDestinationAddr(main, middle, sub);
            clientPacket->getSourcAddr(area, groupe, line);

            out->text(out->text() + "[GET ] src: " + to_string(main) + "/" + to_string(middle) + "/" + to_string(sub) + "\n");
        }
    });

    // init
    client->init();

    // connect
    client->connectToServer();



    // not close programm
    while (true)
    {
        sleep(100000);
    }
}