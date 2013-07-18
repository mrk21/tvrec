#pragma once
#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <functional>
#include <memory>
#include <boost/format.hpp>
#include <tssplitter/ts/packet.hpp>
#include <tssplitter/filename.hpp>

namespace tssplitter { namespace ts {
  class splitter {
    constexpr static auto IO_BUFFER_SIZE = ts::packet::SIZE * 1024 * 10; // 1.88MB
    
    filename ipath;
    filename opath;
    std::ifstream ifs;
    std::ofstream ofs;
    std::shared_ptr<char> ifs_buffer;
    std::shared_ptr<char> ofs_buffer;
    
    uint32_t count;
    uint32_t video_pid;
    uint32_t audio_pid;
    
    enum pmt_status_type {
      NO_CHANGE = 0,
      FIRST     = 1,
      CHANGE    = 2,
    };
    
  public:
    splitter(std::string ipath, std::string opath);
    int execute(void);
    
  private:
    uint32_t get_pmt_pid(void);
    void find_pmt(uint32_t pid, std::function<void(psi::pmt *)> callback);
    uint32_t get_pmt_status(void);
    
    void wait_stream(std::function<bool(void)> callback);
    void each_packet(std::function<bool(ts::packet &)> callback);
    void read_packet(ts::packet & packet);
    void write_packet(ts::packet & packet);
    bool open_istream(void);
    bool open_ostream(void);
  };
}}
