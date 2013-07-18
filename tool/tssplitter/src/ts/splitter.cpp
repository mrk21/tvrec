#include <tssplitter/ts/splitter.hpp>

namespace tssplitter { namespace ts {
  splitter::splitter(std::string ipath, std::string opath) :
    ipath(ipath), opath(opath), count(0), video_pid(0), audio_pid(0),
    ifs_buffer(new char[IO_BUFFER_SIZE]), ofs_buffer(new char[IO_BUFFER_SIZE])
  {
    this->ifs.rdbuf()->pubsetbuf(this->ifs_buffer.get(), IO_BUFFER_SIZE);
    this->ofs.rdbuf()->pubsetbuf(this->ofs_buffer.get(), IO_BUFFER_SIZE);
  }
  
  int splitter::execute(void) {
    if (!this->open_istream()) return 1;
    if (!this->open_ostream()) return 1;
    
    auto status = 0;
    auto display = [this](){
      std::cerr << boost::format("video pid: 0x%0x, audio pid: 0x%0x") % this->video_pid % this->audio_pid << std::endl;
    };
    
    this->wait_stream([this, &status, &display](){
      switch (this->get_pmt_status()) {
        case pmt_status_type::FIRST:
          display();
          break;
          
        case pmt_status_type::CHANGE:
          display();
          if (!this->open_ostream()) {
            status = 1;
            return true;
          }
          break;
      }
      return false;
    });
    
    return status;
  }
  
// private
  uint32_t splitter::get_pmt_pid(void) {
    uint32_t pid = 0;
    
    this->each_packet([&](ts::packet & packet){
      if (packet.header.pid != 0x00 || packet.header.payload_unit_start_indicator != 1) return false;
      auto & pat = packet.payload()->psi.pat;
      
      for (auto & section: pat.sections) {
        if (section.is_pmt()) pid = section.program_pid;
      }
      return true;
    });
    
    return pid;
  }
  
  void splitter::find_pmt(uint32_t pid, std::function<void(psi::pmt *)> callback) {
    std::vector<byte_t> payload_data;
    
    this->each_packet([this, &payload_data, &pid](ts::packet & packet){
      if (packet.header.pid != pid || packet.header.payload_unit_start_indicator != 1) return false;
      packet.copy_payload(payload_data);
      
      auto & pmt = packet.payload()->psi.pmt;
      
      if (packet.payload_size() < pmt.size()) {
        this->each_packet([&](ts::packet & packet){
          if (packet.header.pid != pid || packet.header.payload_unit_start_indicator != 0) return false;
          packet.copy_payload(payload_data);
          return true;
        });
      }
      return true;
    });
    
    if (payload_data.size() > 0) {
      auto pmt = reinterpret_cast<psi::pmt *>(payload_data.data());
      callback(pmt);
    }
  }
  
  uint32_t splitter::get_pmt_status(void) {
    auto pmt_pid = this->get_pmt_pid();
    auto status = pmt_status_type::NO_CHANGE;
    
    this->find_pmt(pmt_pid, [this, &status](psi::pmt * pmt){
      auto video_pid = 0;
      auto audio_pid = 0;
      
      for (auto & section: pmt->sections) {
        uint32_t pid = section.elementary_pid;
        if (section.is_video_stream()) video_pid = pid;
        if (section.is_audio_stream()) audio_pid = pid;
      }
      
      if (video_pid != 0) {
        if      (this->video_pid == 0        ) status = pmt_status_type::FIRST;
        else if (this->video_pid != video_pid) status = pmt_status_type::CHANGE;
        this->video_pid = video_pid;
      }
      
      if (audio_pid != 0) {
        if      (this->audio_pid == 0        ) status = pmt_status_type::FIRST;
        else if (this->audio_pid != audio_pid) status = pmt_status_type::CHANGE;
        this->audio_pid = audio_pid;
      }
    });
    
    return status;
  }
  
  
  void splitter::wait_stream(std::function<bool(void)> callback) {
    while (!this->ifs.eof() || this->ifs.good()) {
      if (callback()) break;
    }
  }
  
  void splitter::each_packet(std::function<bool(ts::packet &)> callback) {
    this->wait_stream([&callback, this](){
      ts::packet packet;
      this->read_packet(packet);
      auto result = callback(packet);
      this->write_packet(packet);
      return result;
    });
  }
  
  bool splitter::open_istream(void) {
    this->ifs.open(this->ipath.get_name(), std::ios::in | std::ios::binary);
    if (!this->ifs) {
      std::cerr << "Invalid input file!" << std::endl;
      return false;
    }
    return true;
  }
  
  bool splitter::open_ostream(void) {
    std::string path = this->opath.get_name();
    
    if (this->ofs.is_open()) {
      this->ofs.close();
      ++this->count;
    }
    
    if (this->opath.is_replacable() && this->count != 0) {
      path = this->opath.replace(".ts", (boost::format("_%d.ts") % this->count).str());
    }
    
    this->ofs.open(path, std::ios::out | std::ios::binary);
    
    if (!this->ofs) {
      std::cerr << "Invalid output file!" << std::endl;
      return false;
    }
    return true;
  }
  
  void splitter::read_packet(ts::packet & packet) {
    this->ifs.read(reinterpret_cast<char *>(packet.data.data()), packet.data.size());
  }
  
  void splitter::write_packet(ts::packet & packet) {
    this->ofs.write(reinterpret_cast<char *>(packet.data.data()), packet.data.size());
  }
}}
