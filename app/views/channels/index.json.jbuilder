json.array!(@channels) do |channel|
  json.extract! channel, :id, :name, :physical_no, :logical_no
end
