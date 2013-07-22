require 'nkf'

class Video < ActiveRecord::Base
  STATUS_RESERVED = 0
  STATUS_RECORDING = 1
  STATUS_RECORDED = 2
  
  belongs_to :channel
  after_create :push_job_que
  before_destroy :remove_job_que
  
  def self.record(id)
    video = self.find(id)
    return nil if video.nil?
    return nil unless video.status == STATUS_RESERVED
    video.update_attributes(status: Video::STATUS_RECORDING, job_id: nil)
    
    ch = video.channel.physical_no
    sec = (video.stop_time - video.start_time).to_i
    path = video.filepath
    
    if video.stop_time <= Time.zone.now then
      return nil
    elsif video.start_time <= Time.zone.now then
      sec = (video.stop_time - Time.zone.now).to_i
    else
      start_time = video.start_time - 5.seconds
      sleep 1 while Time.zone.now < start_time
    end
    
    filepath, status, responce = Tuner::Recorder.record(ch,sec,path)
    video.update_attributes(status: Video::STATUS_RECORDED)
    [filepath, status, responce]
  end
  
  def filepath(subpath=nil)
    path = FileManager.to_filename(self.title)
    date = (self.start_time - 4.hours).strftime('%Y/%m/%d')
    File.join(date, "#{path}#{subpath}")
  end
  
  def job
    JobQue.get(self.job_id)
  end
  
  private
  
  def push_job_que
    job_id = JobQue.push(self.start_time - 1.minutes, "Video.record(#{self.id})")
    self.update_attributes(job_id: job_id)
  end
  
  def remove_job_que
    JobQue.remove(self.job_id)
  end
end
