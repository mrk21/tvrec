class Tuner
  TMPDIR = File.join(Rails.root, "tmp/cache/tuner")
  
  def self.tmpfile(name,ext)
    FileUtils.mkdir_p TMPDIR
    File.join TMPDIR, "#{name}.#{ext}"
  end
  
  def self.search_channel
    FileUtils.rm_rf TMPDIR
    Channel.delete_all
    ChannelTraits.channel_list.each do |ch|
      puts "# channel #{ch}..."
      tmpfile, level = Recorder.test_record(ch,4)
      
      if level < ChannelTraits::LIMIT_LEVEL then
        puts "FAILED!! #{level}dB"
        FileUtils.rm_f tmpfile
      else
        puts "OK! #{level}dB"
        epg_dumper = EpgDumper.new(ch)
        epg_dumper.dump
        epg_dumper.store
      end
    end
  end
  
  def self.update_epg
    FileUtils.rm_rf TMPDIR
    Channel.all.each do |ch|
      puts "# channel #{ch.physical_no}..."
      epg_dumper = EpgDumper.new(ch.physical_no)
      epg_dumper.dump
      epg_dumper.store
    end
  end
end
