class Tuner::ChannelTraits
  LIMIT_LEVEL = 20 # dB
  
  def self.channel_list
    (13..62).to_a
  end
end
