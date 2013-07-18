require 'tempfile'

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
    [tmpfile, level || 0]
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
      tempfile = Tempfile.new('tvrec-tuner-recorder-record')
      `#{Rails.root}/bin/record #{channel} #{sec} #{path} #{tempfile.path} 2>&1`
      status = $?
      responce = tempfile.read
      tempfile.close
      tempfile.unlink
      result = [path, status, responce]
      return result if status.exitstatus == 0
      sec -= 1 if sec > 1
      sleep 1
    end
    result
  end
end
