#pragma once
#include <OV2640.h>
#include <OV2640Streamer.h>
#include <CRtspSession.h>
#include <list>
#include <memory>

struct RTSP_server final
{
  RTSP_server(OV2640& cam):
    cam_{cam}
  {
    server_.begin();
  }

  RTSP_server(RTSP_server const&) = delete;
  RTSP_server& operator=(RTSP_server const&) = delete;
  RTSP_server(RTSP_server&&) = delete;
  RTSP_server& operator=(RTSP_server&&) = delete;

  static constexpr uint16_t port() { return 8554; }
  static constexpr unsigned fps() { return 10; }

  void update()
  {
    prune_dead_clients();
    handle_new_connection();
    handle_all_clients();
  }

private:
  struct RTSP_client final
  {
    RTSP_client(WiFiClient client, OV2640 &cam):
      client_{ std::move(client) },
      streamer_{&client, cam},
      session_{&client_, &streamer_}
    { }

    RTSP_client(RTSP_client const&) = delete;
    RTSP_client& operator=(RTSP_client const&) = delete;
    RTSP_client(RTSP_client&&) = delete;
    RTSP_client& operator=(RTSP_client&&) = delete;

    WiFiClient client_;
    OV2640Streamer streamer_;
    CRtspSession session_;
    //CRtspSession session_;
    //CStreamer streamer_;
  };


  void prune_dead_clients()
  {
    for(auto it=clients_.begin(); it!=clients_.end(); )
      if( not it->client_.connected() )
      {
        Serial.printf("RTSP client %s disconnected - removing\r\n", it->client_.remoteIP().toString().c_str());
        clients_.erase(it++);
      }
      else
        ++it;
  }

  void handle_new_connection()
  {
    WiFiClient client = server_.accept();
    if(not client)
      return;
    Serial.printf("new RTSP client: %s\r\n", client.remoteIP().toString().c_str());
    clients_.emplace_back(std::move(client), cam_);
  }

  void handle_all_clients()
  {
    if( clients_.empty() )
      return;

    cam_.run(); // get new frame
    for(auto& c: clients_)
      send_frame_to(c);
  }

  void send_frame_to(RTSP_client& c)
  {
    c.session_.handleRequests(0);
    //c.session_.broadcastCurrentFrame( millis() );
    c.streamer_.streamImage( millis() );
  }

  OV2640& cam_;
  WiFiServer server_{ port() };
  std::list<RTSP_client> clients_;
};
