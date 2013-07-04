require 'rexml/document'

class Tuner::EpgDumper
  attr_reader :channel, :data
  
  def initialize(channel)
    @channel = channel
  end
  
  def dump(sec = 60)
    loop do
      tsfile, level, status = Tuner::Recorder.test_record(@channel, sec)
      nil unless status.to_i == 0
      xmlfile = Tuner.tmpfile(@channel,:xml)
      system "epgdump #{@channel} #{tsfile} #{xmlfile}"
      redo unless File.exists? xmlfile
      break
    end
    self.read
  end
  
  def read
    xml_to_hash = lambda do |elem|
      attrs = elem.attributes.map{|v| [v[0].gsub('-','_').intern, v[1]]}
      elems = elem.elements.map{|v| [v.name.gsub('-','_').intern, v.text]}
      Hash[*[attrs, elems].flatten]
    end
    
    xmlfile = Tuner.tmpfile(@channel,:xml)
    tvxml = REXML::Document.new File.read(xmlfile)
    @data = {
      channel: xml_to_hash[tvxml.root.get_elements('channel')[0]],
      programmes: tvxml.root.get_elements('programme').map(&xml_to_hash)
    }
  end
  
  def store
    channel = Channel.where(physical_no: @channel).first
    channel = Channel.create(physical_no: @channel, name: @data[:channel][:display_name]) if channel.nil?
    start_time = Epg.start_time
    Epg.delete_all(['channel_id = ? AND start_time >= ?', channel.id, start_time])
    
    @data[:programmes].each do |programme|
      next if Time.zone.parse(programme[:start]) < start_time
      Epg.create(
        channel: channel,
        title: programme[:title],
        description: programme[:desc],
        start_time: programme[:start],
        stop_time: programme[:stop]
      )
    end
  end
end
