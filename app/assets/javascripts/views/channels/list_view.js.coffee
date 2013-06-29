$$.module 'View.Channels', (self) ->
  class self.ListItemView extends $$.View.Widget.ListItemView
    template: JST['templates/channels/list_item_view']
    
  class self.ListView extends $$.View.Widget.ListView
    itemView: self.ListItemView
