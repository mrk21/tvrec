class Tuner::Recorder
  if Rails.env == 'production' then
    REC_BASEDIR = '/media/video/ts'
  else
    REC_BASEDIR = '/media/video/test/ts'
  end
  
  RETRY_RECORD_COUNT = 10
  
  def self.path(path)
    File.join(REC_BASEDIR, path)
  end
  
  def self.test_record(channel, sec)
    tmpfile = Tuner.tmpfile(channel,:ts)
    level = nil
    count = RETRY_RECORD_COUNT
    while (count -= 1) > 0 do
      responce = `recpt1 --b25 #{channel} #{sec} #{tmpfile} 2>&1`
      status = $?
      if status.to_i == 256 then
        sleep 1
        next
      end
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
  
  def self.record(channel, sec, path=nil)
    if path.nil? then
      path = '/dev/null'
    else
      path = path.to_s.strip
      path += '.ts' if path.match(/\.ts$/).nil?
      path = self.path(path)
      FileUtils.mkdir_p File.dirname(path)
      path = Shellwords.escape path
    end
    count = RETRY_RECORD_COUNT
    result = []
    while (count -= 1) > 0 do
      responce = `recpt1 --b25 --strip --sid hd #{channel} #{sec} #{path} 2>&1`
      result = [path, $?, responce]
      return result unless result[1].to_i == 256
      sec -= 1 if sec > 0
      sleep 1
    end
    result
  end
end
