json.array!(@videos) do |video|
  json.extract! video, :id, :title, :description, :channel_id, :start_time, :stop_time, :path, :status
end
