json.array!(@epgs) do |epg|
  json.extract! epg, :id, :channel_id, :title, :description, :start_time, :stop_time
end
