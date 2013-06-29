class Tuner::Recorder
  REC_BASEDIR = '/media/video/ts'
  
  def self.test_record(channel, sec)
    tmpfile = Tuner.tmpfile(channel,:ts)
    level = nil
    loop do
      responce = `recpt1 --b25 #{channel} #{sec} #{tmpfile} 2>&1`
      matches = responce.match(/^.*C\/N = (.+)dB.*$/m)
      level = case
        when matches.nil? then 0
        when matches[1] == '-nan' then redo
        else matches[1].to_f
      end
      break
    end
    [tmpfile, level]
  end
  
  def self.record(channel, sec, name)
    FileUtils.mkdir_p REC_BASEDIR
    path = File.join(REC_BASEDIR, "#{name}.ts")
    responce = `recpt1 --b25 #{channel} #{sec} '#{path}' 2>&1`
    [path, $?, responce]
  end
end
