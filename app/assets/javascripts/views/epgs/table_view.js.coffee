$$.module 'View.Epgs', (self) ->
  class self.TableItemView extends Backbone.Marionette.ItemView
    template: JST['templates/epgs/table_item_view']
    templateHelpers: $$.Helper.use('epgs')
    serializeData: -> view: @, model: @model
    tagName: 'td'
    className: 'epgs'
    id: -> "epgs_#{@model.id}"
    attributes: -> rowspan: @templateHelpers.getRowSpan(@model)
    
    events:
      'click button': 'onButtonClick'
    
    modelEvents:
      'change': 'render'
    
    onRender: ->
      classNames = [@className]
      classNames.push('reserved') if @model.isReserved()
      classNames.push('broadcast') if @model.isBroadcast()
      @$el.attr(class: classNames.join(' '))
    
    onButtonClick: (event) =>
      switch $(event.target).data('method')
        when 'create' then @model.reserve()
        when 'delete' then @model.unreserve()
  
  class self.TableView extends Backbone.Marionette.CompositeView
    template: JST['templates/epgs/table_view']
    templateHelpers: $$.Helper.use('epgs')
    itemView: self.TableItemView
    tagName: 'table'
    className: 'epgs'
    
    serializeData: ->
      collection: @options.collection
      channels: @options.channels
    
    appendHtml: (collectionView, itemView, index) ->
      collectionView.$('#'+ itemView.model.cid).replaceWith(itemView.el)
