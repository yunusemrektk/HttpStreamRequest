#ifndef CONFIG_H
#define CONFIG_H
#include "string" //c++
#include <QString>
//qt
class Config{

public:
    std::string npmPort="9500"; //hls
    int postmanPort = 10010;  //request
    std::string outputNamem3u8="index"; //hlsoutput
    std::string outputType=".m3u8"; //hls
    QString file264path ="/home/emre/Documents/live555-latest/live/mediaServer/"; //file to rtsp
    std::string m3u8outpath="/home/emre/Documents/live555-latest/live/hlsProxy/";// rtsp to hls
    std::string live555mediaServerPath = "/home/emre/Documents/live555-latest/live/mediaServer/"; // live555MediaServer
    std::string npmModuleHttpServerPath="/home/emre/Documents/node_modules/http-server/bin/";   // Npm Server Module
    std::string live555HLSProxyPath="/home/emre/Documents/live555-latest/live/hlsProxy/";
    std::string localFileSharePath="/home/emre/Documents/live555-latest/live/hlsProxy/"; // Share local files
    std::string ngrokPath="/home/emre/Documents";
    std::string RtspServerLink = "rtsp://10.0.2.15:8554/mytest.264";
};

#endif // CONFIG_H
