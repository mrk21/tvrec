$$.module 'View.Page', (self) ->
  class self.Layout extends Backbone.Marionette.Layout
    template: JST['templates/page/layout']
    templateHelpers: $$.Helper.use()
    tagName: 'article'
    className: 'page'
    regions:
      navRegion: '#page_nav_region'
      bodyRegion: '#page_body_region'
    
    showNavView: ->
      collection = new $$.Model.Widget.NavCollection [
        {id: 'epgs'    , uri: $$.Router.path('epgs'    ,'index'), title: '番組表', default: true}
        {id: 'videos'  , uri: $$.Router.path('videos'  ,'index'), title: 'ビデオ'    }
        {id: 'channels', uri: $$.Router.path('channels','index'), title: 'チャンネル'}
      ]
      @navView = new $$.View.Widget.NavView
        collection: collection
        content: (params) -> params.model.get('title')
      @navRegion.show(@navView)
