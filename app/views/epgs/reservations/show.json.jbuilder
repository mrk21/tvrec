json.id @epg.id

if @epg.video then
  json.video_id @epg.video.id
else
  json.video_id nil
end
