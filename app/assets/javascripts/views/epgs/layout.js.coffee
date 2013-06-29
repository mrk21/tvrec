$$.module 'View.Epgs', (self) ->
  class self.Layout extends Backbone.Marionette.Layout
    template: JST['templates/epgs/layout']
    templateHelpers: $$.Helper.use('epgs')
    regions:
      navRegion: '#epgs_nav_region'
      bodyRegion: '#epgs_body_region'
    
    showTableView: (date) ->
      epgs = new $$.Model.EpgCollection([], {date: date})
      channels = new $$.Model.ChannelCollection()
      
      $.when(epgs.fetch(), channels.fetch()).done =>
        @tableView = new self.TableView(collection: epgs, channels: channels)
        @bodyRegion.show(@tableView)
    
    showNavView: ->
      collection = new $$.Model.Widget.NavCollection
      _.chain(0).range(7).each (i) ->
        date = $$.Model.EpgModel.today().add(i,'day')
        uri = $$.Router.path('epgs','list', date: date)
        collection.push(id: "date-"+ date.format('YYYY-MM-DD'), uri: uri, date: date)
      collection.models[0].set('default', true)
      @navView = new $$.View.Widget.NavView
        collection: collection
        content: (params) -> params.model.get('date').format('YYYY年MM月DD日')
      @navRegion.show(@navView)
