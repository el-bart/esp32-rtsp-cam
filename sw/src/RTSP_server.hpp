#pragma once
#include <OV2640.h>
#include <OV2640Streamer.h>
#include <CRtspSession.h>
#include <list>

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

  void update()
  {
    prune_dead_clients();
    handle_new_connection();
    handle_all_clients();
  }

private:
  struct RTSP_client final
  {
    RTSP_client(WiFiClient const& client, OV2640 &cam):
      client_{client},
      streamer_{&client_, cam},
      session_{&client_, &streamer_}
    { }

    RTSP_client(RTSP_client const&) = delete;
    RTSP_client& operator=(RTSP_client const&) = delete;
    RTSP_client(RTSP_client&&) = delete;
    RTSP_client& operator=(RTSP_client&&) = delete;

    WiFiClient client_;
    OV2640Streamer streamer_;
    CRtspSession session_;
    uint32_t last_frame_time_{};
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
    clients_.emplace_back(client, cam_);
  }

  void handle_all_clients()
  {
    for(auto& c: clients_)
      send_frame_to(c);

    for(auto& c: clients_)
      c.session_.handleRequests(0);
  }

  bool is_time_for_frame(RTSP_client const& c, uint32_t now) const
  {
    // 1ms timer on 32-bit rolls over every ~50d - this prevents a freeze when this happens
    if(now < c.last_frame_time_)
      return true;

    auto constexpr inter_frame_delay_ms = 200;
    return c.last_frame_time_ + inter_frame_delay_ms <= now;
  }

  void send_frame_to(RTSP_client& c)
  {
    auto const now = millis();
    if( not is_time_for_frame(c, now) )
      return;
    Serial.printf("RTSP_server::send_frame_to(): broadcastCurrentFrame(): sending frame at %lu\r\n", now);
    c.session_.broadcastCurrentFrame(now);
    c.last_frame_time_ = now;
  }

  OV2640& cam_;
  WiFiServer server_{ port() };
  std::list<RTSP_client> clients_;
};
