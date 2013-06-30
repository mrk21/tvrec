json.array!(@epgs) do |epg|
  json.extract! epg, :id, :video_id, :channel_id, :title, :description, :start_time, :stop_time
  if epg.video then
    json.video_status epg.video.status
  else
    json.video_status nil
  end
end
