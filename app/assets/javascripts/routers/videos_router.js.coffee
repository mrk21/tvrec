$$.module 'Router', (self) ->
  self.on 'before:start', ->
    $$.routers.videos = new self.VideosRouter
  
  class self.VideosRouter extends self.ApplicationRouter
    routes:
      'videos': 'index'
      'videos/:id': 'show'
    
    index: ->
      @_setParams(id: null)
      @_setup()
    
    show: (id) ->
      @_setParams(id: id)
      @_setup()
    
    _setup: ->
      videos = new $$.Model.VideoCollection()
      videos.fetch().done =>
        video = videos.get(@params.id) || videos.at(0) || new $$.Model.VideoModel
        @videoListView = new $$.View.Videos.ListView(collection: videos)
        $$.contentRegion.show(@videoListView)
