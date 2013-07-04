class Epg < ActiveRecord::Base
  belongs_to :channel
  belongs_to :video
  
  TIME_OFFSET = 4.hour
  
  scope :in_channel, -> channel {
    channel = channel.id if channel.kind_of? ActiveRecord::Base
    where channel_id: channel
  }
  
  scope :in_date, -> date = Date.today {
    date = date.to_date.to_s unless date.kind_of? String
    date_start = Time.zone.parse(date) + TIME_OFFSET
    date_end = date_start + 1.day
    where ["? <= start_time and start_time < ?", date_start, date_end]
  }
  
  scope :in_hour, -> hour = Time.zone.now.hour {
    hour -= Time.zone.now.time_zone.utc_offset / 1.hour
    where ["hour(start_time) = ?", hour]
  }
  
  scope :order_by_time, -> { order [:start_time, :channel_id] }
  
  
  def self.today
    (Time.zone.now - TIME_OFFSET).to_date
  end
  
  def self.start_time(date_or_time = Time.zone.now)
    date_or_time.to_date + TIME_OFFSET
  end
  
  def reserve
    return false unless self.video.nil?
    return false if Time.zone.now >= self.stop_time
    video = Video.create(
      channel: self.channel,
      title: self.title,
      description: self.description,
      start_time: self.start_time,
      stop_time: self.stop_time
    )
    self.update_attributes(video_id: video.id)
    true
  end
  
  def unreserve
    return false if self.video.nil?
    return false unless self.video.status == Video::STATUS_RESERVED
    return false if Time.zone.now >= self.start_time
    self.video.destroy
    self.update_attributes(video_id: nil)
    true
  end
end
