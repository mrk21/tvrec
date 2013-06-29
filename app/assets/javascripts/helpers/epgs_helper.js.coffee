$$.module 'Helper', (self) ->
  class self.EpgsHelper
    initEpgTable: ->
      @_timeRange = _.chain(4*60).range(28*60)
      @_timetable = @collection.groupBy (model) -> model.timeValue()
      @_timetable = _(@_timetable).reduce (memo, value, key) ->
        memo[key] = _(value).groupBy (model) -> model.get('channel_id')
        memo
      , {}
    
    getRowSpan: (epg) ->
      timeValue = epg.timeValue()
      endTimeValue = 28*60
      airtime = epg.airtime()
      airtime = endTimeValue - timeValue if endTimeValue < (timeValue + airtime)
      airtime
    
    getCellInfo: (tv,ch) ->
      @_timetable[tv] ||= {}
      @_timetable[tv][ch.id] ||= 'empty'
      cell = @_timetable[tv][ch.id]
      
      return cell unless _.isArray(cell)
      _.chain(tv).range(tv + cell[0].airtime()).each (t) =>
        @_timetable[t] ||= {}
        @_timetable[t][ch.id] = 'reserved'
      cell[0]
