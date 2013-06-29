$$.module 'View.Widget', (self) ->
  class self.NavItemView extends Backbone.Marionette.ItemView
    template: JST['templates/widget/nav_item_view']
    templateHelpers: $$.Helper.use()
    tagName: 'li'
    className: -> @model.id
    
    serializeData: ->
      model: @model
      content: @options.content(model: @model)
    
    active: -> @$el.addClass('current')
    inactive: -> @$el.removeClass('current')
    select: -> @trigger('select')
  
  class self.NavView extends Backbone.Marionette.CollectionView
    tagName: 'ul'
    className: 'nav'
    itemView: self.NavItemView
    itemViewOptions: => content: @options.content
    
    constructor: (options) ->
      super(options)
      @options.content ||= (params) => params.model.uri()
      @on('itemview:select', @onSelect)
      Backbone.history.on('all', @onChangeUri)
      @onChangeUri()
    
    onChangeUri: =>
      currentUri = URI('/'+ Backbone.history.fragment)
      defaultView = @children.find (v) -> v.model.get('default')
      currentView = defaultView if defaultView && defaultView.model.isCurrent(currentUri)
      currentView = @children.find((v) -> v.model.isCurrent(currentUri)) unless currentView
      @onSelect(currentView)
    
    onSelect: (itemView) =>
      unless itemView
        @currentItemView.inactive() if @currentItemView
        @currentItemView = @previousItemView = null
        return
      @previousItemView = @currentItemView if @currentItemView
      @previousItemView.inactive() if @previousItemView
      @currentItemView = itemView
      @currentItemView.active()
