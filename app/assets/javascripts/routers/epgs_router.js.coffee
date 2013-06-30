$$.module 'Router', (self) ->
  self.on 'before:start', ->
    $$.routers.epgs = new self.EpgsRouter
  
  class self.EpgsRouter extends self.ApplicationRouter
    routes:
      'epgs': 'index'
      'epgs/in/:date': 'list'
      'epgs/:id/reservation': 'reservation'
    
    index: -> @list()
    
    list: (date) -> 
      @_setParams(date: date)
      @_setup()
      @layout.showTableView(@params.date)
    
    _setup: ->
      @layout ||= new $$.View.Epgs.Layout
      return if $$.contentRegion.currentView == @layout
      $$.contentRegion.show(@layout)
      @layout.showNavView()
    
    _checkParams: (params={}) ->
      params.date = $$.Model.EpgModel.format_date(params.date)
      params
