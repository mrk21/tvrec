#include <tssplitter/ts/psi/pmt.hpp>

namespace tssplitter { namespace ts { namespace psi {
/// section_type
  using section_type = pmt::section_type;
  
  std::size_t section_type::size(void) const {
    return es_info_length_type::END_POSITION + this->es_info_length;
  }
  
  bool section_type::is_audio_stream(void) const {
    constexpr static uint32_t AUDIO_STREAM_TYPES[]{
      0x03, // ISO/IEC 11172-3音声
      0x04, // ISO/IEC 13818-3音声
      0x0F, // ISO/IEC 13818-7音声（ADTSトランスポート構造）
      0x11, // ISO/IEC 14496-3音声（ISO/IEC 14496-3 / AMD 1で規定される LATMトランスポート構造）
      0x1C, // ISO/IEC 14496-3音声（DST、ALS、SLSなど何ら追加のトランスポート構造を使用しないもの）
    };
    uint32_t stream_type = this->stream_type;
    for (auto v: AUDIO_STREAM_TYPES) if (stream_type == v) return true;
    return false;
  }
  
  bool section_type::is_video_stream(void) const {
    constexpr static uint32_t VIDEO_STREAM_TYPES[]{
      0x01, // ISO/IEC 11172-2映像
      0x02, // ITU-T勧告H.262|ISO/IEC 13818-2映像またはISO/IEC 11172-2制約パラメータ映像ストリーム
      0x10, // ISO/IEC 14496-2映像
      0x1B, // ITU-T 勧告 H.264|ISO/IEC 14496-10 映像で規定される AVC映像ストリーム
      0x1E, // ISO/IEC 23002-3 で規定される補助映像ストリーム
    };
    uint32_t stream_type = this->stream_type;
    for (auto v: VIDEO_STREAM_TYPES) if (stream_type == v) return true;
    return false;
  }
  
  
  
/// section_type descriptor_container_type
  byteptr section_type::descriptor_container_type::base_address(void) {
    return this->to_byteptr() + es_info_length_type::END_POSITION;
  }
  
  std::size_t section_type::descriptor_container_type::length(void) const {
    return this->member_type<section_type>()->es_info_length;
  }
  
  
  
/// section_container_type
  using section_container_type = pmt::section_container_type;
  
  byteptr section_container_type::base_address(void) {
    return this->to_byteptr() + program_info_length_type::END_POSITION + this->member_type<pmt>()->program_info_length;
  }
  
  std::size_t section_container_type::length(void) const {
    return this->member_type<pmt>()->section_length - (program_info_length_type::END_POSITION + this->member_type<pmt>()->program_info_length);
  }
  
  
  
/// descriptor_container_type
  byteptr pmt::descriptor_container_type::base_address(void) {
    return this->to_byteptr() + program_info_length_type::END_POSITION;
  }
  
  std::size_t pmt::descriptor_container_type::length(void) const {
    return this->member_type<pmt>()->program_info_length;
  }
}}}
