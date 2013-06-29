$$.module 'View.Videos', (self) ->
  class self.ListItemView extends $$.View.Widget.ListItemView
    template: JST['templates/videos/list_item_view']
    
  class self.ListView extends $$.View.Widget.ListView
    itemView: self.ListItemView
