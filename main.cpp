#include <QCoreApplication>
#include <crow_all.h>
#include <QProcess>
#include <QString>

#include <sys/stat.h>
#include <string>
#include <iostream>
#include <stdlib.h>
#include <QFileInfo>
#include <QDir>
#include <QTime>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonValue>
#include <QJsonArray>
#include "Config.h"

using namespace crow;
using namespace std;

void RunShellScript();
void createRandomFolder();
void getNgrokUrl();
void createhttpserver();
void openlive555();
void openNgrok();
void convert264tom3u8();
void convertffmpeg();

string output;
QString ngrokUrl;
QString randomString;
Config cfg;

int main(int argc, char *argv[])
{

    QCoreApplication a(argc, argv);
    SimpleApp app;

    std::thread live555(openlive555);   //mediaserver -> file to rtsp
    live555.detach();

    std::thread httpserver(createhttpserver);   // npm
    httpserver.detach();

    std::thread ngrok(openNgrok);   //hls stream ngrok
    ngrok.detach();

    CROW_ROUTE(app, "/Start")
    .methods("POST"_method)
    ([](const request& req){
        auto x = crow::json::load(req.body);

        if (!x)
            return crow::response(400);

        QString fileName = QString::fromStdString(x["Rtsp"].s());
        QString portNum = QString::fromStdString(x["Port"].s());

        //Run command

        if(QFileInfo(cfg.file264path+fileName).exists() && x["Port"].s()==cfg.npmPort){
           createRandomFolder();
           /* std::thread convert(convert264tom3u8);
            convert.detach();
            sleep(3);
            //Sleep until the m3u8 file is generated

            while(QFileInfo(QString::fromStdString(cfg.m3u8outpath)+"/index.m3u8").exists()==false){
                sleep(1);
            }
*/
            getNgrokUrl();
            output="{\"HlsUrl\":\""+ngrokUrl.toStdString()+"/"+cfg.outputNamem3u8+cfg.outputType+"\"}";
            randomString="";
        }

        else{

            output= "Check the Url and Port!";
        }



        std::ostringstream os;
        os << output;

        return response(200,os.str());
    });

    app.port(cfg.postmanPort).multithreaded().run();

    return a.exec();
}

void createhttpserver(){
    std::string command = "cd "+cfg.npmModuleHttpServerPath+" && ./http-server -p "+cfg.npmPort+" "+cfg.localFileSharePath;
    std::system(command.c_str());
}

void openlive555(){
    std::string command = "cd "+cfg.live555mediaServerPath+" && ./live555MediaServer";
    std::system(command.c_str());

}

void openNgrok(){
    std::string command = "cd "+cfg.ngrokPath+" && ./ngrok http "+cfg.npmPort;
    std::system(command.c_str());
}

void convert264tom3u8(){

    std::string command = "cd "+cfg.live555HLSProxyPath+" &&./live555HLSProxy -t "+cfg.RtspServerLink+" "+cfg.outputNamem3u8;
    std::system(command.c_str());
}
/*
void convertffmpeg(){
    std::string command = "ffmpeg -i rtsp://10.0.2.15:8554/mytest.264 -acodec copy -bsf:a aac_adtstoasc -vcodec copy /home/emre/Documents/live555-latest/live/hlsProxy/index.m3u8";
    std::system(command.c_str());
}
*/
void createRandomFolder(){

   /* const QString possibleCharacters("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789");
    const int randomStringLength = 10; // assuming you want random strings of 12 characters


    srand(time(NULL));
    for(int i=0; i<randomStringLength; ++i)
    {

        int index = rand() % possibleCharacters.length();
        QChar nextChar = possibleCharacters.at(index);
        randomString.append(nextChar);
    }

    QDir().mkdir(QString::fromStdString(cfg.m3u8outpath)+randomString);*/
/*
    std::thread convert(convertffmpeg);
    convert.detach();
*/

    std::thread convert(convert264tom3u8);
    convert.detach();

    sleep(3);

    //Sleep until the m3u8 file is generated

    while(QFileInfo(QString::fromStdString(cfg.m3u8outpath)+"/"+QString::fromStdString(cfg.outputNamem3u8+cfg.outputType)).exists()==false){
        sleep(1);
    }


}

void getNgrokUrl(){
    QProcess process;
    process.execute("/bin/sh",QStringList()<<"-c"<<"curl http://localhost:4040/api/tunnels >tunnels.json");

    QString val;
    QFile file;
    QStringList urlNames;

    file.setFileName("tunnels.json");
    file.open(QIODevice::ReadOnly | QIODevice::Text);
    val = file.readAll();
    file.close();

    QJsonDocument doc = QJsonDocument::fromJson(val.toUtf8());
    QJsonObject obj = doc.object();
    QJsonArray jsArray = obj["tunnels"].toArray();

    foreach (const QJsonValue & value, jsArray) {
        QJsonObject obj = value.toObject();
        urlNames.append(obj["public_url"].toString());

    }
    ngrokUrl= urlNames.takeAt(0);

}
