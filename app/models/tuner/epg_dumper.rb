require 'rexml/document'

class Tuner::EpgDumper
  attr_reader :channel, :data
  
  def initialize(channel)
    @channel = channel
  end
  
  def dump(tsfile=nil)
    tsfile, level = Tuner::Recorder.test_record(@channel, 60) if tsfile.nil?
    xmlfile = Tuner.tmpfile(@channel,:xml)
    system "epgdump #{@channel} #{tsfile} #{xmlfile}"
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
    Epg.delete_all(['channel_id = ?', channel.id])
    
    @data[:programmes].each do |programme|
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
