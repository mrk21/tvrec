class Epg < ActiveRecord::Base
  belongs_to :channel
  belongs_to :video
  
  scope :in_channel, -> channel {
    channel = channel.id if channel.kind_of? ActiveRecord::Base
    where channel_id: channel
  }
  
  scope :in_date, -> date = Date.today {
    date = date.to_date.to_s unless date.kind_of? String
    date_start = Time.zone.parse(date) + 4.hour
    date_end = date_start + 1.day
    where ["? <= start_time and start_time < ?", date_start, date_end]
  }
  
  scope :in_hour, -> hour = Time.zone.now.hour {
    hour -= Time.zone.now.time_zone.utc_offset / 1.hour
    where ["hour(start_time) = ?", hour]
  }
  
  scope :order_by_time, -> { order [:start_time, :channel_id] }
  
  def program
    return self.video unless self.video.nil?
    video = Video.create(
      channel: self.channel,
      title: self.title,
      description: self.description,
      start_time: self.start_time,
      stop_time: self.stop_time
    )
    self.update_attributes(video_id: video.id)
    video
  end
end
